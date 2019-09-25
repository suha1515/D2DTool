
// MyToolView.cpp : CMyToolView 클래스의 구현
//

#include "stdafx.h"
#include "MyToolView.h"

// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MyTool.h"
#endif

#include "MyToolDoc.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MyForm.h"
#include "Terrain.h"

#include "GameObject.h"
#include "Camera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyToolView

IMPLEMENT_DYNCREATE(CMyToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMyToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CMyToolView 생성/소멸
HWND g_hWnd;

CMyToolView::CMyToolView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pCameraMgr(CCameraMgr::GetInstance()),
	m_Cam(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	g_hWnd = m_hWnd;

	HRESULT hr = 0;

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");
}

CMyToolView::~CMyToolView()
{
	cout << "툴뷰소멸자" << endl;
}

BOOL CMyToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMyToolView 그리기

void CMyToolView::OnDraw(CDC* /*pDC*/)
{
	CMyToolDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_Cam->Update();
	m_Cam->SetTransform();

	m_pDeviceMgr->Render_Begin();
	D3DXMATRIX* mat = m_Cam->GetViewProjMatrix();

	
	m_pDeviceMgr->GetLine()->SetWidth(1.f);
	m_pDeviceMgr->GetLine()->Begin();
	{
		//Horizontal
		//1024*1024
		for (int i = 0; i < 64; ++i)
		{
			m_Line[0] = { (-512.f),  (512.f) - (i*16.f),0.0f };
			m_Line[1] = { (512.f),  (512.f) - (i*16.f),0.0f };
			m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(23, 234, 0));
		}
		for (int i = 0; i < 64; ++i)
		{
			m_Line[0] = { (-512.f) + (i*16.f),  (512.f) ,0.0f };
			m_Line[1] = { (-512.f) + (i*16.f), -(512.f) ,0.0f };
			m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(23, 234, 0));
		}
	}
	m_pDeviceMgr->GetLine()->End();


	D3DXMATRIX matTrans;
	
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 1.0f, -1.0f, 1.0f);
	D3DXMATRIX matWorld;
	
	TCHAR szIndex[MIN_STR] = L"";
	

	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE| D3DXSPRITE_SORT_TEXTURE);
	{
		D3DXMatrixTranslation(&matTrans,0.0f,0.0f, 0.0f);
		matWorld = matScale * matTrans;
		swprintf_s(szIndex, L"%d", 4000);
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(),
			szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	}
	m_pDeviceMgr->GetSprite()->End();
	
	for (auto& i : m_Tile)
		i->Render();

	m_pDeviceMgr->Render_End(m_hWnd);
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMyToolView 인쇄

BOOL CMyToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMyToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMyToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMyToolView 진단

#ifdef _DEBUG
void CMyToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMyToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyToolDoc* CMyToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyToolDoc)));
	return (CMyToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyToolView 메시지 처리기


void CMyToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//스크롤 범위를 지정하는곳
	int cx = TILECX*TILEX;
	int cy = TILECY / 2 * TILEY;

	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));

	// 프레임 윈도우를 얻어온다.
	// AfxGetApp: CWinApp의 포인터를 얻어오는 MFC 전역 함수.
	// GetMainWnd: MainFrame의 포인터를 얻어오는 CWinApp의 멤버함수.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrm);

	// MainFrame의 크기를 얻어온다.
	// GetWindowRect: 윈도우의 전체 테두리를 포함한 사각형 크기를 얻어오는 함수.
	// 이 때 left, top, right, bottom은 스크린좌표 기준이다.
	RECT rcMain = {};
	pMainFrm->GetWindowRect(&rcMain);
	::SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	// View의 크기를 얻어온다.
	// GetClientRect: 윈도우의 테두리를 포함하지 않은 순수 클라이언트 영역의 사각형 크기를 얻어오는 함수.
	// 이 때 left, top, rigth, bottom은 클라이언트 좌표 기준이다.
	// 즉, left, top은 항상 0, 0이다.
	RECT rcView = {};
	GetClientRect(&rcView);

	// MainFrame과 View의 간격을 계산.
	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	// MainFrame의 윈도우 위치와 크기를 다시 조정.
	pMainFrm->SetWindowPos(
		nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER);
	

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	RECT rc = {};
	GetClientRect(&rc);
	float winX = rc.right - rc.left;
	float winY = rc.bottom - rc.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_pCameraMgr->SetMainCamera(m_Cam);
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));
	
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

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
	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

}


void CMyToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
	RECT rc = {};
	GetClientRect(&rc);
	float winX = rc.right - rc.left;
	float winY = rc.bottom - rc.top;

	D3DXVECTOR3 vMouse = { float(point.x)- winX*0.5f,(winY*0.5f- float(point.y)),1.0f };
	cout << vMouse.x << " , " << vMouse.y << endl;
	D3DXVECTOR4 vMouse2;
	D3DXMATRIX viewMat = *m_Cam->GetViewMatrix();
	D3DXMATRIX InvViewMat;
	D3DXMatrixInverse(&InvViewMat, NULL, &viewMat);
	D3DXVec3Transform(&vMouse2,&vMouse,&InvViewMat);

	cout <<"변환후 :"<< vMouse2.x << " , " << vMouse2.y << endl;

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitter.GetPane(1, 0));
	NULL_CHECK(pMyForm);

	
	pMyForm->GetTileName();

	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	pGameObject->SetPosition(D3DXVECTOR3(vMouse.x, vMouse2.y, vMouse2.z));
	pGameObject->SetVertex(16, pMyForm->GetTexPos());

}


void CMyToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);
}


void CMyToolView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	RECT rcView = {};
	GetClientRect(&rcView);

	float winX = rcView.right - rcView.left;
	float winY = rcView.bottom - rcView.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(2.0f, 2.0f, 1.0f));
}


void CMyToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
	cout << "키누름" << endl;
	
	if (nChar == VK_LEFT)
	{
		m_Cam->MoveCamera(-5.0f, 0.0f);
		cout << "툴뷰 왼쪽!" << endl;
	}
	else if (nChar == VK_UP)
	{
		m_Cam->MoveCamera(0.0f, 5.0f);
		cout << "툴뷰 위!" << endl;
	}
	else if (nChar == VK_DOWN)
	{
		m_Cam->MoveCamera(0.0f, -5.0f);
	}
	else if (nChar == VK_RIGHT)
	{
		m_Cam->MoveCamera(5.0f, 0.0f);
	}
	Invalidate(FALSE);
}




BOOL CMyToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	
	float scale = 0.0f;
	if (zDelta <= 0)
	{
		scale = -0.1f;
		cout << "마우스 휠 다운" << endl;
	}
	else
	{
		scale = 0.1f;
		cout << "마우스 휠 업" << endl;
	}
	m_Cam->Scaling(scale, scale);
	Invalidate(FALSE);
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
	
}
