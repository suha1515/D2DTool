#pragma once

#include "TexCompPage.h"
#include "BoxColliderPage.h"
#include "AnimatorPage.h"
#include "ScriptPage.h"
#include "BoxCollider.h"

// CComponentSheet
class CGameObject;
class CComponentSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CComponentSheet)

public:
	CComponentSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CComponentSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CComponentSheet(CWnd* pParentWnd = NULL);
	virtual ~CComponentSheet();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CTexCompPage			m_TexCompPage;
	CBoxColliderPage		m_BoxCompPage;
	CAnimatorPage			m_AnimatorCompPage;
	CScriptPage				m_ScriptCompPage;
	
public:
	afx_msg void OnRemove();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
	void UpdateInfo(CGameObject* pObject);
	void RemoveAll();
};


