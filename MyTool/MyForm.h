#pragma once

//유닛툴 다이얼로그
#include "UnitTool.h"
//맵 로드다이얼로그
#include "MapSaveTool.h"

#include "MapTool.h"
#include "UnitTool.h"


#include "afxwin.h"
#include "afxcmn.h"


// CMyForm 폼 뷰입니다.
class CCamera;
class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	//다이얼로그
	CUnitTool	m_UnitTool;
	//탭에들어갈 다이얼로그
	CMapTool  m_MapTool;

	virtual void OnDraw(CDC* /*pDC*/);

public:
	CMapTool* GetMapTool();
private:

	map<CString, MAP_INFO*> m_mapTileSetData;
public:

	// //툴탭
	CTabCtrl m_ToolList;
	afx_msg void OnTcnSelchangeToollist(NMHDR *pNMHDR, LRESULT *pResult);
};


