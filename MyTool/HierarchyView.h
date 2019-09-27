#pragma once
#include "afxcmn.h"


// CHierarchyView ��ȭ �����Դϴ�.
class CGameObject;
class CHierarchyView : public CDialogEx
{
	DECLARE_DYNAMIC(CHierarchyView)

public:
	CHierarchyView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHierarchyView();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERARCHY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void AddObject(CGameObject* object);

public:
	CTreeCtrl m_Hierarchy;
	virtual BOOL OnInitDialog();

	list<std::pair<CGameObject*, HTREEITEM>> m_objectlist;

	afx_msg void OnNMClickHierarchytree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedHierarchytree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkHierarchytree(NMHDR *pNMHDR, LRESULT *pResult);
};
