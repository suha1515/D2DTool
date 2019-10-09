// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "MyTool.h"
#include "MiniView.h"
#include "MyForm.h"
#include "Camera.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CScrollView)



CMiniView::CMiniView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	 m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pVB(nullptr),m_pIB(nullptr),m_Cam(nullptr),m_texInfo(nullptr)
{
	itileSizeX = 16;
	itileSizeY = 16;
}

CMiniView::~CMiniView()
{
	if(m_Cam!=nullptr)
  		delete m_Cam;
	if (m_pIB != nullptr)
		m_pIB->Release();
	if (m_pVB != nullptr)
		m_pVB->Release();
	cout << "미니뷰 소멸자" << endl;
	
}

BEGIN_MESSAGE_MAP(CMiniView, CScrollView)
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	
	
	//cout << "카메라 위치 : "<<m_Cam->GetPosition().x << " , " << m_Cam->GetPosition().y << endl;
	// TODO: 여기에 그리기 코드를 추가합니다.
	m_pDeviceMgr->Render_Begin();
	if (m_texInfo != nullptr)
	{
		m_Cam->Update();
		m_Cam->SetTransform();
		m_pDeviceMgr->GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pDeviceMgr->GetDevice()->SetIndices(m_pIB);
		m_pDeviceMgr->GetDevice()->SetFVF(FVF_VERTEX);
		m_pDeviceMgr->GetDevice()->SetTexture(0, m_texInfo->pTexture);

		m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDeviceMgr->GetDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			6,
			0,
			2
		);
		m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	}
	m_pDeviceMgr->GetLine()->End();
	m_pDeviceMgr->Render_End(m_hWnd);
}


// CMiniView 진단입니다.


//void CMiniView::AssertValid() const
//{
//	CView::AssertValid();
//}
//
//#ifndef _WIN32_WCE
//void CMiniView::Dump(CDumpContext& dc) const
//{
//	CView::Dump(dc);
//}
void CMiniView::Initialize(CString tileName)
{

	RECT rect;
	this->GetClientRect(&rect);
	float winX = rect.right - rect.left;
	float winY = rect.bottom - rect.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));

	wstring temp = tileName.operator LPCWSTR();
	m_texInfo = m_pTextureMgr->GetTexInfo(L"TILE_MAP",tileName.operator LPCWSTR());
	NULL_CHECK_RETURN(m_texInfo);

	//스크롤 범위를 지정하는곳
	int cx = m_texInfo->tImgInfo.Width;
	int cy = m_texInfo->tImgInfo.Height;
	imgWidth = cx;
	imgHeight = cy;

	itileCountWidth = imgWidth / itileSizeX;
	itileCountHeight = imgHeight / itileSizeY;

	fGapX = (float)itileSizeX / imgWidth;
	fGapY = (float)itileSizeY / imgHeight;
	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));
	
	

	VerTexUpdate();
	Invalidate(FALSE);

}
void CMiniView::VerTexUpdate()
{
	 //TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pVB != nullptr)
	{
		m_pVB->Release();
		m_pVB = nullptr;
	}
	if (m_pIB != nullptr)
	{
		m_pIB->Release();
		m_pIB = nullptr;
	}
	if (m_Cam == nullptr)
		m_Cam = new CCamera;

	RECT rect;
	this->GetClientRect(&rect);

	float winX = (rect.right - rect.left);
	float winY = (rect.bottom - rect.top);

	//cout << "가로 창 : " << winX << endl;
	//cout << "세로 창 : " << winY << endl;

	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_Cam->Update();

	int scrollX = GetScrollPos(0);
	int scrollY = GetScrollPos(1);

	//cout << scrollX << " , " << scrollY << endl;
	

	m_Vertex[0] = Vertex(-winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, (scrollX / (float)imgWidth), (scrollY / (float)imgHeight));
	m_Vertex[1] = Vertex(-winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, (scrollX / (float)imgWidth), ((winY + scrollY) / (float)imgHeight));
	m_Vertex[2] = Vertex(winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, ((winX + scrollX) / (float)imgWidth), (scrollY / (float)imgHeight));
	m_Vertex[3] = Vertex(winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, ((winX + scrollX) / (float)imgWidth), ((winY + scrollY) / (float)imgHeight));

	/*m_Vertex[0] = Vertex(-winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	m_Vertex[1] = Vertex(-winX*0.5f, -imgHeight, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[2] = Vertex(imgWidth  , winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	m_Vertex[3] = Vertex(imgWidth, -imgHeight, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,1.0f);*/

	m_pDeviceMgr->GetDevice()->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, FVF_VERTEX, D3DPOOL_MANAGED, &m_pVB, 0);
	Vertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	v[0] = m_Vertex[0];
	v[1] = m_Vertex[1];
	v[2] = m_Vertex[2];
	v[3] = m_Vertex[3];

	m_pVB->Unlock();

	m_pDeviceMgr->GetDevice()->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);

	WORD* i = 0;
	m_pIB->Lock(0, 0, (void**)&i, 0);
	i[0] = 0, i[1] = 2, i[2] = 1;
	i[3] = 1, i[4] = 2, i[5] = 3;


	m_pIB->Unlock();
}
void CMiniView::Clear()
{
	m_texInfo = nullptr;
	CScrollView::SetScrollSizes(MM_TEXT, CSize(0, 0));

	if (m_pVB != nullptr)
	{
		m_pVB->Release();
		m_pVB = nullptr;
	}
	if (m_pIB != nullptr)
	{
		m_pIB->Release();
		m_pIB = nullptr;
	}
	Invalidate(FALSE);
}
void CMiniView::SetTileSize(int sizeX,int sizeY)
{
	itileSizeX = sizeX;
	itileSizeY = sizeY;
	fGapX = (float)itileSizeX / imgWidth;
	fGapY = (float)itileSizeY / imgHeight;
}


// CMiniView 메시지 처리기입니다.


void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CScrollView::SetScrollSizes(MM_TEXT, CSize(0, 0));

	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);

	//// use alpha channel in texture for alpha 이미지에서 알파값 가져옴
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//// set blending factors so that alpha component determines transparency
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//빛끔
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	m_MousePos.x = 0.0f;
	m_MousePos.y = 0.0f;
}


void CMiniView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnMouseMove(nFlags, point);


}


void CMiniView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_texInfo != nullptr)
		 {
			D3DXVECTOR3 vMouse = { float(point.x) + GetScrollPos(0),float(point.y) + GetScrollPos(1),0.f };
			
			m_MousePos.x = vMouse.x;
			m_MousePos.y = vMouse.y;

			cout << int(vMouse.x) / itileSizeX << " , " << int(vMouse.y) / itileSizeY << endl;
			int indexX = int(vMouse.x) / itileSizeX;
			int indexY = int(vMouse.y) / itileSizeY;

			tex[0].x = indexX*fGapX, tex[0].y = indexY*fGapY;
			tex[1].x = indexX*fGapX, tex[1].y = (indexY + 1)*fGapY;
			tex[2].x = (indexX + 1)*fGapX, tex[2].y = indexY*fGapY;
			tex[3].x = (indexX + 1)*fGapX, tex[3].y = (indexY + 1)*fGapY;

			cout << "텍셀 1 : " << indexX*(fGapX) << " , " << indexY*(fGapY) << endl;
			cout << "텍셀 2 : " << indexX*(fGapX) << " , " << fGapY *(indexY + 1) << endl;
			cout << "텍셀 3 : " << fGapX *(indexX + 1) << " , " << indexY*(fGapY) << endl;
			cout << "텍셀 4 : " << fGapX *(indexX + 1) << " ," << fGapY *(indexY + 1) << endl;

			CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
			NULL_CHECK(pFrameWnd);

			CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitter.GetPane(1, 0));
			NULL_CHECK(pMyForm);
			CString str;
			TCHAR szIndex[MIN_STR] = L"";

			swprintf_s(szIndex, L"%f , %f", tex[0].x, tex[0].y);

			wcout << szIndex << endl;
			str = (LPCTSTR)szIndex;
			cout << str << endl;
			//텍스쳐 좌표 전달.
			if (pMyForm->GetMapTool()->IsWindowVisible())
			{
				if(pMyForm->GetMapTool()->m_InvertY.GetCheck()==0)
				pMyForm->GetMapTool()->Renew(tex);
				else
				{
					XMFLOAT2 temp[2];
					temp[0] = tex[0];
					temp[1] = tex[1];

					tex[0] = tex[2];
					tex[1] = tex[3];
					tex[2] = temp[0];
					tex[3] = temp[1]; 
					pMyForm->GetMapTool()->Renew(tex);
					
				}
				cout << "맵툴에 전달됨" << endl;
			}
			if (pMyForm->GetAnimTool()->m_AnimMaker.IsWindowVisible())
			{
				pMyForm->GetAnimTool()->m_AnimMaker.SetTexture(m_texInfo->textureName.c_str(), tex,&XMFLOAT2(itileSizeX,itileSizeY));
				cout << "애니메이션 툴에 전달됨" << endl;
			}			
		}
	
	}
	

}


void CMiniView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	//m_Cam->SetPosisiton(D3DXVECTOR3(GetScrollPos(0), -GetScrollPos(1),0.0f));
	cout << "수평 값 : " << GetScrollPos(0) << endl;
	VerTexUpdate();
	Invalidate(FALSE);
}


void CMiniView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	//m_Cam->SetPosisiton(D3DXVECTOR3(GetScrollPos(0), -GetScrollPos(1), 0.0f));
	cout << "수직 값 : " << GetScrollPos(1) << endl;
	VerTexUpdate();
	Invalidate(FALSE);
}


void CMiniView::OnSize(UINT nType, int cx, int cy)
{

	CScrollView::OnSize(nType, cx, cy);
		VerTexUpdate();
}


int CMiniView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	cout << "On Create" << endl;
	return 0;
}


