
// MyToolView.h : CMyToolView 클래스의 인터페이스
//

#pragma once
//프레임
class CMainFrame;
//뷰
class CMyForm;
class CInspectView;


static enum MODE { MAP, ANIM };

class CCamera;
class CMyToolDoc;
class CGameObject;
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
	CDeviceMgr *	m_pDeviceMgr;		//디바이스 매니저
	CTextureMgr*	m_pTextureMgr;		//텍스처   매니저
	CCameraMgr*		m_pCameraMgr;		//카메라   매니저
	CObjectMgr*		m_pObjectMgr;		//오브젝트 매니저
	CShaderMgr*		m_pShaderMgr;		//쉐이더   매니저


	D3DXVECTOR3		m_Line[2];

	CCamera*		m_Cam;

	CString			m_MapName;

	CPoint			m_MousePoint;		//마우스 위치

	CMainFrame*		m_pFrameWnd;
	CMyForm*		m_pMyForm;
	CInspectView*	m_pInspect;

	//임시 오브젝트 표현 변수.
	vector<CGameObject*> m_GameObject;

	//타일피킹
	XMFLOAT2			 m_TileSize;

	//툴 모드
	MODE				 m_Mode;

	//게임 실행, 정지 ,초기화
	bool				 m_bIsActive;
	bool				 m_bIsReInit;
public:
	const CPoint& MousePicking(const CPoint& point);
public:
	//업데이트
	void Update();
	//렌더
	void Render();
	//게임 재생상태
public:
	bool GetIsPlaying();

public:
	//모드 변경
	void		  SetMode(MODE mode);

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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnObjectPopUp();
	afx_msg void OnPlay();
	afx_msg void OnPause();
	afx_msg void OnInit();
};

#ifndef _DEBUG  // MyToolView.cpp의 디버그 버전
inline CMyToolDoc* CMyToolView::GetDocument() const
   { return reinterpret_cast<CMyToolDoc*>(m_pDocument); }
#endif

