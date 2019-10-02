#pragma once
#include "afxwin.h"


// CAnimationFrame 대화 상자입니다.
class CGameObject;
class CAnimationFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationFrame)

public:
	CAnimationFrame(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationFrame();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMFRAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// //애니메이션 클립 리스트
	CListBox m_ClipList;
	// 애니메이션 위치
	float m_PositionX;
	float m_PositionY;
	float m_PositionZ;

	// //애니메이션 회전
	float m_RotationX;
	float m_RotationY;
	float m_RotationZ;
	// //애니메이션 크기
	float m_ScaleX;
	float m_ScaleY;
	float m_ScaleZ;
	// //애니메이션 재생 시간
	float m_PlayTime;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);


public:
	void				SetTexture(const CString&clipName, const XMFLOAT2* tex, const XMFLOAT2* size);
	void				Initialize();
	void				Update();
	void				Render();
	void				VertexUpdate();

	//SetAnim
	void				LoadAnimation(const CString& clipName, const vector<ANIM_CLIP>& clips);
public:
	bool				IsPlaying();
private:
	CDeviceMgr*			m_pDeviceMgr;
	CTextureMgr*		m_pTextureMgr;

	//이미지 표현을 위한 변수들
	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;
	const TEX_INFO*			m_texInfo;
	CString					m_TexName;

	//애니메이션 행렬
	D3DXMATRIX				m_TransMat;
	D3DXMATRIX				m_Rotmat;
	D3DXMATRIX				m_ScaleMat;

	XMFLOAT2    m_Tex[4];
	Vertex		m_Vertex[4];
	CCamera*	m_Cam;
	///////////////////////////

	//클립 저장을 위한 맵
	map<CString, ANIM_CLIP>		m_Clips;
	XMFLOAT2					m_Size;

	CGameObject*		m_ClickedObject;

	map<CString, ANIM_CLIP>::iterator m_StartIndex;
	map<CString, ANIM_CLIP>::iterator m_EndIndex;

	//재생을위한 불 변수
	bool				m_IsPlaying;
	bool				m_ReWind;
	//재생
	float				m_time;
	float				m_EndTime;

public:
	// 클립이름
	CString m_ClipName;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedClipAdd();
	afx_msg void OnBnClickedClipRemove();
	afx_msg void OnBnClickedAnimationSave();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedReWind();
	afx_msg void OnBnClickedPause();
	afx_msg void OnLbnDblclkCliplist();
	CString m_TextureName;
	float m_TextureSizeX;
	float m_TextureSizeY;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_CheckLoop;
};
