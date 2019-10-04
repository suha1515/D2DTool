 // TexCompPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "TexCompPage.h"
#include "afxdialogex.h"

#include "GameObject.h"
#include "TextureRenderer.h"
// CTexCompPage ��ȭ �����Դϴ�.

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
		cout << "������Ʈ ����" << endl;
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


// CTexCompPage �޽��� ó�����Դϴ�.


void CTexCompPage::OnEnChangeEdit9()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CPropertyPage::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTexCompPage::OnBnClickedChangeTex()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTexCompPage::OnBnClickedCheckcomponent()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	cout << "������Ʈ ���� ����" << endl;
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pMainFrm, L"InspectView pMainFrm is nullptr");

	CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_MSG(pMyToolView, L"InspectView toolview is nullptr");

	pMyToolView->Invalidate(FALSE);
}


BOOL CTexCompPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	Update();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTexCompPage::OnBnClickedAddComp()
{
	if (m_pObject->GetComponent<CTextureRenderer>() != nullptr)
	{
		wstring alert = m_pObject->GetObjectName() + L"�� ������Ʈ�� �̹� �ؽ�ó ������Ʈ�� �ֽ��ϴ�.";
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

