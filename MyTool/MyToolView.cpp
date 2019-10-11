
// MyToolView.cpp : CMyToolView 클래스의 구현
//

#include "stdafx.h"
#include "MyToolView.h"

// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MyTool.h"
#endif

#include "MyToolDoc.h"
#include "MainFrm.h"
#include "MiniView.h"
#include "MyForm.h"
#include "InspectView.h"

#include "GameObject.h"
#include "Camera.h"

//텍스쳐 컴포넌트
#include "TextureRenderer.h"
//트랜스폼 컴포넌트
#include "Transform.h"
#include "BoxCollider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyToolView

IMPLEMENT_DYNCREATE(CMyToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMyToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32771, &CMyToolView::OnObjectPopUp)
	ON_COMMAND(ID_32774, &CMyToolView::OnPlay)
	ON_COMMAND(ID_32775, &CMyToolView::OnPause)
	ON_COMMAND(ID_32776, &CMyToolView::OnInit)
END_MESSAGE_MAP()

// CMyToolView 생성/소멸
HWND g_hWnd;

CMyToolView::CMyToolView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pCameraMgr(CCameraMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pShaderMgr(CShaderMgr::GetInstance()),
	m_pInstanceMgr(CInstanceMgr::GetInstance()),
	m_Cam(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	g_hWnd = m_hWnd;

	HRESULT hr = 0;

	m_bIsActive = false;
	m_bIsReInit = false;

	m_AnimObject = nullptr;

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	m_pShaderMgr->Initialize();
	m_pInstanceMgr->Initialize();
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

}

CMyToolView::~CMyToolView()
{
	cout << "툴뷰소멸자" << endl;

	m_pObjectMgr->DestroyInstance();
	m_pShaderMgr->DestroyInstance();
	m_pInstanceMgr->DestroyInstance();
}

BOOL CMyToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMyToolView 그리기



void CMyToolView::OnDraw(CDC* /*pDC*/)
{
	CMyToolDoc* pDoc = GetDocument();
	
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (m_Mode == MAP)
	{
		m_Cam->Update();
		m_Cam->SetTransform();

		m_pDeviceMgr->Render_Begin();

		D3DXMATRIX* mat = m_Cam->GetViewProjMatrix();
		m_pDeviceMgr->GetLine()->SetWidth(1.f);
		m_pDeviceMgr->GetLine()->Begin();
		{
			//Horizontal
			//1024*1024
			for (int i = 0; i < 64; ++i)
			{
				m_Line[0] = { (-512.f),  (512.f) - (i*16.f),0.0f };
				m_Line[1] = { (512.f),  (512.f) - (i*16.f),0.0f };
				m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(23, 234, 0));
			}
			for (int i = 0; i < 64; ++i)
			{
				m_Line[0] = { (-512.f) + (i*16.f),  (512.f) ,0.0f };
				m_Line[1] = { (-512.f) + (i*16.f), -(512.f) ,0.0f };
				m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(23, 234, 0));
			}
		}
		m_pDeviceMgr->GetLine()->End();
		//오브젝트들 업데이트
		m_pObjectMgr->Update();
		//오브젝트 렌더.
		m_pObjectMgr->Render();

		m_pDeviceMgr->Render_End(m_hWnd);
	}
	
}


BOOL CMyToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMyToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}

void CMyToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}


// CMyToolView 진단

#ifdef _DEBUG
void CMyToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMyToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyToolDoc* CMyToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyToolDoc)));
	return (CMyToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyToolView 메시지 처리기


void CMyToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CScrollView::SetScrollSizes(MM_TEXT, CSize(300,300));

	m_pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(m_pFrameWnd);

	//폼 가져오기
	m_pMyForm = dynamic_cast<CMyForm*>(m_pFrameWnd->m_SecondSplitter.GetPane(1, 0));
	NULL_CHECK(m_pMyForm);

	m_pInspect = dynamic_cast<CInspectView*>(m_pFrameWnd->m_MainSplitter.GetPane(0, 2));
	NULL_CHECK_MSG(m_pInspect, L"inspectorview nullptr");

	// MainFrame의 크기를 얻어온다.
	// GetWindowRect: 윈도우의 전체 테두리를 포함한 사각형 크기를 얻어오는 함수.
	// 이 때 left, top, right, bottom은 스크린좌표 기준이다.
	RECT rcMain = {};
	m_pFrameWnd->GetWindowRect(&rcMain);
	::SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	// View의 크기를 얻어온다.
	// GetClientRect: 윈도우의 테두리를 포함하지 않은 순수 클라이언트 영역의 사각형 크기를 얻어오는 함수.
	// 이 때 left, top, rigth, bottom은 클라이언트 좌표 기준이다.
	// 즉, left, top은 항상 0, 0이다.
	RECT rcView = {};
	GetClientRect(&rcView);

	// MainFrame과 View의 간격을 계산.
	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	// MainFrame의 윈도우 위치와 크기를 다시 조정.
	m_pFrameWnd->SetWindowPos(
		nullptr, 0, 0, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER);

	RECT rc = {};
	GetClientRect(&rc);
	int winX = rc.right - rc.left;
	int winY = rc.bottom - rc.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_pCameraMgr->SetMainCamera(m_Cam);
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));


	//기본값.
	m_TileSize.x = 16;
	m_TileSize.y = 16;
	m_Mode = MAP;

}

void CMyToolView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	RECT rcView = {};
	GetClientRect(&rcView);

	int winX = rcView.right - rcView.left;
	int winY = rcView.bottom - rcView.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(2.0f, 2.0f, 1.0f));

}

//타일 입력
void CMyToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnLButtonDown(nFlags, point);
	if (m_Mode == MAP)
	{
		CString tileName = m_pMyForm->GetMapTool()->GetTileName();
		const XMFLOAT2* tex = m_pMyForm->GetMapTool()->GetTexPos();
		const XMFLOAT2& size = m_pMyForm->GetMapTool()->GetTileSize();
		if (size.x != 0 && size.y != 0)
		{
			m_TileSize = size;
		}
		const CPoint mousePos = MousePicking(point);
		//auto temp = CObjectMgr::GetInstance()->GetObjects();
		if (tex != nullptr)
		{
			wstring name = L"GameObject" + to_wstring(m_pObjectMgr->GetObjectCount());

			CGameObject* pGameObject = new CGameObject;
			pGameObject->Initialize();
			pGameObject->SetObjectName(name);
			//인스턴스 일경우.
			if (m_pMyForm->m_MapTool.m_Instacne.GetCheck() == 1)
			{
				pGameObject->SetObjectTag(L"Instance");
			}
			//레이어지정
			if (m_pMyForm->m_MapTool.m_Layer.GetCurSel() != -1)
				pGameObject->SetObjectLayer((Layer)m_pMyForm->m_MapTool.m_Layer.GetCurSel());

			//트랜스폼 컴포넌트
			CTransform* pTransform = new CTransform;
			pTransform->Initialize(pGameObject);
			D3DXVECTOR3 pos = D3DXVECTOR3((float)mousePos.x, (float)mousePos.y, 0.0f);
			pTransform->SetPosition(pos);

			pGameObject->AddComponent(pTransform);

			// 렌더 컴포넌트 넣기.
			CTextureRenderer* pRender = new CTextureRenderer;
			pRender->Initialize(pGameObject);
			pRender->SetTexture((LPCTSTR)tileName);
			pRender->SetVertex(size, tex);

			pGameObject->AddComponent(pRender);

			//충돌하는경우.
			if (m_pMyForm->m_MapTool.m_Collide.GetCheck() == 1)
			{
				CBoxCollider*	pBoxCollider = new CBoxCollider;
				pBoxCollider->Initialize(pGameObject);
				pBoxCollider->SetCollideType(NORMAL);
				pGameObject->AddComponent(pBoxCollider);
			}

			//0을 반환한 경우는 부모로생성하는것
			m_pInspect->m_HierarchyView.AddObject(pGameObject);
			m_pObjectMgr->AddObject(pGameObject);
			Invalidate(FALSE);
		}
	}

	
}
//타일 지우기
void CMyToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	cout << "aw" << endl;
}


void CMyToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);
	
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_LBUTTON))
	{

	}
		
	m_MousePoint = point;
}




void CMyToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
	if (m_Mode == MAP)
	{
		if (nChar == VK_LEFT)
		{
			m_Cam->MoveCamera(-5.0f, 0.0f);
		}
		else if (nChar == VK_UP)
		{
			m_Cam->MoveCamera(0.0f, 5.0f);
		}
		else if (nChar == VK_DOWN)
		{
			m_Cam->MoveCamera(0.0f, -5.0f);
		}
		else if (nChar == VK_RIGHT)
		{
			m_Cam->MoveCamera(5.0f, 0.0f);
		}
		Invalidate(FALSE);
	}
}


BOOL CMyToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	float scale = 0.0f;
	if (zDelta <= 0)
	{
		scale = -0.1f;
	}
	else
	{
		scale = 0.1f;
	}
	m_Cam->Scaling(scale, scale);
	Invalidate(FALSE);
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
	
}

const CPoint CMyToolView::MousePicking(const CPoint& point)
{
	if (m_Mode == MAP)
	{
		RECT rc = {};
		GetClientRect(&rc);
		float winX = float(rc.right - rc.left);
		float winY = float(rc.bottom - rc.top);

		D3DXVECTOR3 vMouse = { float(point.x) - winX*0.5f,(winY*0.5f - float(point.y)),1.0f };
		D3DXVECTOR4 vMouse2;
		D3DXMATRIX viewMat = *m_Cam->GetViewMatrix();
		D3DXMATRIX InvViewMat;
		D3DXMatrixInverse(&InvViewMat, NULL, &viewMat);
		D3DXVec3Transform(&vMouse2, &vMouse, &InvViewMat);

		cout << vMouse.x << " , " << vMouse.y << endl;
		float newX, newY;
		if (vMouse2.x>0)
			newX = ((int)vMouse2.x / (int)m_TileSize.x)*m_TileSize.x + m_TileSize.x*0.5f;
		else
			newX = ((int)vMouse2.x / (int)m_TileSize.x)*m_TileSize.x - m_TileSize.x*0.5f;

		if (vMouse2.y>0)
			newY = ((int)vMouse2.y / (int)m_TileSize.y)*m_TileSize.y + m_TileSize.y*0.5f;
		else
			newY = ((int)vMouse2.y / (int)m_TileSize.y)*m_TileSize.y - m_TileSize.y*0.5f;

		cout << newX << " , " << newY << endl;

		return CPoint((int)newX, (int)newY);
	}
	return CPoint(0, 0);
}

void CMyToolView::Update()
{
	//카메라 업데이트
	m_Cam->Update();
	m_Cam->SetTransform();
	if (m_Mode == ANIM)
	{
		if (m_AnimObject != nullptr)
			m_AnimObject->Update();
	}
	else
	m_pObjectMgr->Update();
}

void CMyToolView::Render()
{
	m_pDeviceMgr->Render_Begin();
	if (m_Mode == ANIM)
	{
		if (m_AnimObject != nullptr)
			m_AnimObject->Render();
	}
	else
	m_pObjectMgr->Render();

	CFrameMgr::GetInstance()->RenderFPS();
	m_pDeviceMgr->Render_End(m_hWnd);
}

bool CMyToolView::GetIsPlaying()
{
	return m_bIsActive;
}

void CMyToolView::SetMode(MODE mode)
{
	m_Mode = mode;
}

//오른쪽 클릭시 나오는것 (컨텍스트 메뉴)
void CMyToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CMenu popup;
	CMenu* pMenu;
	popup.LoadMenuW(IDR_MENU3);
	
	if(m_Mode ==MAP )
	{
	pMenu = popup.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, point.x, point.y,this);
	}
	else if (m_Mode == UNIT)
	{
		pMenu = popup.GetSubMenu(2);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	
}


//오브젝트 추가
void CMyToolView::OnObjectPopUp()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPoint mousePos = MousePicking(m_MousePoint);

	wstring name = L"GameObject" + to_wstring(m_pObjectMgr->GetObjectCount());

	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	pGameObject->SetObjectName(name);

	//트랜스폼 컴포넌트
	CTransform* pTransform = new CTransform;
	pTransform->Initialize(pGameObject);
	D3DXVECTOR3 objPos = D3DXVECTOR3((float)mousePos.x, (float)mousePos.y, 0.0f);
	pTransform->SetPosition(objPos);

	pGameObject->AddComponent(pTransform);

	m_pInspect->m_HierarchyView.AddObject(pGameObject);
	m_pObjectMgr->AddObject(pGameObject);


}

void CMyToolView::OnPlay()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bIsActive = true;
}

void CMyToolView::OnPause()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bIsActive = false;
	Invalidate(FALSE);
}

void CMyToolView::OnInit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
