#pragma once

//유닛툴 다이얼로그
#include "UnitTool.h"
#include "afxwin.h"


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
	afx_msg void OnBnClickedUnitTool();
	virtual void OnInitialUpdate();

public:
	CUnitTool	m_UnitTool;
	virtual void OnDraw(CDC* /*pDC*/);

public:
	void Renew(XMFLOAT2 * tex);
	void VertexUpdate();
	const XMFLOAT2* GetTexPos();
	const CString&  GetTileName();
private:
	CDeviceMgr*	 m_pDeviceMgr;
	CTextureMgr* m_pTextureMgr;
	
	CString		m_str;

	//타일이미지를 출력하기위한 변수들 ----------
	XMFLOAT2    m_Tex[4];
	Vertex		m_Vertex[4];
	CCamera*	m_Cam;

	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;
	const TEX_INFO*			m_texInfo;
	CString					m_TileName;
	//-------------------------------------------

	map<CString, MAP_INFO*> m_mapTileSetData;
public:
	CStatic m_TexValue;
	//타일셋 리스트
	CListBox m_TileSetList;
	//타일맵 이름
	CEdit m_TileNameEdit;

	afx_msg void OnBnClickedFileOpen();
	afx_msg void OnStnClickedTexValue();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedDeleteMap();
	afx_msg void OnLbnSelchangeTileClick();
	afx_msg void OnLbnDblclkTileList();
	afx_msg void OnBnClickedLoadMapDB();
	afx_msg void OnBnClickedSaveMapDB();
	afx_msg void OnDropFiles(HDROP hDropInfo);

};


