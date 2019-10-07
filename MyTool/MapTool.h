#pragma once
#include "afxwin.h"

//����
#include "MapSaveTool.h"

// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	void Renew(XMFLOAT2 * tex);
	void VertexUpdate();
	const XMFLOAT2* GetTexPos();
	const CString&  GetTileName();
	const XMFLOAT2&	GetTileSize();

	//���̾�α� ��� Ondraw
	void OnDraw(CDC* pDC);

private:
	CDeviceMgr*	 m_pDeviceMgr;
	CTextureMgr* m_pTextureMgr;

	CString		m_str;

	//Ÿ���̹����� ����ϱ����� ������ ----------
	XMFLOAT2    m_Tex[4];
	Vertex		m_Vertex[4];
	CCamera*	m_Cam;

	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;
	const TEX_INFO*			m_texInfo;
	CString					m_TileName;
	//-------------------------------------------
	//�ν��Ͻ� ��ü ����
	bool					m_bIsInstance;

	//���� ���̾�α�
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


	// //Ÿ�ϼ� �̸�
	CEdit m_TileSetName;
	// //Ÿ�ϼ� ����Ʈ
	CListBox m_TileSetList;
	virtual BOOL OnInitDialog();
	// Ÿ�ϼ� �������̴�. 
	CString m_TileSetSize;
	int m_iTileSizeX;
	int m_iTileSizeY;
	XMFLOAT2	m_fTileSize;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// �ν��Ͻ� üũ �ڽ�
	CButton m_Instacne;
	afx_msg void OnBnClickedInstance();
};
