// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "MyTool.h"
#include "MiniView.h"
#include "Camera.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CScrollView)



CMiniView::CMiniView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	 m_pTextrueMgr(CTextureMgr::GetInstance())
{

}

CMiniView::~CMiniView()
{
	delete m_Cam;
	if (m_pVB)
		m_pVB->Release();
	if (m_pIB)
		m_pIB->Release();
	if(m_pLine->Release())
		MessageBox(0, L"m_mLine Release Failed", MB_OK);
}

BEGIN_MESSAGE_MAP(CMiniView, CScrollView)
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	RECT rect;
	this->GetClientRect(&rect);

	float winX = rect.right - rect.left;
	float winY = rect.bottom - rect.top;

	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_Cam->Update();

	m_Vertex[0] = Vertex(-winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	m_Vertex[1] = Vertex(-winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, winY / 1024.f);
	m_Vertex[2] = Vertex(winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, winX / 512.f, 0.0f);
	m_Vertex[3] = Vertex(winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, winX / 512.f, winY / 1024.f);

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
	
	// TODO: 여기에 그리기 코드를 추가합니다.
	m_pDeviceMgr->Render_Begin();

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

	D3DXMATRIX* mat = m_Cam->GetViewProjMatrix();
	m_pLine->Begin();

	D3DXVECTOR3 vLine[2];

	vLine[0] = { -winX*0.5f,winY*0.5f,0.0f };
	vLine[1] = { 0.f,0.f,0.0f };

	D3DXVECTOR2 vLine2[2];
	vLine2[0] = { 10.f,0.0f };
	vLine2[1] = { 50.f,0.0f };

	m_pLine->DrawTransform(vLine, 2, mat, D3DCOLOR_XRGB(255, 0, 0));
	m_pLine->SetWidth(3);
	vLine[0] = { 10.0f,0.0f,0.0f };
	vLine[1] = { 50.f,0.f,0.0f };
	m_pLine->DrawTransform(vLine, 2, mat, D3DCOLOR_XRGB(0, 0, 0));


	m_pLine->End();

	m_pDeviceMgr->Render_End(m_hWnd);


}


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
void CMiniView::Initialize()
{

}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.


void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	HRESULT hr = 0;
	hr = m_pTextrueMgr->LoadTexture(L"../Texture/Map/autumn-outside.png", L"MAP_TILE", L"AUTUMN_OUTSIDE");
	FAILED_CHECK_MSG(hr, L"MAP_AUTUMN LOAD FAILED");

	m_texInfo = m_pTextrueMgr->GetTexInfo(L"MAP_TILE", L"AUTUMN_OUTSIDE");
	//스크롤 범위를 지정하는곳
	int cx = m_texInfo->tImgInfo.Width;
	int cy = m_texInfo->tImgInfo.Height;

	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));

	m_Cam = nullptr;

	RECT rect;
	this->GetClientRect(&rect);
	float winX = rect.right - rect.left;
	float winY = rect.bottom - rect.top;

	m_Cam = new CCamera;
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));


	/*hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");*/

	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// use alpha channel in texture for alpha 이미지에서 알파값 가져옴
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// set blending factors so that alpha component determines transparency
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);


	D3DXCreateLine(m_pDeviceMgr->GetDevice(), &m_pLine);
	m_pLine->SetWidth(2);

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

}
