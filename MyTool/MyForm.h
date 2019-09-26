#pragma once

//������ ���̾�α�
#include "UnitTool.h"
//�� �ε���̾�α�
#include "MapSaveTool.h"

#include "MapTool.h"
#include "UnitTool.h"


#include "afxwin.h"
#include "afxcmn.h"


// CMyForm �� ���Դϴ�.
class CCamera;
class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	//���̾�α�
	CUnitTool	m_UnitTool;
	//�ǿ��� ���̾�α�
	CMapTool  m_MapTool;

	virtual void OnDraw(CDC* /*pDC*/);

public:
	CMapTool* GetMapTool();
private:

	map<CString, MAP_INFO*> m_mapTileSetData;
public:

	// //����
	CTabCtrl m_ToolList;
	afx_msg void OnTcnSelchangeToollist(NMHDR *pNMHDR, LRESULT *pResult);
};


