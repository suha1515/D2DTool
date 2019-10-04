#pragma once
#include "afxwin.h"


// CScriptPage 대화 상자입니다.
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

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRIPTPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 컴포넌트 On/Off
	CButton m_CompSwitch;
	// 컴포넌트 추가
	CButton m_CompAdd;
	// 컴포넌트 제거
	CButton m_CompRemove;
	// 스크립트 리스트.
	CComboBox m_ScriptList;
	// 스크립트 지정 변수
	CButton m_SetScriptBtn;
};
