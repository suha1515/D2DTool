#pragma once
#include "afxwin.h"


// CMapSaveTool 대화 상자입니다.
class CGameObject;
class CMapSaveTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMapSaveTool)

public:
	CMapSaveTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapSaveTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAVETOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMapSave();
	afx_msg void OnBnClickedMapLoad();

	//맵 정보를 담을 컨테이너
private:
	map<CString, vector<MAP_TILE>> m_map;

public:
	afx_msg void OnLbnDblclkMaplist();
	CListBox m_MapList;
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton8();
};
