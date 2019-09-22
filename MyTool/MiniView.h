#pragma once


// CMiniView 뷰입니다.
class CCamera;
class CMiniView : public CScrollView
{
	DECLARE_DYNCREATE(CMiniView)

protected:
	CMiniView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMiniView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	void		Initialize();
public:
	CDeviceMgr*	 m_pDeviceMgr;
	CTextureMgr* m_pTextureMgr;

	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;
	Vertex				    m_Vertex[4];

	CCamera*				m_Cam;
	const TEX_INFO*				m_texInfo;

private:
	
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


