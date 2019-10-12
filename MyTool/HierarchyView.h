#pragma once
#include "afxcmn.h"


// CHierarchyView 대화 상자입니다.
class CGameObject;
class CHierarchyView : public CDialogEx
{
	DECLARE_DYNAMIC(CHierarchyView)

public:
	CHierarchyView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHierarchyView();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERARCHY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
