// ScriptPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "ScriptPage.h"
#include "afxdialogex.h"


// CScriptPage 대화 상자입니다.
#include "Scripts.h"
#include "GameObject.h"

//스크립트들
#include "PlayerScript.h"

IMPLEMENT_DYNAMIC(CScriptPage, CPropertyPage)

CScriptPage::CScriptPage()
	: CPropertyPage(IDD_SCRIPTPAGE)
	, m_ScriptName(_T(""))
{

}

CScriptPage::~CScriptPage()
{
}

void CScriptPage::SetObject(CGameObject * pObject)
{
	m_pObject = pObject;
	Update();
}

void CScriptPage::Update()
{
	m_list.ResetContent();
	CString name=L"";
	//선택된 오브젝트가 Null이 아닐경우
	if (nullptr != m_pObject)
	{
		//스크립트 컴포넌트가 있는지 확인.
		const map<string,CScripts*>& sizeScript = m_pObject->GetScripts();
		
		if (sizeScript.size()>0)
		{
			m_CompRemove.EnableWindow(TRUE);	//컴포넌트 제거버튼 활성화
			m_ScriptList.EnableWindow(TRUE);	//스크립트 콤보박스 활성화
			
			for (auto&i : sizeScript)
			{
				CString name;
				name = i.first.c_str();
				m_list.AddString(name);
			}
		}
		//컴포넌트가 없는경우
		else
		{
			m_CompAdd.EnableWindow(TRUE);			//컴포넌트 추가버튼 활성화
			m_CompRemove.EnableWindow(FALSE);		//컴포넌트 제거버튼 비활성화

			m_ScriptList.EnableWindow(TRUE);		//스크립트 콤보박스  활성화
		}
	}
	//선택된 오브젝트가 없는 경우
	else//모두 비활성화.
	{
		m_CompSwitch.EnableWindow(FALSE);			//컴포넌트 활성화 버튼 비활성화
		m_CompAdd.EnableWindow(FALSE);				//컴포넌트 추가버튼 비활성화
		m_CompRemove.EnableWindow(FALSE);			//컴포넌트 제거버튼 비활성화

		m_ScriptList.EnableWindow(FALSE);			//스크립트 콤보박스 비활성화
	}

	UpdateData(TRUE);
	UpdateData(FALSE);

}

void CScriptPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CompSwitch);
	DDX_Control(pDX, IDC_BUTTON1, m_CompAdd);
	DDX_Control(pDX, IDC_BUTTON2, m_CompRemove);
	DDX_Control(pDX, IDC_COMBO1, m_ScriptList);
	DDX_Text(pDX, IDC_Scriptname, m_ScriptName);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CScriptPage, CPropertyPage)

	ON_BN_CLICKED(IDC_BUTTON1, &CScriptPage::OnBnClickedCompAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &CScriptPage::OnBnClickedCompRemove)
	ON_BN_CLICKED(IDC_CHECK1, &CScriptPage::OnBnClickedCheckBox)
END_MESSAGE_MAP()


// CScriptPage 메시지 처리기입니다.



void CScriptPage::OnBnClickedCompAdd()
{
	if (m_pObject != nullptr)
	{
		int index = m_ScriptList.GetCurSel();
		if (index != -1)
		{
			CString scriptName;
			m_ScriptList.GetLBText(index,scriptName);

			HRESULT hr = 0;
			hr = CScriptMgr::GetInstance()->LoadScripts(scriptName.operator LPCWSTR(), m_pObject);
			FAILED_CHECK_MSG_RETURN(hr, L"스크립트 넣기 실패");
			Update();	
		}
	}
}


void CScriptPage::OnBnClickedCompRemove()
{
	if (m_pObject != nullptr)
	{
		int index = m_list.GetCurSel();
		if (index != -1)
		{
			CString name;
			m_list.GetText(index, name);
			CT2CA pszConvertedAnsiString(name);
			string scriptName(pszConvertedAnsiString);
			m_pObject->RemoveScript(scriptName);
			Update();
		}
	}
}


void CScriptPage::OnBnClickedCheckBox()
{

}
BOOL CScriptPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_ScriptList.AddString(L"CPlayerScript");			//플레이어 스크립트.

	return TRUE;  
}

