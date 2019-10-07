// ScriptPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "ScriptPage.h"
#include "afxdialogex.h"


// CScriptPage ��ȭ �����Դϴ�.
#include "Scripts.h"
#include "GameObject.h"

//��ũ��Ʈ��
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
	//���õ� ������Ʈ�� Null�� �ƴҰ��
	if (nullptr != m_pObject)
	{
		//��ũ��Ʈ ������Ʈ�� �ִ��� Ȯ��.
		const map<string,CScripts*>& sizeScript = m_pObject->GetScripts();
		
		if (sizeScript.size()>0)
		{
			m_CompRemove.EnableWindow(TRUE);	//������Ʈ ���Ź�ư Ȱ��ȭ
			m_ScriptList.EnableWindow(TRUE);	//��ũ��Ʈ �޺��ڽ� Ȱ��ȭ
			
			for (auto&i : sizeScript)
			{
				CString name;
				name = i.first.c_str();
				m_list.AddString(name);
			}
		}
		//������Ʈ�� ���°��
		else
		{
			m_CompAdd.EnableWindow(TRUE);			//������Ʈ �߰���ư Ȱ��ȭ
			m_CompRemove.EnableWindow(FALSE);		//������Ʈ ���Ź�ư ��Ȱ��ȭ

			m_ScriptList.EnableWindow(TRUE);		//��ũ��Ʈ �޺��ڽ�  Ȱ��ȭ
		}
	}
	//���õ� ������Ʈ�� ���� ���
	else//��� ��Ȱ��ȭ.
	{
		m_CompSwitch.EnableWindow(FALSE);			//������Ʈ Ȱ��ȭ ��ư ��Ȱ��ȭ
		m_CompAdd.EnableWindow(FALSE);				//������Ʈ �߰���ư ��Ȱ��ȭ
		m_CompRemove.EnableWindow(FALSE);			//������Ʈ ���Ź�ư ��Ȱ��ȭ

		m_ScriptList.EnableWindow(FALSE);			//��ũ��Ʈ �޺��ڽ� ��Ȱ��ȭ
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


// CScriptPage �޽��� ó�����Դϴ�.



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
			FAILED_CHECK_MSG_RETURN(hr, L"��ũ��Ʈ �ֱ� ����");
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

	m_ScriptList.AddString(L"CPlayerScript");			//�÷��̾� ��ũ��Ʈ.

	return TRUE;  
}

