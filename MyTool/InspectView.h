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
	afx_msg void OnBnClickedChangeValue();
	int m_PosX;
	int m_PosY;
	int m_PosZ;
	int m_RotX;
	int m_RotY;
	int m_RotZ;

	float m_ScaleX;
	float m_ScaleY;
	float m_ScaleZ;
};


