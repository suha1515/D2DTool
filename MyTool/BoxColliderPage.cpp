// BoxColliderPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "BoxColliderPage.h"
#include "afxdialogex.h"


#include "GameObject.h"
#include "BoxCollider.h"
#include "Transform.h"

#include "MainFrm.h"
#include "MyToolView.h"
IMPLEMENT_DYNAMIC(CBoxColliderPage, CPropertyPage)

CBoxColliderPage::CBoxColliderPage()
	: CPropertyPage(IDD_BOXCOLLIDERPAGE)
	, m_width(0)
	, m_height(0)
	, m_PosX(0)
	, m_PosY(0)
{

}

CBoxColliderPage::~CBoxColliderPage()
{
}

void CBoxColliderPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CompSwitch);
	DDX_Control(pDX, IDC_BUTTON1, m_CompAddBtn);
	DDX_Control(pDX, IDC_BUTTON2, m_CompRemoveBtn);
	DDX_Control(pDX, IDC_EDIT1, m_EditWidth);
	DDX_Control(pDX, IDC_EDIT2, m_Editheight);
	DDX_Control(pDX, IDC_EDIT3, m_EditPosX);
	DDX_Control(pDX, IDC_EDIT12, m_EditPosY);
	DDX_Control(pDX, IDC_BUTTON3, m_SetColliderBtn);
	DDX_Text(pDX, IDC_EDIT1, m_width);
	DDX_Text(pDX, IDC_EDIT2, m_height);
	DDX_Text(pDX, IDC_EDIT3, m_PosX);
	DDX_Text(pDX, IDC_EDIT12, m_PosY);
	DDX_Control(pDX, IDC_COMBO1, m_CollideList);
}


BEGIN_MESSAGE_MAP(CBoxColliderPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CBoxColliderPage::OnBnClickedAddComp)
	ON_BN_CLICKED(IDC_BUTTON2, &CBoxColliderPage::OnBnClickedRemoveComp)
	ON_BN_CLICKED(IDC_BUTTON3, &CBoxColliderPage::OnBnClickedSetCollide)
	ON_BN_CLICKED(IDC_CHECK1, &CBoxColliderPage::OnBnClickedCompCheck)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CBoxColliderPage::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CBoxColliderPage �޽��� ó�����Դϴ�.


void CBoxColliderPage::OnBnClickedAddComp()
{
	if (m_pObject != nullptr)
	{
		if (m_pObject->GetComponent<CTransform>() == nullptr)
		{
			wstring alert = m_pObject->GetObjectName() + L"�� ������Ʈ�� Ʈ������ ������Ʈ�� �����ϴ�.";
			MessageBox(alert.c_str(), L"Fail", ERROR);
			return;
		}
		if (m_pObject->GetComponent<CBoxCollider>() != nullptr)
		{
			wstring alert = m_pObject->GetObjectName() + L"�� ������Ʈ�� �̹� �ִϸ����� ������Ʈ�� �ֽ��ϴ�.";
			MessageBox(alert.c_str(), L"Fail", ERROR);
			return;
		}
		CBoxCollider* pComponent = new CBoxCollider;
		pComponent->Initialize(m_pObject);
		pComponent->SetBoxOffset(m_PosX, m_PosY);
		pComponent->SetBoxSize(m_width, m_height);
		pComponent->SetBoxCollider();

		m_pObject->AddComponent(pComponent);
		Update();
	}
}


void CBoxColliderPage::OnBnClickedRemoveComp()
{
	if (m_pObject != nullptr)
	{
		CBoxCollider* pComponent = m_pObject->GetComponent<CBoxCollider>();
		m_pObject->RemoveComponent(typeid(CBoxCollider).name());
		Update();
	}
}


void CBoxColliderPage::OnBnClickedSetCollide()
{

	if (m_pObject != nullptr)
	{
		CBoxCollider* pComponent = m_pObject->GetComponent<CBoxCollider>();
		UpdateData(TRUE);

		pComponent->SetBoxOffset(m_PosX, m_PosY);
		pComponent->SetBoxSize(m_width, m_height);
		int index = m_CollideList.GetCurSel();
		COLLIDE_TYPE type = RIGHT_TOP;
		if (index != -1)
		{
			CString coltype;
			m_CollideList.GetLBText(index, coltype);
			if (coltype == L"���������ﰢ��")
				type = RIGHT_TOP;
			else if (coltype == L"�����ʾƷ��ﰢ��")
				type = RIGHT_BOTTOM;
			else if (coltype == L"�������ﰢ��")
				type = LEFT_TOP;
			else if (coltype == L"���ʾƷ��ﰢ��")
				type = LEFT_BOTTOM;
			else
				type = NORMAL;
			//m_CollideList.AddString(L"���������ﰢ��");
			////m_CollideList.AddString(L"�����ʾƷ��ﰢ��");
			//m_CollideList.AddString(L"�������ﰢ��");
			//m_CollideList.AddString(L"���ʾƷ��ﰢ��");
		}
		pComponent->SetCollideType(type);
		pComponent->SetBoxCollider();

		UpdateData(FALSE);
		Update();
	}

}


void CBoxColliderPage::OnBnClickedCompCheck()
{
	//������Ʈ Ȱ��ȭ��ư�� ��������. ��Ȱ��ȭ �Ǿ�����
	if (m_CompSwitch.GetCheck() == 0)
	{
		m_pObject->GetComponent<CBoxCollider>()->SetOn(false);
		cout << "����" << endl;
	}
	//Ȱ��ȭ���
	else
	{
		cout << "�ѱ�" << endl;
		m_pObject->GetComponent<CBoxCollider>()->SetOn(true);
	}

	cout << "�ִϸ��̼� ���� ����" << endl;
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pMainFrm, L"InspectView pMainFrm is nullptr");

	CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_MSG(pMyToolView, L"InspectView toolview is nullptr");
	Update();
	pMyToolView->Invalidate(FALSE);
}

void CBoxColliderPage::SetObject(CGameObject * pObject)
{
	m_pObject = pObject;
	Update();
}

void CBoxColliderPage::Update()
{
	//���õ� ������Ʈ�� Null�� �ƴҰ��
	if (nullptr != m_pObject)
	{
		//�ڽ� �ݶ��̴� ������Ʈ�� �ִ��� Ȯ��
		CBoxCollider *pComponent = m_pObject->GetComponent<CBoxCollider>();
		if (nullptr != pComponent)
		{
			UpdateData(TRUE);
			m_width = pComponent->GetBoxWidth();
			m_height = pComponent->GetBoxHeight();
			m_PosX = pComponent->GetBoxOffsetX();
			m_PosY = pComponent->GetBoxoffsetY();

			UpdateData(FALSE);
			if (pComponent->GetOn())
			{
				//������Ʈ�� Ȱ��ȭ �Ǿ��ִ��� Ȯ��.
				m_CompSwitch.SetCheck(TRUE);				//������Ʈ Ȱ��ȭ ��ư üũ
				m_CompAddBtn.EnableWindow(FALSE);			//������Ʈ �߰���ư ��Ȱ��ȭ
				m_CompRemoveBtn.EnableWindow(TRUE);			//������Ʈ ���Ź�ư Ȱ��ȭ

				m_EditWidth.EnableWindow(TRUE);				//�ڽ�ũ�� width ��Ʈ�� Ȱ��ȭ
				m_Editheight.EnableWindow(TRUE);			//�ڽ�ũ�� Height ��Ʈ��Ȱ��ȭ

				m_EditPosX.EnableWindow(TRUE);				//������ X ������Ʈ�� Ȱ��ȭ
				m_EditPosY.EnableWindow(TRUE);				//������  Y ������Ʈ�� Ȱ��ȭ
				m_SetColliderBtn.EnableWindow(TRUE);		//�ݶ��̴� ���� ��ư ��ư Ȱ��ȭ
			}
				//������Ʈ�� Ȱ��ȭ ���� ���� ���.
		else
		{
			m_CompSwitch.SetCheck(FALSE);				//������Ʈ Ȱ��ȭ ��ư üũ
			m_CompAddBtn.EnableWindow(FALSE);			//������Ʈ �߰���ư ��Ȱ��ȭ
			m_CompRemoveBtn.EnableWindow(TRUE);			//������Ʈ ���Ź�ư Ȱ��ȭ

			m_EditWidth.EnableWindow(FALSE);				//�ڽ�ũ�� width ��Ʈ�� Ȱ��ȭ
			m_Editheight.EnableWindow(FALSE);			//�ڽ�ũ�� Height ��Ʈ��Ȱ��ȭ

			m_EditPosX.EnableWindow(FALSE);				//������ X ������Ʈ�� Ȱ��ȭ
			m_EditPosY.EnableWindow(FALSE);				//������  Y ������Ʈ�� Ȱ��ȭ
			m_SetColliderBtn.EnableWindow(FALSE);		//�ݶ��̴� ���� ��ư ��ư Ȱ��ȭ
			}
		
		}
		//������Ʈ�� ���°��.
		else
		{
			m_CompSwitch.SetCheck(FALSE);				//������Ʈ Ȱ��ȭ ��ư üũ
			m_CompAddBtn.EnableWindow(TRUE);			//������Ʈ �߰���ư ��Ȱ��ȭ
			m_CompRemoveBtn.EnableWindow(FALSE);			//������Ʈ ���Ź�ư Ȱ��ȭ

			m_EditWidth.EnableWindow(FALSE);				//�ڽ�ũ�� width ��Ʈ�� Ȱ��ȭ
			m_Editheight.EnableWindow(FALSE);			//�ڽ�ũ�� Height ��Ʈ��Ȱ��ȭ

			m_EditPosX.EnableWindow(FALSE);				//������ X ������Ʈ�� Ȱ��ȭ
			m_EditPosY.EnableWindow(FALSE);				//������  Y ������Ʈ�� Ȱ��ȭ
			m_SetColliderBtn.EnableWindow(FALSE);		//�ݶ��̴� ���� ��ư ��ư Ȱ��ȭ
		}
	}
	//���õ� ������Ʈ�� ���°��.
	else
	{
		m_CompSwitch.SetCheck(FALSE);				//������Ʈ Ȱ��ȭ ��ư üũ
		m_CompAddBtn.EnableWindow(FALSE);			//������Ʈ �߰���ư ��Ȱ��ȭ
		m_CompRemoveBtn.EnableWindow(FALSE);			//������Ʈ ���Ź�ư Ȱ��ȭ
		m_EditWidth.EnableWindow(FALSE);				//�ڽ�ũ�� width ��Ʈ�� Ȱ��ȭ
		m_Editheight.EnableWindow(FALSE);			//�ڽ�ũ�� Height ��Ʈ��Ȱ��ȭ
		m_EditPosX.EnableWindow(FALSE);				//������ X ������Ʈ�� Ȱ��ȭ
		m_EditPosY.EnableWindow(FALSE);				//������  Y ������Ʈ�� Ȱ��ȭ
		m_SetColliderBtn.EnableWindow(FALSE);		//�ݶ��̴� ���� ��ư ��ư Ȱ��ȭ
	}

}



void CBoxColliderPage::OnCbnSelchangeCombo1()
{
}


BOOL CBoxColliderPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_CollideList.AddString(L"�簢��");
	m_CollideList.AddString(L"���������ﰢ��");			// 1
	m_CollideList.AddString(L"�����ʾƷ��ﰢ��");			// 2
	m_CollideList.AddString(L"�������ﰢ��");				// 3
	m_CollideList.AddString(L"���ʾƷ��ﰢ��");			// 4

	return TRUE;
}


