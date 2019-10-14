// BoxColliderPage.cpp : 구현 파일입니다.
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


// CBoxColliderPage 메시지 처리기입니다.


void CBoxColliderPage::OnBnClickedAddComp()
{
	if (m_pObject != nullptr)
	{
		if (m_pObject->GetComponent<CTransform>() == nullptr)
		{
			wstring alert = m_pObject->GetObjectName() + L"번 오브젝트는 트랜스폼 컴포넌트가 없습니다.";
			MessageBox(alert.c_str(), L"Fail", ERROR);
			return;
		}
		if (m_pObject->GetComponent<CBoxCollider>() != nullptr)
		{
			wstring alert = m_pObject->GetObjectName() + L"번 오브젝트는 이미 애니메이터 컴포넌트가 있습니다.";
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
			if (coltype == L"오른쪽위삼각형")
				type = RIGHT_TOP;
			else if (coltype == L"오른쪽아래삼각형")
				type = RIGHT_BOTTOM;
			else if (coltype == L"왼쪽위삼각형")
				type = LEFT_TOP;
			else if (coltype == L"왼쪽아래삼각형")
				type = LEFT_BOTTOM;
			else
				type = NORMAL;
			//m_CollideList.AddString(L"오른쪽위삼각형");
			////m_CollideList.AddString(L"오른쪽아래삼각형");
			//m_CollideList.AddString(L"왼쪽위삼각형");
			//m_CollideList.AddString(L"왼쪽아래삼각형");
		}
		pComponent->SetCollideType(type);
		pComponent->SetBoxCollider();

		UpdateData(FALSE);
		Update();
	}

}


void CBoxColliderPage::OnBnClickedCompCheck()
{
	//컴포넌트 활성화버튼이 눌렸을때. 비활성화 되었으면
	if (m_CompSwitch.GetCheck() == 0)
	{
		m_pObject->GetComponent<CBoxCollider>()->SetOn(false);
		cout << "끄기" << endl;
	}
	//활성화라면
	else
	{
		cout << "켜기" << endl;
		m_pObject->GetComponent<CBoxCollider>()->SetOn(true);
	}

	cout << "애니메이션 정보 갱신" << endl;
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
	//선택된 오브젝트가 Null이 아닐경우
	if (nullptr != m_pObject)
	{
		//박스 콜라이더 컴포넌트가 있는지 확인
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
				//컴포넌트가 활성화 되어있는지 확인.
				m_CompSwitch.SetCheck(TRUE);				//컴포넌트 활성화 버튼 체크
				m_CompAddBtn.EnableWindow(FALSE);			//컴포넌트 추가버튼 비활성화
				m_CompRemoveBtn.EnableWindow(TRUE);			//컴포넌트 제거버튼 활성화

				m_EditWidth.EnableWindow(TRUE);				//박스크기 width 컨트롤 활성화
				m_Editheight.EnableWindow(TRUE);			//박스크기 Height 컨트롤활성화

				m_EditPosX.EnableWindow(TRUE);				//오프셋 X 에딧컨트롤 활성화
				m_EditPosY.EnableWindow(TRUE);				//오프셋  Y 에딧컨트롤 활성화
				m_SetColliderBtn.EnableWindow(TRUE);		//콜라이더 지정 버튼 버튼 활성화
			}
				//컴포넌트가 활성화 되지 않은 경우.
		else
		{
			m_CompSwitch.SetCheck(FALSE);				//컴포넌트 활성화 버튼 체크
			m_CompAddBtn.EnableWindow(FALSE);			//컴포넌트 추가버튼 비활성화
			m_CompRemoveBtn.EnableWindow(TRUE);			//컴포넌트 제거버튼 활성화

			m_EditWidth.EnableWindow(FALSE);				//박스크기 width 컨트롤 활성화
			m_Editheight.EnableWindow(FALSE);			//박스크기 Height 컨트롤활성화

			m_EditPosX.EnableWindow(FALSE);				//오프셋 X 에딧컨트롤 활성화
			m_EditPosY.EnableWindow(FALSE);				//오프셋  Y 에딧컨트롤 활성화
			m_SetColliderBtn.EnableWindow(FALSE);		//콜라이더 지정 버튼 버튼 활성화
			}
		
		}
		//컴포넌트가 없는경우.
		else
		{
			m_CompSwitch.SetCheck(FALSE);				//컴포넌트 활성화 버튼 체크
			m_CompAddBtn.EnableWindow(TRUE);			//컴포넌트 추가버튼 비활성화
			m_CompRemoveBtn.EnableWindow(FALSE);			//컴포넌트 제거버튼 활성화

			m_EditWidth.EnableWindow(FALSE);				//박스크기 width 컨트롤 활성화
			m_Editheight.EnableWindow(FALSE);			//박스크기 Height 컨트롤활성화

			m_EditPosX.EnableWindow(FALSE);				//오프셋 X 에딧컨트롤 활성화
			m_EditPosY.EnableWindow(FALSE);				//오프셋  Y 에딧컨트롤 활성화
			m_SetColliderBtn.EnableWindow(FALSE);		//콜라이더 지정 버튼 버튼 활성화
		}
	}
	//선택된 오브젝트가 없는경우.
	else
	{
		m_CompSwitch.SetCheck(FALSE);				//컴포넌트 활성화 버튼 체크
		m_CompAddBtn.EnableWindow(FALSE);			//컴포넌트 추가버튼 비활성화
		m_CompRemoveBtn.EnableWindow(FALSE);			//컴포넌트 제거버튼 활성화
		m_EditWidth.EnableWindow(FALSE);				//박스크기 width 컨트롤 활성화
		m_Editheight.EnableWindow(FALSE);			//박스크기 Height 컨트롤활성화
		m_EditPosX.EnableWindow(FALSE);				//오프셋 X 에딧컨트롤 활성화
		m_EditPosY.EnableWindow(FALSE);				//오프셋  Y 에딧컨트롤 활성화
		m_SetColliderBtn.EnableWindow(FALSE);		//콜라이더 지정 버튼 버튼 활성화
	}

}



void CBoxColliderPage::OnCbnSelchangeCombo1()
{
}


BOOL CBoxColliderPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_CollideList.AddString(L"사각형");
	m_CollideList.AddString(L"오른쪽위삼각형");			// 1
	m_CollideList.AddString(L"오른쪽아래삼각형");			// 2
	m_CollideList.AddString(L"왼쪽위삼각형");				// 3
	m_CollideList.AddString(L"왼쪽아래삼각형");			// 4

	return TRUE;
}


