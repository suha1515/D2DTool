#pragma once
#include "afxwin.h"

//맵툴
#include "MapSaveTool.h"

// CMapTool 대화 상자입니다.

class CMapTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void Renew(XMFLOAT2 * tex);
	void VertexUpdate();
	const XMFLOAT2* GetTexPos();
	const CString&  GetTileName();
	const XMFLOAT2&	GetTileSize();

	//다이얼로그 기반 Ondraw
	void OnDraw(CDC* pDC);

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
	//인스턴스 객체 생성
	bool					m_bIsInstance;

	//맵툴 다이얼로그
	CMapSaveTool				m_SaveTool;

	map<CString, MAP_INFO*> m_mapTileSetData;
public:
	afx_msg void OnBnClickedTileSetOpen();
	afx_msg void OnBnClickedTileSetRemove();
	afx_msg void OnBnClickedTileListSave();
	afx_msg void OnBnClickedTileListLoad();
	afx_msg void OnBnClickedMapLoad();
	afx_msg void OnPaint();
	afx_msg void OnLbnDblclkTileList();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnStnClickedTilesetsize();


	// //타일셋 이름
	CEdit m_TileSetName;
	// //타일셋 리스트
	CListBox m_TileSetList;
	virtual BOOL OnInitDialog();
	// 타일셋 사이즈이다. 
	CString m_TileSetSize;
	int m_iTileSizeX;
	int m_iTileSizeY;
	XMFLOAT2	m_fTileSize;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// 인스턴스 체크 박스
	CButton m_Instacne;
	afx_msg void OnBnClickedInstance();
};
