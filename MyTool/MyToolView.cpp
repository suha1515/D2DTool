
// MyToolView.cpp : CMyToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MyTool.h"
#endif

#include "MyToolDoc.h"
#include "MyToolView.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "Terrain.h"

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
END_MESSAGE_MAP()

// CMyToolView 생성/소멸
HWND g_hWnd;

CMyToolView::CMyToolView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance())
{
	// TODO: 여기에 생성 코드를 추가합니다.
	g_hWnd = m_hWnd;

	HRESULT hr = 0;

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");
}

CMyToolView::~CMyToolView()
{
	
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
	m_pDeviceMgr->Render_Begin();

	m_pDeviceMgr->Render_End();
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
	

}


void CMyToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}


void CMyToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMouseMove(nFlags, point);
}
