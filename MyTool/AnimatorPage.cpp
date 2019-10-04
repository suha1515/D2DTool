// AnimatorPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "AnimatorPage.h"
#include "afxdialogex.h"

#include "GameObject.h"
// CAnimatorPage 대화 상자입니다.
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
	//선택된 오브젝트가 Null이 아닐경우
	if (nullptr != m_pObject )
	{
		//애니메이터 컴포넌트가 있는지 확인
		CAnimator *pComponent = m_pObject->GetComponent<CAnimator>();
		if (nullptr != pComponent)
		{
			//컴포넌트가 활성화 되있는지 확인
			if (pComponent->GetOn())
			{
				m_CompSwitch.SetCheck(TRUE);		//컴포넌트 활성화 버튼 체크
				m_CompAdd.EnableWindow(FALSE);		//컴포넌트 추가버튼 비활성화
				m_CompRemove.EnableWindow(TRUE);	//컴포넌트 제거버튼 활성화

				m_AnimCombo.EnableWindow(TRUE);	//애니메이션 콤보박스 활성화
				m_ClipCombo.EnableWindow(TRUE);	//클립		 콤보박스 활성화
				
				m_PlayBtn.EnableWindow(TRUE);	//클립 재생버튼 활성화
				m_PauseBtn.EnableWindow(TRUE);  //클립 정지버튼 활성화
				m_InitBtn.EnableWindow(TRUE);	//클립 초기화 버튼 활성화

				m_animListRenew.EnableWindow(TRUE);	//애니메이션 지정 버튼 활성화
			}
			//컴포넌트가 활성화 되지 않은 경우.
			else
			{
				m_CompSwitch.SetCheck(FALSE);		//컴포넌트 활성화 버튼 언체크
				m_CompAdd.EnableWindow(FALSE);		//컴포넌트 추가버튼 비활성화
				m_CompRemove.EnableWindow(TRUE);	//컴포넌트 제거버튼 활성화

				m_AnimCombo.EnableWindow(FALSE);		//애니메이션 콤보박스 비활성화
				m_ClipCombo.EnableWindow(FALSE);		//클립		 콤보박스 비활성화

				m_PlayBtn.EnableWindow(FALSE);		//클립 재생버튼 비활성화
				m_PauseBtn.EnableWindow(FALSE);		//클립 정지버튼 비활성화
				m_InitBtn.EnableWindow(FALSE);		//클립 초기화 버튼 비활성화

				m_animListRenew.EnableWindow(FALSE);		//애니메이션 지정 버튼 비활성화
			}
		}
		//컴포넌트가 없는경우.
		else
		{
			m_CompSwitch.SetCheck(FALSE);		//컴포넌트 활성화 버튼 언체크
			m_CompAdd.EnableWindow(TRUE);		//컴포넌트 추가버튼 활성화
			m_CompRemove.EnableWindow(FALSE);	//컴포넌트 제거버튼 비활성화

			m_AnimCombo.EnableWindow(FALSE);	//애니메이션 콤보박스 비활성
			m_ClipCombo.EnableWindow(FALSE);	//클립		 콤보박스 비활성

			m_PlayBtn.EnableWindow(FALSE);		//클립 재생버튼 비활성화
			m_PauseBtn.EnableWindow(FALSE);		//클립 정지버튼 비활성화
			m_InitBtn.EnableWindow(FALSE);		//클립 초기화 버튼 비활성화

			m_animListRenew.EnableWindow(FALSE);		//애니메이션 지정 버튼 비활성화
		}
	}
	//선택된 오브젝트가 없는경우.
	else
	{
		//모두 비활성화
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


// CAnimatorPage 메시지 처리기입니다.


void CAnimatorPage::OnBnClickedCompOn()
{
	//컴포넌트 활성화버튼이 눌렸을때. 비활성화 되었으면
	if (m_CompSwitch.GetCheck() == 0)
	{
		m_pObject->GetComponent<CAnimator>()->SetOn(false);
		cout << "끄기" << endl;
	}
	//활성화라면
	else
	{
		cout << "켜기" << endl;
		m_pObject->GetComponent<CAnimator>()->SetOn(true);
	}
	
	cout << "애니메이션 정보 갱신" << endl;
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
			wstring alert = m_pObject->GetObjectName() + L"번 오브젝트는 이미 애니메이터 컴포넌트가 있습니다.";
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
	if (nullptr!= m_pObject&&index!=-1)				//오브젝트도 지정되어 있어야하고 클립도 선택이 되어야 한다.
	{
		CString clipName;
		m_ClipCombo.GetLBText(index, clipName);		//클립 이름을 가져온다.
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
	if (nullptr != m_pObject&&index != -1)				//오브젝트도 지정되어 있어야하고 클립도 선택이 되어야 한다.
	{
		m_pObject->GetComponent<CAnimator>()->Stop();
	}
}


void CAnimatorPage::OnBnClickedReWind()
{
	int index = m_ClipCombo.GetCurSel();
	if (nullptr != m_pObject&&index != -1)				//오브젝트도 지정되어 있어야하고 클립도 선택이 되어야 한다.
	{
		m_pObject->GetComponent<CAnimator>()->ReSet();
	}
}
