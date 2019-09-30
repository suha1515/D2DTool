
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
END_MESSAGE_MAP()

// CMyToolView ����/�Ҹ�
HWND g_hWnd;

CMyToolView::CMyToolView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pCameraMgr(CCameraMgr::GetInstance()),
	m_pObjectMgr(CObjectMgr::GetInstance()),
	m_pShaderMgr(CShaderMgr::GetInstance()),
	m_Cam(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	g_hWnd = m_hWnd;

	HRESULT hr = 0;

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	m_pShaderMgr->Initialize();
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");

	
}

CMyToolView::~CMyToolView()
{
	cout << "����Ҹ���" << endl;

	for (auto &i : m_GameObject)
		SafeDelete(i);
	m_GameObject.clear();
	m_GameObject.shrink_to_fit();

	m_pObjectMgr->DestroyInstance();
	m_pShaderMgr->DestroyInstance();
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


	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 1.0f, -1.0f, 1.0f);
	D3DXMATRIX matWorld;
	
	TCHAR szIndex[MIN_STR] = L"";
	

	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE| D3DXSPRITE_SORT_TEXTURE);
	{
		D3DXMatrixTranslation(&matTrans,0.0f,0.0f, 0.0f);
		matWorld = matScale * matTrans;
		swprintf_s(szIndex, L"%d", 4000);
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(),
			szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	}
	m_pDeviceMgr->GetSprite()->End();

	m_pObjectMgr->Update();

	m_pDeviceMgr->Render_End(m_hWnd);
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

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//��ũ�� ������ �����ϴ°�
	int cx = TILECX*TILEX;
	int cy = TILECY / 2 * TILEY;

	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));

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
	

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	RECT rc = {};
	GetClientRect(&rc);
	float winX = rc.right - rc.left;
	float winY = rc.bottom - rc.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_pCameraMgr->SetMainCamera(m_Cam);
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));

}

void CMyToolView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	RECT rcView = {};
	GetClientRect(&rcView);

	float winX = rcView.right - rcView.left;
	float winY = rcView.bottom - rcView.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(2.0f, 2.0f, 1.0f));

}

//Ÿ�� �Է�
void CMyToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CView::OnLButtonDown(nFlags, point);

	CString tileName = m_pMyForm->GetMapTool()->GetTileName();
	const XMFLOAT2* tex = m_pMyForm->GetMapTool()->GetTexPos();

	const CPoint mousePos = MousePicking(point);
	if (tex != nullptr)
	{
		//Ÿ�� �ߺ� �˻� - �ߺ��� �ؽ�ó ������ �ٲ۴�.
		auto iter_find = find_if(m_GameObject.begin(), m_GameObject.end(),
		[&mousePos](CGameObject* pGameObject)
		{
			CTransform* pTransform = pGameObject->GetComponent<CTransform>();
			if (pTransform->GetPosition() == D3DXVECTOR3(mousePos.x, mousePos.y, 1.0f))
			{
				return true;
			}
			return false;
		});

		if (m_GameObject.end() != iter_find)
		{
			(*iter_find)->GetComponent<CTextureRenderer>()->SetTexture((LPCTSTR)tileName);
			(*iter_find)->GetComponent<CTextureRenderer>()->SetVertex(16, tex);
		}
		else
		{
			wstring name = L"GameObject" + to_wstring(m_pObjectMgr->GetRootObjectCount());

			CGameObject* pGameObject = new CGameObject;
			pGameObject->Initialize();
			pGameObject->SetObjectName(name);

			//Ʈ������ ������Ʈ
			CTransform* pTransform = new CTransform;
			pTransform->Initialize();
			pTransform->SetPosition(D3DXVECTOR3(mousePos.x, mousePos.y, 0.0f));

			pGameObject->AddComponent(pTransform);

			// ���� ������Ʈ �ֱ�.
			CTextureRenderer* pRender = new CTextureRenderer;
			pRender->Initialize();
			pRender->SetTexture((LPCTSTR)tileName);
			pRender->SetVertex(16, tex);

			pGameObject->AddComponent(pRender);

			
			//0�� ��ȯ�� ���� �θ�λ����ϴ°�
			if (m_pInspect->m_HierarchyView.AddObject(pGameObject) == 0)
			{
				m_pObjectMgr->AddObject(pGameObject);
			}
		}
		Invalidate(FALSE);
	}
}

//Ÿ�� �����
void CMyToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnRButtonDown(nFlags, point);

	const CPoint mousePos = MousePicking(point);

	//�ش� ��ǥ�� ������Ʈ�� ã�´�. Ÿ���߿���.
	auto iter_find = find_if(m_GameObject.begin(), m_GameObject.end(),
	[&mousePos](CGameObject* pObject)
	{
		CTransform* pTransform = pObject->GetComponent<CTransform>();
		if (pTransform->GetPosition() == D3DXVECTOR3(mousePos.x, mousePos.y, 0.0f))
		{
			return true;
		}
		else return false;
	});

	if (m_GameObject.end() != iter_find)
	{
		SafeDelete((*iter_find));
		m_GameObject.erase(iter_find);
		Invalidate(FALSE);
	}
}

void CMyToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnMouseMove(nFlags, point);

	m_MousePoint = point;
}




void CMyToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
	cout << "Ű����" << endl;
	
	if (nChar == VK_LEFT)
	{
		m_Cam->MoveCamera(-5.0f, 0.0f);
		cout << "���� ����!" << endl;
	}
	else if (nChar == VK_UP)
	{
		m_Cam->MoveCamera(0.0f, 5.0f);
		cout << "���� ��!" << endl;
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




BOOL CMyToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	
	float scale = 0.0f;
	if (zDelta <= 0)
	{
		scale = -0.1f;
		cout << "���콺 �� �ٿ�" << endl;
	}
	else
	{
		scale = 0.1f;
		cout << "���콺 �� ��" << endl;
	}
	m_Cam->Scaling(scale, scale);
	Invalidate(FALSE);
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
	
}



const CPoint& CMyToolView::MousePicking(const CPoint& point)
{
	RECT rc = {};
	GetClientRect(&rc);
	float winX = rc.right - rc.left;
	float winY = rc.bottom - rc.top;

	D3DXVECTOR3 vMouse = { float(point.x) - winX*0.5f,(winY*0.5f - float(point.y)),1.0f };
	D3DXVECTOR4 vMouse2;
	D3DXMATRIX viewMat = *m_Cam->GetViewMatrix();
	D3DXMATRIX InvViewMat;
	D3DXMatrixInverse(&InvViewMat, NULL, &viewMat);
	D3DXVec3Transform(&vMouse2, &vMouse, &InvViewMat);

	cout << vMouse.x << " , " << vMouse.y << endl;
	float newX, newY;
	if (vMouse2.x>0)
		newX = ((int)vMouse2.x / 16)*16.f + 8.0f;
	else
		newX = ((int)vMouse2.x / 16)*16.f - 8.0f;

	if (vMouse2.y>0)
		newY = ((int)vMouse2.y / 16)*16.f + 8.0f;
	else
		newY = ((int)vMouse2.y / 16)*16.f - 8.0f;

	cout << newX << " , " << newY << endl;

	return CPoint(newX, newY);
}

//������ Ŭ���� �����°�
void CMyToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	CMenu popup;
	CMenu* pMenu;
	popup.LoadMenuW(IDR_MENU3);
	
	pMenu = popup.GetSubMenu(0);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, point.x, point.y,this);
}


//������Ʈ �߰�
void CMyToolView::OnObjectPopUp()
{
	

	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CPoint mousePos = MousePicking(m_MousePoint);

	wstring name = L"GameObject" + to_wstring(m_pObjectMgr->GetRootObjectCount());

	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	pGameObject->SetObjectName(name);

	//Ʈ������ ������Ʈ
	CTransform* pTransform = new CTransform;
	pTransform->Initialize();
	pTransform->SetPosition(D3DXVECTOR3(mousePos.x, mousePos.y, 0.0f));

	pGameObject->AddComponent(pTransform);

	//0�� ��ȯ�� ���� �θ�λ����ϴ°�
	if (m_pInspect->m_HierarchyView.AddObject(pGameObject) == 0)
	{
		m_pObjectMgr->AddObject(pGameObject);
	}

}
