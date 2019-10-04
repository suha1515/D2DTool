#pragma once
#include "afxwin.h"


// CScriptPage ��ȭ �����Դϴ�.
class CGameObject;
class CScriptPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CScriptPage)

public:
	CScriptPage();
	virtual ~CScriptPage();

public:
	void SetObject(CGameObject* pObject);
	void Update();
private:
	CGameObject *m_pObject;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRIPTPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// ������Ʈ On/Off
	CButton m_CompSwitch;
	// ������Ʈ �߰�
	CButton m_CompAdd;
	// ������Ʈ ����
	CButton m_CompRemove;
	// ��ũ��Ʈ ����Ʈ.
	CComboBox m_ScriptList;
	// ��ũ��Ʈ ���� ����
	CButton m_SetScriptBtn;
};
