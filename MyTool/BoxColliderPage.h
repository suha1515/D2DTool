#pragma once
#include "afxwin.h"


// CBoxColliderPage ��ȭ �����Դϴ�.
class CGameObject;
class CBoxColliderPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBoxColliderPage)

public:
	CBoxColliderPage();
	virtual ~CBoxColliderPage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOXCOLLIDERPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// ������Ʈ ��/Ȱ��ȭ ��ư
	CButton m_CompSwitch;
	// ������Ʈ �߰� ��ư
	CButton m_CompAddBtn;
	// ������Ʈ ���� ��ư
	CButton m_CompRemoveBtn;
	// �ڽ�ũ�� width ��Ʈ��
	CEdit m_EditWidth;
	// �ڽ�ũ�� Height ��Ʈ��
	CEdit m_Editheight;
	// ������ X ������Ʈ��
	CEdit m_EditPosX;
	// ������  Y ������Ʈ��
	CEdit m_EditPosY;
	// �ݶ��̴� ���� ��ư
	CButton m_SetColliderBtn;
	afx_msg void OnBnClickedAddComp();
	afx_msg void OnBnClickedRemoveComp();
	afx_msg void OnBnClickedSetCollide();
	afx_msg void OnBnClickedCompCheck();

public:
	void SetObject(CGameObject* pObject);
	void Update();

private:
	CGameObject*		m_pObject;
public:
	// �ڽ� ����
	float m_width;
	// �ڽ� ����
	float m_height;
	// �ڽ� ��ġX
	float m_PosX;
	// �ڽ� ��ġY
	float m_PosY;
};
