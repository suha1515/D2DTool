// ScriptPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "ScriptPage.h"
#include "afxdialogex.h"


// CScriptPage ��ȭ �����Դϴ�.
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
	//���õ� ������Ʈ�� Null�� �ƴҰ��
	if (nullptr != m_pObject)
	{
		//��ũ��Ʈ ������Ʈ�� �ִ��� Ȯ��.
		CScripts *pComponent = m_pObject->GetComponent<CScripts>();
		if (nullptr != pComponent)
		{
			//������Ʈ�� Ȱ��ȭ ���ִ��� Ȯ��

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


// CScriptPage �޽��� ó�����Դϴ�.
