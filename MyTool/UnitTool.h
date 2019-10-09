#pragma once
#include "afxwin.h"


// CUnitTool ��ȭ �����Դϴ�.

class CUnitTool : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUnitTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// ���� �� �˻���
	CString m_Search;
	// ���̶�Ű�� ������ �̸�
	CString m_name;
	afx_msg void OnBnClickedAddObject();

	OBJ_COPY MakeCopy(CGameObject* pObject);
	void	 WriteCopy(CFile* pFile,OBJ_COPY* copy);

public:
	//�߰��� ������ ���� �����̳�.
	map<wstring,OBJ_COPY>		m_Units;
	// //���� ������Ʈ
	CListBox m_ObjectList;
	afx_msg void OnBnClickedSaveList();
	afx_msg void OnBnClickedLoadObject();
	// ī�ǵ� ������Ʈ �̸�
	CString m_CopyName;
	afx_msg void OnBnClickedToHierarchy();
	afx_msg void OnBnClickedRemove();
};
