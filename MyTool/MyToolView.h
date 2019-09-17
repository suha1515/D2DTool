
// MyToolView.h : CMyToolView 클래스의 인터페이스
//

#pragma once

class CMyToolDoc;
class CMyToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CMyToolView();
	DECLARE_DYNCREATE(CMyToolView)

// 특성입니다.
public:
	CMyToolDoc* GetDocument() const;

// 작업입니다.
public:
	CDeviceMgr *	m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


// 구현입니다.
public:
	virtual ~CMyToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MyToolView.cpp의 디버그 버전
inline CMyToolDoc* CMyToolView::GetDocument() const
   { return reinterpret_cast<CMyToolDoc*>(m_pDocument); }
#endif

