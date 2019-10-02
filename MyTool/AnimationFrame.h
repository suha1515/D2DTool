#pragma once
#include "afxwin.h"


// CAnimationFrame ��ȭ �����Դϴ�.
class CGameObject;
class CAnimationFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationFrame)

public:
	CAnimationFrame(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimationFrame();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMFRAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// //�ִϸ��̼� Ŭ�� ����Ʈ
	CListBox m_ClipList;
	// �ִϸ��̼� ��ġ
	float m_PositionX;
	float m_PositionY;
	float m_PositionZ;

	// //�ִϸ��̼� ȸ��
	float m_RotationX;
	float m_RotationY;
	float m_RotationZ;
	// //�ִϸ��̼� ũ��
	float m_ScaleX;
	float m_ScaleY;
	float m_ScaleZ;
	// //�ִϸ��̼� ��� �ð�
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

	//�̹��� ǥ���� ���� ������
	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;
	const TEX_INFO*			m_texInfo;
	CString					m_TexName;

	//�ִϸ��̼� ���
	D3DXMATRIX				m_TransMat;
	D3DXMATRIX				m_Rotmat;
	D3DXMATRIX				m_ScaleMat;

	XMFLOAT2    m_Tex[4];
	Vertex		m_Vertex[4];
	CCamera*	m_Cam;
	///////////////////////////

	//Ŭ�� ������ ���� ��
	map<CString, ANIM_CLIP>		m_Clips;
	XMFLOAT2					m_Size;

	CGameObject*		m_ClickedObject;

	map<CString, ANIM_CLIP>::iterator m_StartIndex;
	map<CString, ANIM_CLIP>::iterator m_EndIndex;

	//��������� �� ����
	bool				m_IsPlaying;
	bool				m_ReWind;
	//���
	float				m_time;
	float				m_EndTime;

public:
	// Ŭ���̸�
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
