#pragma once
#include "afxwin.h"


// CMapSaveTool ��ȭ �����Դϴ�.
class CGameObject;
class CMapSaveTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMapSaveTool)

public:
	CMapSaveTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapSaveTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAVETOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMapSave();
	afx_msg void OnBnClickedMapLoad();

	//�� ������ ���� �����̳�
private:
	map<CString, vector<MAP_TILE>> m_map;

public:
	afx_msg void OnLbnDblclkMaplist();
	CListBox m_MapList;
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton8();
};
