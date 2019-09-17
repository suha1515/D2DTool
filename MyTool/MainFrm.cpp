
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MyTool.h"

#include "MainFrm.h"

#include "MyToolView.h"
#include "MiniView.h"
#include "MyForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기


//분할창 사용시 CFrameWnd 에서만  붙일수있는 컨트롤인 OnCreateClient를 재정의한다.
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//창3개 분할.
	m_MainSplitter.CreateStatic(this, 1, 2);
	//툴뷰는 두번째 열에 지정. 아마.. Create할때는 1행2열을 명시하고 접근시에는 배열처럼 접근하는 형식인듯..
	//또한 3번째 매개변수는 View를 상속받는 클래스의 주소를 받는것일까? 끝까지 추적해봤는데 내부구현은 안나온다..
	//CMiniView::GetThisClass(); <- 이게 원본 이것을 Define으로 한것이 RUNTIME_CLASS
	//4번째 매개변수는 해당 뷰의 크기를 나타낸다.
	//5번째는 뷰를 만드는 데 생성 컨텍스트에 대 한 포인터 이다.
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyToolView), CSize(WINCX, WINCY), pContext);

	//열의 너비를 지정. 첫번재 인자는 CSplitterWnd 에서 나눠진 영역 이며 두번째는 이상적인 너비의 픽셀크기,3번째는 최소픽셀크기이다.
	m_MainSplitter.SetColumnInfo(0, 300, 0);

	//위에서 1행 2열중 2열에 ToolView창을 만들었다면 나머지 영역을 두개로 나누는데
	//이때도 CSplitterWnd를 사용한다. 다만 CreateStatic시 첫번째인자로 부모프레임 창의 포인터를 받는데
	//이경우 첫번째로 나눈 m_MainSplitter 주소를 받는다 m_MainSplitter의 첫번째가 this 인이유는 해당 창분할기가
	//CMainFrame의 창을 나누기 때문이다.

	//2행1열로 나눈다. 그리고 또다른 매개변수 2개를 더 추가해야하는데
	// 4번째 매개변수는 창스타일을 지정하는 매개변수이다 기본 매개변수는 WS_CHILD | WS_VISIBLE로 지정되어있으며
	// 자식 윈도우와 해당창을 바로 표시하라는 플래그이다. (윈도우 스타일에 대해 검색해 볼것)
	// 5번째 매개변수는 창을분할할 다른 윈도분할기의 자식 창의 ID로써 현재
	// IdFromRowCol 함수로 0,0 즉 1행1열의 오른쪽 창의 ID를 반환하여 해당 창을 분할하는것이다.
	m_SecondSplitter.CreateStatic(&m_MainSplitter, 2, 1,WS_CHILD|WS_VISIBLE,m_MainSplitter.IdFromRowCol(0,0));



	m_SecondSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
	m_SecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);

	return TRUE;
}
