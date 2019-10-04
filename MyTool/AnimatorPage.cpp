// AnimatorPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "AnimatorPage.h"
#include "afxdialogex.h"

#include "GameObject.h"
// CAnimatorPage ��ȭ �����Դϴ�.
#include "Animator.h"


#include "MainFrm.h"
#include "MyToolView.h"

IMPLEMENT_DYNAMIC(CAnimatorPage, CPropertyPage)

CAnimatorPage::CAnimatorPage()
	: CPropertyPage(IDD_ANIMATORPAGE)
{

}

CAnimatorPage::~CAnimatorPage()
{
}

void CAnimatorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK2, m_CompSwitch);
	DDX_Control(pDX, IDC_COMBO1, m_AnimCombo);
	DDX_Control(pDX, IDC_COMBO2, m_ClipCombo);
	DDX_Control(pDX, IDC_BUTTON1, m_CompAdd);
	DDX_Control(pDX, IDC_BUTTON2, m_CompRemove);
	DDX_Control(pDX, IDC_BUTTON4, m_PlayBtn);
	DDX_Control(pDX, IDC_BUTTON8, m_PauseBtn);
	DDX_Control(pDX, IDC_BUTTON9, m_InitBtn);
	DDX_Control(pDX, IDC_BUTTON5, m_animListRenew);
	DDX_Control(pDX, IDC_CHECK1, m_LoopCheck);
}

void CAnimatorPage::SetObject(CGameObject * pObject)
{
	m_pObject = pObject;
	Update();
}

void CAnimatorPage::Update()
{
	//���õ� ������Ʈ�� Null�� �ƴҰ��
	if (nullptr != m_pObject )
	{
		//�ִϸ����� ������Ʈ�� �ִ��� Ȯ��
		CAnimator *pComponent = m_pObject->GetComponent<CAnimator>();
		if (nullptr != pComponent)
		{
			//������Ʈ�� Ȱ��ȭ ���ִ��� Ȯ��
			if (pComponent->GetOn())
			{
				m_CompSwitch.SetCheck(TRUE);		//������Ʈ Ȱ��ȭ ��ư üũ
				m_CompAdd.EnableWindow(FALSE);		//������Ʈ �߰���ư ��Ȱ��ȭ
				m_CompRemove.EnableWindow(TRUE);	//������Ʈ ���Ź�ư Ȱ��ȭ

				m_AnimCombo.EnableWindow(TRUE);	//�ִϸ��̼� �޺��ڽ� Ȱ��ȭ
				m_ClipCombo.EnableWindow(TRUE);	//Ŭ��		 �޺��ڽ� Ȱ��ȭ
				
				m_PlayBtn.EnableWindow(TRUE);	//Ŭ�� �����ư Ȱ��ȭ
				m_PauseBtn.EnableWindow(TRUE);  //Ŭ�� ������ư Ȱ��ȭ
				m_InitBtn.EnableWindow(TRUE);	//Ŭ�� �ʱ�ȭ ��ư Ȱ��ȭ

				m_animListRenew.EnableWindow(TRUE);	//�ִϸ��̼� ���� ��ư Ȱ��ȭ
			}
			//������Ʈ�� Ȱ��ȭ ���� ���� ���.
			else
			{
				m_CompSwitch.SetCheck(FALSE);		//������Ʈ Ȱ��ȭ ��ư ��üũ
				m_CompAdd.EnableWindow(FALSE);		//������Ʈ �߰���ư ��Ȱ��ȭ
				m_CompRemove.EnableWindow(TRUE);	//������Ʈ ���Ź�ư Ȱ��ȭ

				m_AnimCombo.EnableWindow(FALSE);		//�ִϸ��̼� �޺��ڽ� ��Ȱ��ȭ
				m_ClipCombo.EnableWindow(FALSE);		//Ŭ��		 �޺��ڽ� ��Ȱ��ȭ

				m_PlayBtn.EnableWindow(FALSE);		//Ŭ�� �����ư ��Ȱ��ȭ
				m_PauseBtn.EnableWindow(FALSE);		//Ŭ�� ������ư ��Ȱ��ȭ
				m_InitBtn.EnableWindow(FALSE);		//Ŭ�� �ʱ�ȭ ��ư ��Ȱ��ȭ

				m_animListRenew.EnableWindow(FALSE);		//�ִϸ��̼� ���� ��ư ��Ȱ��ȭ
			}
		}
		//������Ʈ�� ���°��.
		else
		{
			m_CompSwitch.SetCheck(FALSE);		//������Ʈ Ȱ��ȭ ��ư ��üũ
			m_CompAdd.EnableWindow(TRUE);		//������Ʈ �߰���ư Ȱ��ȭ
			m_CompRemove.EnableWindow(FALSE);	//������Ʈ ���Ź�ư ��Ȱ��ȭ

			m_AnimCombo.EnableWindow(FALSE);	//�ִϸ��̼� �޺��ڽ� ��Ȱ��
			m_ClipCombo.EnableWindow(FALSE);	//Ŭ��		 �޺��ڽ� ��Ȱ��

			m_PlayBtn.EnableWindow(FALSE);		//Ŭ�� �����ư ��Ȱ��ȭ
			m_PauseBtn.EnableWindow(FALSE);		//Ŭ�� ������ư ��Ȱ��ȭ
			m_InitBtn.EnableWindow(FALSE);		//Ŭ�� �ʱ�ȭ ��ư ��Ȱ��ȭ

			m_animListRenew.EnableWindow(FALSE);		//�ִϸ��̼� ���� ��ư ��Ȱ��ȭ
		}
	}
	//���õ� ������Ʈ�� ���°��.
	else
	{
		//��� ��Ȱ��ȭ
		m_CompSwitch.SetCheck(FALSE);		
		m_CompAdd.EnableWindow(FALSE);		
		m_CompRemove.EnableWindow(FALSE);	

		m_AnimCombo.EnableWindow(FALSE);	
		m_ClipCombo.EnableWindow(FALSE);	

		m_PlayBtn.EnableWindow(FALSE);		
		m_PauseBtn.EnableWindow(FALSE);		
		m_InitBtn.EnableWindow(FALSE);		

		m_animListRenew.EnableWindow(FALSE);
	}
}


BEGIN_MESSAGE_MAP(CAnimatorPage, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK2, &CAnimatorPage::OnBnClickedCompOn)
	ON_BN_CLICKED(IDC_BUTTON5, &CAnimatorPage::OnBnClickedRenewList)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimatorPage::OnBnClickedAddComp)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnimatorPage::OnBnClickedRemoveComp)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimatorPage::OnBnClickedRenewClip)
	ON_BN_CLICKED(IDC_BUTTON6, &CAnimatorPage::OnBnClickedSetAnim)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnimatorPage::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON8, &CAnimatorPage::OnBnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON9, &CAnimatorPage::OnBnClickedReWind)
END_MESSAGE_MAP()


// CAnimatorPage �޽��� ó�����Դϴ�.


void CAnimatorPage::OnBnClickedCompOn()
{
	//������Ʈ Ȱ��ȭ��ư�� ��������. ��Ȱ��ȭ �Ǿ�����
	if (m_CompSwitch.GetCheck() == 0)
	{
		m_pObject->GetComponent<CAnimator>()->SetOn(false);
		cout << "����" << endl;
	}
	//Ȱ��ȭ���
	else
	{
		cout << "�ѱ�" << endl;
		m_pObject->GetComponent<CAnimator>()->SetOn(true);
	}
	
	cout << "�ִϸ��̼� ���� ����" << endl;
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK_MSG(pMainFrm, L"InspectView pMainFrm is nullptr");

	CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_MSG(pMyToolView, L"InspectView toolview is nullptr");
	Update();
	pMyToolView->Invalidate(FALSE);
}


void CAnimatorPage::OnBnClickedRenewList()
{
	m_AnimCombo.ResetContent();
	for (auto&i : CAnimationMgr::GetInstance()->GetClip())
	{
		m_AnimCombo.AddString(i.first.c_str());
	}

	m_ClipCombo.ResetContent();
}


void CAnimatorPage::OnBnClickedAddComp()
{
	if (m_pObject != nullptr)
	{
		if (m_pObject->GetComponent<CAnimator>() != nullptr)
		{
			wstring alert = m_pObject->GetObjectName() + L"�� ������Ʈ�� �̹� �ִϸ����� ������Ʈ�� �ֽ��ϴ�.";
			MessageBox(alert.c_str(), L"Fail", ERROR);
			return;
		}

		CAnimator* pComponent = new CAnimator;
		pComponent->Initialize(m_pObject);

		m_pObject->AddComponent(pComponent);
		Update();
	}
	
}


void CAnimatorPage::OnBnClickedRemoveComp()
{
	if (m_pObject != nullptr)
	{
		CAnimator* pComponent = m_pObject->GetComponent<CAnimator>();
		m_pObject->RemoveComponent(typeid(CAnimator).name());
		Update();
	}
}


void CAnimatorPage::OnBnClickedRenewClip()
{
	int index = m_AnimCombo.GetCurSel();
	if (index != -1)
	{
		m_ClipCombo.ResetContent();
		CString name;
		m_AnimCombo.GetLBText(index, name);
		auto& temp = CAnimationMgr::GetInstance()->GetClip();
		
		auto iter_find = temp.find(name.operator LPCWSTR());

		for (auto & i : iter_find->second)
		{
			m_ClipCombo.AddString(i._clipName);
		}
	}
}


void CAnimatorPage::OnBnClickedSetAnim()
{
	int index = m_AnimCombo.GetCurSel();
	if (index != -1)
	{
		CString animName;
		m_AnimCombo.GetLBText(index, animName);
		m_pObject->GetComponent<CAnimator>()->LoadClips(animName.operator LPCWSTR());
	}
}


void CAnimatorPage::OnBnClickedPlay()
{
	int index = m_ClipCombo.GetCurSel();
	if (nullptr!= m_pObject&&index!=-1)				//������Ʈ�� �����Ǿ� �־���ϰ� Ŭ���� ������ �Ǿ�� �Ѵ�.
	{
		CString clipName;
		m_ClipCombo.GetLBText(index, clipName);		//Ŭ�� �̸��� �����´�.
		CAnimator* pComponent = m_pObject->GetComponent<CAnimator>();
		if (nullptr != pComponent)
		{
			ANIMATION_TYPE type;
			if (m_LoopCheck.GetCheck())
				type = ANIMATION_LOOP;
			else
				type = ANIMATION_ONCE;
			pComponent->Play(clipName.operator LPCWSTR(), type);
		}
	}
}


void CAnimatorPage::OnBnClickedStop()
{
	int index = m_ClipCombo.GetCurSel();
	if (nullptr != m_pObject&&index != -1)				//������Ʈ�� �����Ǿ� �־���ϰ� Ŭ���� ������ �Ǿ�� �Ѵ�.
	{
		m_pObject->GetComponent<CAnimator>()->Stop();
	}
}


void CAnimatorPage::OnBnClickedReWind()
{
	int index = m_ClipCombo.GetCurSel();
	if (nullptr != m_pObject&&index != -1)				//������Ʈ�� �����Ǿ� �־���ϰ� Ŭ���� ������ �Ǿ�� �Ѵ�.
	{
		m_pObject->GetComponent<CAnimator>()->ReSet();
	}
}
