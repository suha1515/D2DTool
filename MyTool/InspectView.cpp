// InspectView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "InspectView.h"
#include "GameObject.h"

#include "MainFrm.h"
#include "MyToolView.h"

//컴포넌트
#include "Transform.h"

// CInspectView

IMPLEMENT_DYNCREATE(CInspectView, CFormView)

CInspectView::CInspectView()
	: CFormView(IDD_INSPECTOR)
	, m_PosX(0)
	, m_PosY(0)
	, m_PosZ(0)
	, m_RotX(0)
	, m_RotY(0)
	, m_RotZ(0)
	, m_ScaleX(0)
	, m_ScaleY(0)
	, m_ScaleZ(0)
{
	ZeroMemory(&m_ScaleY, sizeof(FILETIME));

	m_ClickedObject = nullptr;

}

CInspectView::~CInspectView()
{
}

void CInspectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJNAME, m_ObjectName);
	DDX_Text(pDX, IDC_POSX, m_PosX);
	DDX_Text(pDX, IDC_POSY, m_PosY);
	DDX_Text(pDX, IDC_POSZ, m_PosZ);
	DDX_Text(pDX, IDC_ROTX, m_RotX);
	DDX_Text(pDX, IDC_ROTY, m_RotY);
	DDX_Text(pDX, IDC_ROTZ, m_RotZ);
	DDX_Text(pDX, IDC_SCALEX, m_ScaleX);
	DDX_Text(pDX, IDC_SCALEY, m_ScaleY);
	DDX_Text(pDX, IDC_SCALEY, m_ScaleY);
	DDX_Text(pDX, IDC_SACLEZ, m_ScaleZ);
}

BEGIN_MESSAGE_MAP(CInspectView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CInspectView::OnBnClickedHierarchy)
END_MESSAGE_MAP()


// CInspectView 진단입니다.

#ifdef _DEBUG
void CInspectView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInspectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInspectView 메시지 처리기입니다.
void CInspectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if (nullptr == m_HierarchyView.GetSafeHwnd())
		m_HierarchyView.Create(IDD_HIERARCHY);

	//프로퍼티 시트 작업을 위한 코드.. 뭘까?
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//픽쳐 컨트롤를 가져온다
	CWnd* pPlaceHolder = GetDlgItem(IDC_PLACEHOLDER);
	m_pComponentSheet = new CComponentSheet(pPlaceHolder);

	//프로퍼티 시트를 픽쳐 컨트롤에 생성한다.
	if (!m_pComponentSheet->Create(pPlaceHolder, WS_CHILD | WS_VISIBLE))
	{
		delete m_pComponentSheet;
		m_pComponentSheet = nullptr;
		return;
	}

	CRect rcSheet;

	pPlaceHolder->GetWindowRect(&rcSheet);
	ScreenToClient(&rcSheet);

	//탭 컨트롤을 표시하기위한 작업 아마?
	CTabCtrl * tabCtrl = m_pComponentSheet->GetTabControl();
	tabCtrl->MoveWindow(0, 0, rcSheet.Width(), rcSheet.Height());

	m_pComponentSheet->SetWindowPos(NULL, 0, 0, rcSheet.Width(), rcSheet.Height(),
		SWP_NOZORDER | SWP_NOACTIVATE);

	//스타일 적용후 페이지 활성화.
	m_pComponentSheet->ModifyStyle(0, WS_EX_CONTROLPARENT);
	m_pComponentSheet->ModifyStyle(0, WS_TABSTOP);
	m_pComponentSheet->SetActivePage(0);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CInspectView::OnBnClickedHierarchy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_HierarchyView.ShowWindow(SW_SHOW);
}

void CInspectView::SetObject(CGameObject * object)
{
	m_ClickedObject = object;
}

//에딧박스 정보 갱신
void CInspectView::UpdateInfo()
{
	UpdateData(TRUE);
	if (m_ClickedObject != nullptr)
	{
		m_ObjectName.SetWindowTextW(m_ClickedObject->GetObjectName().c_str());

		CTransform* pTransform = m_ClickedObject->GetComponent<CTransform>();
		NULL_CHECK_MSG_RETURN(pTransform, L"Inspect View GameObject Transform is null");

		//위치 값 갱신
		m_PosX = pTransform->GetPosition().x;
		m_PosY = pTransform->GetPosition().y;
		m_PosZ = pTransform->GetPosition().z;

		//회전 값 갱신
		m_RotX = pTransform->GetRotation().x;
		m_RotY = pTransform->GetRotation().y;
		m_RotZ = pTransform->GetRotation().z;

		//크기 값 갱신.
		m_ScaleX = pTransform->GetScale().x;
		m_ScaleY = pTransform->GetScale().y;
		m_ScaleZ = pTransform->GetScale().z;

		m_pComponentSheet->UpdateInfo(m_ClickedObject);

		
	}
	else
	{
		m_ObjectName.SetWindowTextW(L"");
		//위치 값 갱신
		m_PosX = 0.0f;
		m_PosY = 0.0f;
		m_PosZ = 0.0f;

		//회전 값 갱신
		m_RotX = 0.0f;
		m_RotY = 0.0f;
		m_RotZ = 0.0f;

		//크기 값 갱신.
		m_ScaleX = 0.0f;
		m_ScaleY = 0.0f;
		m_ScaleZ = 0.0f;
	}
	

	UpdateData(FALSE);
}

//오브젝트 정보 갱신
void CInspectView::UpdateObject()
{
	UpdateData(TRUE);
	if (m_ClickedObject != nullptr)
	{
		D3DXVECTOR3 pos(m_PosX, m_PosY, m_PosZ);
		XMFLOAT3	rot(m_RotX, m_RotY, m_RotZ);
		D3DXVECTOR3 scale(m_ScaleX, m_ScaleY, m_ScaleZ);

		CTransform* pTransform = m_ClickedObject->GetComponent<CTransform>();
		NULL_CHECK_MSG_RETURN(pTransform, L"Inspect View GameObject Transform is null");

		pTransform->SetPosition(pos);
		pTransform->SetRotation(rot);
		pTransform->SetScaling(scale);

		CString name;
		m_ObjectName.GetWindowTextW(name);
		m_ClickedObject->SetObjectName(name.operator LPCWSTR());
	}
	UpdateData(FALSE);
}




BOOL CInspectView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			cout << "오브젝트 정보 갱신" << endl;
			CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
			NULL_CHECK_MSG(pMainFrm, L"InspectView pMainFrm is nullptr");

			CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
			NULL_CHECK_MSG(pMyToolView,L"InspectView toolview is nullptr");

			UpdateObject();
			m_HierarchyView.Update();
			pMyToolView->Invalidate(FALSE);
		}
	}



	return CFormView::PreTranslateMessage(pMsg);
}
