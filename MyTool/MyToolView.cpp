
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
#include "Terrain.h"

#include "GameObject.h"
#include "Camera.h"

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
END_MESSAGE_MAP()

// CMyToolView ����/�Ҹ�
HWND g_hWnd;

CMyToolView::CMyToolView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pCameraMgr(CCameraMgr::GetInstance()),
	m_Cam(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	g_hWnd = m_hWnd;

	HRESULT hr = 0;

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG(hr, L"InitDevice Failed");
}

CMyToolView::~CMyToolView()
{
	cout << "����Ҹ���" << endl;
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
	
	for (auto& i : m_Tile)
		i->Render();

	m_pDeviceMgr->Render_End(m_hWnd);
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CMyToolView �μ�

BOOL CMyToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMyToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMyToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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

	// ������ �����츦 ���´�.
	// AfxGetApp: CWinApp�� �����͸� ������ MFC ���� �Լ�.
	// GetMainWnd: MainFrame�� �����͸� ������ CWinApp�� ����Լ�.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pMainFrm);

	// MainFrame�� ũ�⸦ ���´�.
	// GetWindowRect: �������� ��ü �׵θ��� ������ �簢�� ũ�⸦ ������ �Լ�.
	// �� �� left, top, right, bottom�� ��ũ����ǥ �����̴�.
	RECT rcMain = {};
	pMainFrm->GetWindowRect(&rcMain);
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
	pMainFrm->SetWindowPos(
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
	
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);

	//// use alpha channel in texture for alpha �̹������� ���İ� ������
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//// set blending factors so that alpha component determines transparency
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//����
	//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

}


void CMyToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonDown(nFlags, point);
	RECT rc = {};
	GetClientRect(&rc);
	float winX = rc.right - rc.left;
	float winY = rc.bottom - rc.top;

	D3DXVECTOR3 vMouse = { float(point.x)- winX*0.5f,(winY*0.5f- float(point.y)),1.0f };
	cout << vMouse.x << " , " << vMouse.y << endl;
	D3DXVECTOR4 vMouse2;
	D3DXMATRIX viewMat = *m_Cam->GetViewMatrix();
	D3DXMATRIX InvViewMat;
	D3DXMatrixInverse(&InvViewMat, NULL, &viewMat);
	D3DXVec3Transform(&vMouse2,&vMouse,&InvViewMat);

	cout <<"��ȯ�� :"<< vMouse2.x << " , " << vMouse2.y << endl;

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pFrameWnd->m_SecondSplitter.GetPane(1, 0));
	NULL_CHECK(pMyForm);

	
	pMyForm->GetTileName();

	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	pGameObject->SetPosition(D3DXVECTOR3(vMouse.x, vMouse2.y, vMouse2.z));
	pGameObject->SetVertex(16, pMyForm->GetTexPos());

}


void CMyToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnMouseMove(nFlags, point);
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
