#pragma once

#include"HierarchyView.h"
#include "afxwin.h"

#include "ComponentSheet.h"

// CInspectView �� ���Դϴ�.

class CGameObject;
class CInspectView : public CFormView
{
	DECLARE_DYNCREATE(CInspectView)

protected:
	CInspectView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CInspectView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSPECTOR };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHierarchy();

public:
	//Ŭ���� ������Ʈ ����.
	void SetObject(CGameObject* object);
public:
	//���� �ڽ��� ���� ���� ������Ʈ ���ؼ�.
	void UpdateInfo();
	//������Ʈ ���� �����ڽ��� �ִ� �������� ����
	void UpdateObject();

public:
	CHierarchyView		m_HierarchyView;
	virtual void OnInitialUpdate();
	// �ν����� ������Ʈ �̸�
	CEdit m_ObjectName;
	// �ν����� ������ x,y,z ��
	float m_PosX, m_PosY, m_PosZ;
	// �ν����� �����̼� x,y,z��
	float m_RotX, m_RotY, m_RotZ;
	// �ν����� ������ x,y,z ��
	float m_ScaleX, m_ScaleY, m_ScaleZ;

private:
	CGameObject*		m_ClickedObject;

	//������Ʈ ������Ƽ ��Ʈ
	CComponentSheet*	m_pComponentSheet;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// ������Ʈ �±�
	CString m_ObjectTag;
	// ������Ʈ ���̾�
	CComboBox m_ObjectLayer;
	// //����� ���
	CButton m_Debug;
	afx_msg void OnBnClickedCheck1();
};


