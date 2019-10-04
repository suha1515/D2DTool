// ScriptPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "ScriptPage.h"
#include "afxdialogex.h"


// CScriptPage 대화 상자입니다.
#include "Scripts.h"
#include "GameObject.h"

IMPLEMENT_DYNAMIC(CScriptPage, CPropertyPage)

CScriptPage::CScriptPage()
	: CPropertyPage(IDD_SCRIPTPAGE)
{

}

CScriptPage::~CScriptPage()
{
}

void CScriptPage::SetObject(CGameObject * pObject)
{
	m_pObject = pObject;
}

void CScriptPage::Update()
{
	//선택된 오브젝트가 Null이 아닐경우
	if (nullptr != m_pObject)
	{
		//스크립트 컴포넌트가 있는지 확인.
		CScripts *pComponent = m_pObject->GetComponent<CScripts>();
		if (nullptr != pComponent)
		{
			//컴포넌트가 활성화 되있는지 확인

		}
	}

}

void CScriptPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CompSwitch);
	DDX_Control(pDX, IDC_BUTTON1, m_CompAdd);
	DDX_Control(pDX, IDC_BUTTON2, m_CompRemove);
	DDX_Control(pDX, IDC_COMBO1, m_ScriptList);
	DDX_Control(pDX, IDC_BUTTON3, m_SetScriptBtn);
}


BEGIN_MESSAGE_MAP(CScriptPage, CPropertyPage)
END_MESSAGE_MAP()


// CScriptPage 메시지 처리기입니다.
