
// MyToolView.h : CMyToolView Ŭ������ �������̽�
//

#pragma once

class CCamera;
class CMyToolDoc;
class CGameObject;
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
	CDeviceMgr *	m_pDeviceMgr;		//����̽� �Ŵ���
	CTextureMgr*	m_pTextureMgr;		//�ؽ�ó   �Ŵ���
	CCameraMgr*		m_pCameraMgr;		//ī�޶�   �Ŵ���
	CObjectMgr*		m_pObjectMgr;		//������Ʈ �Ŵ���
	CShaderMgr*		m_pShaderMgr;		//���̴�   �Ŵ���


	D3DXVECTOR3		m_Line[2];

	CCamera*		m_Cam;

	CString			m_MapName;

	//�ӽ� ������Ʈ ǥ�� ����.
	vector<CGameObject*> m_GameObject;
public:
	const CPoint& MousePicking(const CPoint& point);

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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
};

#ifndef _DEBUG  // MyToolView.cpp�� ����� ����
inline CMyToolDoc* CMyToolView::GetDocument() const
   { return reinterpret_cast<CMyToolDoc*>(m_pDocument); }
#endif

