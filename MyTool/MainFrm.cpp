
// MainFrm.cpp : CMainFrame Ŭ������ ����
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
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
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
	//	TRACE0("���� ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

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


// CMainFrame �޽��� ó����


//����â ���� CFrameWnd ������  ���ϼ��ִ� ��Ʈ���� OnCreateClient�� �������Ѵ�.
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//â3�� ����.
	m_MainSplitter.CreateStatic(this, 1, 2);
	//����� �ι�° ���� ����. �Ƹ�.. Create�Ҷ��� 1��2���� ����ϰ� ���ٽÿ��� �迭ó�� �����ϴ� �����ε�..
	//���� 3��° �Ű������� View�� ��ӹ޴� Ŭ������ �ּҸ� �޴°��ϱ�? ������ �����غôµ� ���α����� �ȳ��´�..
	//CMiniView::GetThisClass(); <- �̰� ���� �̰��� Define���� �Ѱ��� RUNTIME_CLASS
	//4��° �Ű������� �ش� ���� ũ�⸦ ��Ÿ����.
	//5��°�� �並 ����� �� ���� ���ؽ�Ʈ�� �� �� ������ �̴�.
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyToolView), CSize(WINCX, WINCY), pContext);

	//���� �ʺ� ����. ù���� ���ڴ� CSplitterWnd ���� ������ ���� �̸� �ι�°�� �̻����� �ʺ��� �ȼ�ũ��,3��°�� �ּ��ȼ�ũ���̴�.
	m_MainSplitter.SetColumnInfo(0, 300, 0);

	//������ 1�� 2���� 2���� ToolViewâ�� ������ٸ� ������ ������ �ΰ��� �����µ�
	//�̶��� CSplitterWnd�� ����Ѵ�. �ٸ� CreateStatic�� ù��°���ڷ� �θ������� â�� �����͸� �޴µ�
	//�̰�� ù��°�� ���� m_MainSplitter �ּҸ� �޴´� m_MainSplitter�� ù��°�� this �������� �ش� â���ұⰡ
	//CMainFrame�� â�� ������ �����̴�.

	//2��1���� ������. �׸��� �Ǵٸ� �Ű����� 2���� �� �߰��ؾ��ϴµ�
	// 4��° �Ű������� â��Ÿ���� �����ϴ� �Ű������̴� �⺻ �Ű������� WS_CHILD | WS_VISIBLE�� �����Ǿ�������
	// �ڽ� ������� �ش�â�� �ٷ� ǥ���϶�� �÷����̴�. (������ ��Ÿ�Ͽ� ���� �˻��� ����)
	// 5��° �Ű������� â�������� �ٸ� �������ұ��� �ڽ� â�� ID�ν� ����
	// IdFromRowCol �Լ��� 0,0 �� 1��1���� ������ â�� ID�� ��ȯ�Ͽ� �ش� â�� �����ϴ°��̴�.
	m_SecondSplitter.CreateStatic(&m_MainSplitter, 2, 1,WS_CHILD|WS_VISIBLE,m_MainSplitter.IdFromRowCol(0,0));



	m_SecondSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
	m_SecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 300), pContext);

	return TRUE;
}
