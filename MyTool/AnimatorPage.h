#pragma once
#include "afxwin.h"


// CAnimatorPage ��ȭ �����Դϴ�.
class CGameObject;
class CAnimatorPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAnimatorPage)

public:
	CAnimatorPage();
	virtual ~CAnimatorPage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATORPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// ������Ʈ ��/Ȱ��ȭ
	CButton m_CompSwitch;
	// �ִϸ��̼� ����Ʈ
	CComboBox m_AnimCombo;
	// Ŭ�� �޺�
	CComboBox m_ClipCombo;

public:
	void SetObject(CGameObject* pObject);
	void Update();
	// ������Ʈ �߰���ư
	CButton m_CompAdd;
	// ������Ʈ ���� ��ư
	CButton m_CompRemove;
	// ��� ��ư
	CButton m_PlayBtn;
	// ���� ��ư
	CButton m_PauseBtn;
	// �ʱ�ȭ ��ư
	CButton m_InitBtn;
	// //�ִϸ��̼� ����Ʈ ����
	CButton m_animListRenew;
private:
	CGameObject *m_pObject;
public:
	afx_msg void OnBnClickedCompOn();
	afx_msg void OnBnClickedRenewList();
	
	afx_msg void OnBnClickedAddComp();
	afx_msg void OnBnClickedRemoveComp();
	afx_msg void OnBnClickedRenewClip();
	afx_msg void OnBnClickedSetAnim();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedReWind();
	// �ݺ� üũ�ڽ�.
	CButton m_LoopCheck;
};
