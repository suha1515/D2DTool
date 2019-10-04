 // TexCompPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "TexCompPage.h"
#include "afxdialogex.h"

#include "GameObject.h"
#include "TextureRenderer.h"
// CTexCompPage 대화 상자입니다.

#include "MainFrm.h"
#include "MyToolView.h"


IMPLEMENT_DYNAMIC(CTexCompPage, CPropertyPage)

CTexCompPage::CTexCompPage()
	: CPropertyPage(IDD_TEXCOMPPAGE)
	, m_sizeX(0)
	, m_sizeY(0)
	,m_pObject(nullptr)
{

}

CTexCompPage::~CTexCompPage()
{
}

void CTexCompPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT9, m_sizeX);
	DDX_Text(pDX, IDC_EDIT10, m_sizeY);
	DDX_Control(pDX, IDC_BUTTON1, m_ChangeBtn);
	DDX_Control(pDX, IDC_EDIT9, m_sizeXEdit);
	DDX_Control(pDX, IDC_EDIT10, m_sizeYEdit);
	DDX_Control(pDX, IDC_CHECKCOMPONENT, m_CompCheckBox);
	DDX_Control(pDX, IDC_BUTTON2, m_ComponentAdd);
	DDX_Control(pDX, IDC_BUTTON4, m_ComponentRemove);
}

void CTexCompPage::SetObject(CGameObject * pObject)
{
	m_pObject = pObject;
	Update();
}

void CTexCompPage::Update()
{
	if (m_pObject != nullptr)
	{
		cout << "오브젝트 지정" << endl;
		CTextureRenderer* pComponent = m_pObject->GetComponent<CTextureRenderer>();
		if (nullptr != pComponent)
		{
			if (pComponent->GetOn())
			{
				m_CompCheckBox.SetCheck(TRUE);
				m_ChangeBtn.EnableWindow(TRUE);
				m_sizeXEdit.EnableWindow(TRUE);
				m_sizeYEdit.EnableWindow(TRUE);
			}
			else
			{
				m_CompCheckBox.SetCheck(FALSE);
				m_ChangeBtn.EnableWindow(FALSE);
				m_sizeXEdit.EnableWindow(FALSE);
				m_sizeYEdit.EnableWindow(FALSE);

			}	
			m_CompCheckBox.EnableWindow(TRUE);
			m_ComponentRemove.EnableWindow(TRUE);

			m_ComponentAdd.EnableWindow(FALSE);
			UpdateData(TRUE);
			m_sizeX = (int)pComponent->GetTexSize().x;
			m_sizeY = (int)pComponent->GetTexSize().y;
			//(*pComponent->GetTexInfo())->
			UpdateData(FALSE);	
		}
		else
		{
			m_ChangeBtn.EnableWindow(FALSE);
			m_sizeXEdit.EnableWindow(FALSE);
			m_sizeYEdit.EnableWindow(FALSE);

			m_CompCheckBox.EnableWindow(FALSE);
			m_ComponentAdd.EnableWindow(TRUE);
			m_ComponentRemove.EnableWindow(FALSE);
		}
	}
	else
	{
		m_ChangeBtn.EnableWindow(FALSE);
		m_sizeXEdit.EnableWindow(FALSE);
		m_sizeYEdit.EnableWindow(FALSE);

		m_CompCheckBox.EnableWindow(FALSE);
		m_ComponentAdd.EnableWindow(FALSE);
		m_ComponentRemove.EnableWindow(FALSE);
	}
}


BEGIN_MESSAGE_MAP(CTexCompPage, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT9, &CTexCompPage::OnEnChangeEdit9)
	ON_BN_CLICKED(IDC_BUTTON1, &CTexCompPage::OnBnClickedChangeTex)
	ON_BN_CLICKED(IDC_CHECKCOMPONENT, &CTexCompPage::OnBnClickedCheckcomponent)
	ON_BN_CLICKED(IDC_BUTTON2, &CTexCompPage::OnBnClickedAddComp)
	ON_BN_CLICKED(IDC_BUTTON4, &CTexCompPage::OnBnClickedRemoveComp)
END_MESSAGE_MAP()


// CTexCompPage 메시지 처리기입니다.


void CTexCompPage::OnEnChangeEdit9()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTexCompPage::OnBnClickedChangeTex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTexCompPage::OnBnClickedCheckcomponent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_CompCheckBox.GetCheck() == 0)
	{
		m_ChangeBtn.EnableWindow(FALSE);
		m_sizeXEdit.EnableWindow(FALSE);
		m_sizeYEdit.EnableWindow(FALSE);
		
		m_pObject->GetComponent<CTextureRenderer>()->SetOn(false);

	}
	else
	{
		m_ChangeBtn.EnableWindow(TRUE);
		m_sizeXEdit.EnableWindow(TRUE);
		m_sizeYEdit.EnableWindow(TRUE);

		m_pObject->GetComponent<CTextureRenderer>()->SetOn(true);
	}
	cout << "오브젝트 정보 갱신" << endl;
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pMainFrm, L"InspectView pMainFrm is nullptr");

	CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_MSG(pMyToolView, L"InspectView toolview is nullptr");

	pMyToolView->Invalidate(FALSE);
}


BOOL CTexCompPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	Update();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTexCompPage::OnBnClickedAddComp()
{
	if (m_pObject->GetComponent<CTextureRenderer>() != nullptr)
	{
		wstring alert = m_pObject->GetObjectName() + L"번 오브젝트는 이미 텍스처 컴포넌트가 있습니다.";
		MessageBox(alert.c_str(),L"Fail",ERROR);
		return;
	}
	
	CTextureRenderer* pComponent = new CTextureRenderer;
	pComponent->Initialize(m_pObject);

	m_pObject->AddComponent(pComponent);
	Update();
}


void CTexCompPage::OnBnClickedRemoveComp()
{
	m_pObject->RemoveComponent(typeid(CTextureRenderer).name());
	Update();
}

