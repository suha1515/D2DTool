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
	int AddObject(CGameObject* object);
	int AddCopyObject(const OBJ_COPY* copy, const wstring& name);
	void AddList(HTREEITEM * item, const OBJ_COPY * copy, const wstring& name, CGameObject* parent);
public:
	void LoadObject();
	void Clear();
	void Update();

public:
	CTreeCtrl m_Hierarchy;
	virtual BOOL OnInitDialog();
	void RecursivelyDelete(HTREEITEM childItem);
	//list<std::pair<CGameObject*, HTREEITEM>> m_objectlist;

	map<HTREEITEM, CGameObject*>	m_objectlist;

	CGameObject*					m_CurClicked;
	CGameObject*					m_PreClicked;

	afx_msg void OnNMClickHierarchytree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedHierarchytree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkHierarchytree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDeselectAll();
	afx_msg void OnBnClickedAddObject();
	afx_msg void OnBnClickedDeleteObject();
};
