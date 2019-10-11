
// MyToolView.cpp : CMyToolView Ŭ������ ����
//

#include "stdafx.h"
#include "MyToolView.h"

// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

//�ؽ��� ������Ʈ
#include "TextureRenderer.h"
//Ʈ������ ������Ʈ
#include "Transform.h"
#include "BoxCollider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyToolView

IMPLEMENT_DYNCREATE(CMyToolView, CScrollView)

BEGIN_MESSAGE_MAP(CMyToolView, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
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

// CMyToolView ����/�Ҹ�
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
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	cout << "����Ҹ���" << endl;

	m_pObjectMgr->DestroyInstance();
	m_pShaderMgr->DestroyInstance();
	m_pInstanceMgr->DestroyInstance();
}

BOOL CMyToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMyToolView �׸���



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
		//������Ʈ�� ������Ʈ
		m_pObjectMgr->Update();
		//������Ʈ ����.
		m_pObjectMgr->Render();

		m_pDeviceMgr->Render_End(m_hWnd);
	}
	
}


BOOL CMyToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMyToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}

void CMyToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}


// CMyToolView ����

#ifdef _DEBUG
void CMyToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMyToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyToolDoc* CMyToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyToolDoc)));
	return (CMyToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyToolView �޽��� ó����


void CMyToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CScrollView::SetScrollSizes(MM_TEXT, CSize(300,300));

	m_pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(m_pFrameWnd);

	//�� ��������
	m_pMyForm = dynamic_cast<CMyForm*>(m_pFrameWnd->m_SecondSplitter.GetPane(1, 0));
	NULL_CHECK(m_pMyForm);

	m_pInspect = dynamic_cast<CInspectView*>(m_pFrameWnd->m_MainSplitter.GetPane(0, 2));
	NULL_CHECK_MSG(m_pInspect, L"inspectorview nullptr");

	// MainFrame�� ũ�⸦ ���´�.
	// GetWindowRect: �������� ��ü �׵θ��� ������ �簢�� ũ�⸦ ������ �Լ�.
	// �� �� left, top, right, bottom�� ��ũ����ǥ �����̴�.
	RECT rcMain = {};
	m_pFrameWnd->GetWindowRect(&rcMain);
	::SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	// View�� ũ�⸦ ���´�.
	// GetClientRect: �������� �׵θ��� �������� ���� ���� Ŭ���̾�Ʈ ������ �簢�� ũ�⸦ ������ �Լ�.
	// �� �� left, top, rigth, bottom�� Ŭ���̾�Ʈ ��ǥ �����̴�.
	// ��, left, top�� �׻� 0, 0�̴�.
	RECT rcView = {};
	GetClientRect(&rcView);

	// MainFrame�� View�� ������ ���.
	int iGapX = rcMain.right - rcView.right;
	int iGapY = rcMain.bottom - rcView.bottom;

	// MainFrame�� ������ ��ġ�� ũ�⸦ �ٽ� ����.
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


	//�⺻��.
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

//Ÿ�� �Է�
void CMyToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
			//�ν��Ͻ� �ϰ��.
			if (m_pMyForm->m_MapTool.m_Instacne.GetCheck() == 1)
			{
				pGameObject->SetObjectTag(L"Instance");
			}
			//���̾�����
			if (m_pMyForm->m_MapTool.m_Layer.GetCurSel() != -1)
				pGameObject->SetObjectLayer((Layer)m_pMyForm->m_MapTool.m_Layer.GetCurSel());

			//Ʈ������ ������Ʈ
			CTransform* pTransform = new CTransform;
			pTransform->Initialize(pGameObject);
			D3DXVECTOR3 pos = D3DXVECTOR3((float)mousePos.x, (float)mousePos.y, 0.0f);
			pTransform->SetPosition(pos);

			pGameObject->AddComponent(pTransform);

			// ���� ������Ʈ �ֱ�.
			CTextureRenderer* pRender = new CTextureRenderer;
			pRender->Initialize(pGameObject);
			pRender->SetTexture((LPCTSTR)tileName);
			pRender->SetVertex(size, tex);

			pGameObject->AddComponent(pRender);

			//�浹�ϴ°��.
			if (m_pMyForm->m_MapTool.m_Collide.GetCheck() == 1)
			{
				CBoxCollider*	pBoxCollider = new CBoxCollider;
				pBoxCollider->Initialize(pGameObject);
				pBoxCollider->SetCollideType(NORMAL);
				pGameObject->AddComponent(pBoxCollider);
			}

			//0�� ��ȯ�� ���� �θ�λ����ϴ°�
			m_pInspect->m_HierarchyView.AddObject(pGameObject);
			m_pObjectMgr->AddObject(pGameObject);
			Invalidate(FALSE);
		}
	}

	
}
//Ÿ�� �����
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	//ī�޶� ������Ʈ
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

//������ Ŭ���� �����°� (���ؽ�Ʈ �޴�)
void CMyToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

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


//������Ʈ �߰�
void CMyToolView::OnObjectPopUp()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CPoint mousePos = MousePicking(m_MousePoint);

	wstring name = L"GameObject" + to_wstring(m_pObjectMgr->GetObjectCount());

	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	pGameObject->SetObjectName(name);

	//Ʈ������ ������Ʈ
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_bIsActive = true;
}

void CMyToolView::OnPause()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_bIsActive = false;
	Invalidate(FALSE);
}

void CMyToolView::OnInit()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}
