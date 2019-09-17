
// MyToolView.h : CMyToolView Ŭ������ �������̽�
//

#pragma once

class CMyToolDoc;
class CMyToolView : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CMyToolView();
	DECLARE_DYNCREATE(CMyToolView)

// Ư���Դϴ�.
public:
	CMyToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	CDeviceMgr *	m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);


// �����Դϴ�.
public:
	virtual ~CMyToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MyToolView.cpp�� ����� ����
inline CMyToolDoc* CMyToolView::GetDocument() const
   { return reinterpret_cast<CMyToolDoc*>(m_pDocument); }
#endif

