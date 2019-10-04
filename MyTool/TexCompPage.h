#pragma once
#include "afxwin.h"


// CTexCompPage ��ȭ �����Դϴ�.
class CGameObject;
class CTexCompPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTexCompPage)

public:
	CTexCompPage();
	virtual ~CTexCompPage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXCOMPPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.


public:
	void SetObject(CGameObject* pObject);
	void Update();

private:
	CGameObject*		m_pObject;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnBnClickedChangeTex();

	// �ؽ��� ����� ���� ���� X ũ��.
	int m_sizeX;
	int m_sizeY;
	//�ؽ�ó �ٲٱ� ��ư
	CButton m_ChangeBtn;

	// �ؽ�ó ũ�� ���� ��Ʈ��
	CEdit m_sizeXEdit;
	CEdit m_sizeYEdit;
	// ������Ʈ ��/Ȱ��ȭ
	CButton m_CompCheckBox;
	afx_msg void OnBnClickedCheckcomponent();

	CButton m_ComponentAdd;
	CButton m_ComponentRemove;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddComp();
	afx_msg void OnBnClickedRemoveComp();
};
