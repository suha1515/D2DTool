// MiniView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "MyTool.h"
#include "MiniView.h"
#include "Camera.h"


// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CScrollView)



CMiniView::CMiniView()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	 m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pVB(nullptr),m_pIB(nullptr),m_Cam(nullptr)
{
	cout << "�� ������" << endl;
}

CMiniView::~CMiniView()
{
	delete m_Cam;
	if (m_pIB->Release())
		MessageBox(0, L"m_pIB Release Failed", MB_OK);
	if (m_pVB->Release())
		MessageBox(0, L"m_pVB Release Failed", MB_OK);

	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

BEGIN_MESSAGE_MAP(CMiniView, CScrollView)
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CMiniView �׸����Դϴ�.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	m_pDeviceMgr->Render_Begin();

	m_Cam->SetTransform();
	m_pDeviceMgr->GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
	m_pDeviceMgr->GetDevice()->SetIndices(m_pIB);
	m_pDeviceMgr->GetDevice()->SetFVF(FVF_VERTEX);
	m_pDeviceMgr->GetDevice()->SetTexture(0, m_texInfo->pTexture);

	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pDeviceMgr->GetDevice()->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		6,
		0,
		2
	);
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	/*D3DXMATRIX* mat = m_Cam->GetViewProjMatrix();
	m_pDeviceMgr->GetLine()->Begin();

	D3DXVECTOR3 vLine[2];

	vLine[0] = { -winX*0.5f,winY*0.5f,0.0f };
	vLine[1] = { 0.f,0.f,0.0f };

	D3DXVECTOR2 vLine2[2];
	vLine2[0] = { 10.f,0.0f };
	vLine2[1] = { 50.f,0.0f };

	m_pDeviceMgr->GetLine()->DrawTransform(vLine, 2, mat, D3DCOLOR_XRGB(255, 0, 0));
	m_pDeviceMgr->GetLine()->SetWidth(3);
	vLine[0] = { 10.0f,0.0f,0.0f };
	vLine[1] = { 50.f,0.f,0.0f };
	m_pDeviceMgr->GetLine()->DrawTransform(vLine, 2, mat, D3DCOLOR_XRGB(0, 0, 0));


	m_pDeviceMgr->GetLine()->End();
*/
	m_pDeviceMgr->Render_End(m_hWnd);


}


// CMiniView �����Դϴ�.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
void CMiniView::Initialize()
{

}
#endif
#endif //_DEBUG


// CMiniView �޽��� ó�����Դϴ�.


void CMiniView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	HRESULT hr = 0;
	hr = m_pTextureMgr->LoadTexture(L"../Texture/Map/autumn-outside.png", L"MAP_TILE", L"AUTUMN_OUTSIDE");
	FAILED_CHECK_MSG(hr, L"MAP_AUTUMN LOAD FAILED");

	m_texInfo = m_pTextureMgr->GetTexInfo(L"MAP_TILE", L"AUTUMN_OUTSIDE");
	//��ũ�� ������ �����ϴ°�
	int cx = m_texInfo->tImgInfo.Width;
	int cy = m_texInfo->tImgInfo.Height;

	CScrollView::SetScrollSizes(MM_TEXT, CSize(cx, cy));

	

	RECT rect;
	this->GetClientRect(&rect);
	float winX = rect.right - rect.left;
	float winY = rect.bottom - rect.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));


	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//// use alpha channel in texture for alpha �̹������� ���İ� ������
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//// set blending factors so that alpha component determines transparency
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

}


void CMiniView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnMouseMove(nFlags, point);


}


void CMiniView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);

	if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		D3DXVECTOR3 vMouse = { float(point.x)+GetScrollPos(0),float(point.y) + GetScrollPos(1),0.f };
		cout << int(vMouse.x)/16 << " , " << int(vMouse.y)/16 << endl;
	}


}


void CMiniView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);

	cout << "���� ��ũ�ѹ�!" << endl;
	cout << nPos << endl;
	RECT rc;
	GetClientRect(&rc);

	SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.right - rc.left, rc.bottom - rc.top));
}


void CMiniView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	cout << "���� ��ũ�ѹ�!" << endl;
	RECT rc;
	GetClientRect(&rc);

	SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.right - rc.left, rc.bottom - rc.top));
}


void CMiniView::OnSize(UINT nType, int cx, int cy)
{

	CScrollView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_pVB != nullptr)
		m_pVB->Release();
	if (m_pIB != nullptr)
		m_pIB->Release();
	if (m_Cam == nullptr)
		m_Cam = new CCamera;

	RECT rect;
	this->GetClientRect(&rect);

	float winX = rect.right - rect.left;
	float winY = rect.bottom - rect.top;

	cout << "���� â : " << winX << endl;
	cout << "���� â : " << winY << endl;

	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_Cam->Update();

	int scrollX = GetScrollPos(0);
	int scrollY = GetScrollPos(1);



	m_Vertex[0] = Vertex(-winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f,	 (scrollX/512.f)/2.f,	 (scrollY / 1024.f) / 2.f);
	m_Vertex[1] = Vertex(-winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, (scrollX/512.f) / 2.f, ((winY + scrollY)/1024.f) / 2.f);
	m_Vertex[2] = Vertex( winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f,  ((winX + scrollX) / 512.f) / 2.f, (scrollY/1024.f) / 2.f);
	m_Vertex[3] = Vertex( winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, ((winX + scrollX) / 512.f) / 2.f, ((winY + scrollY) / 1024.f) / 2.f);

	/*m_Vertex[0] = Vertex(-winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	m_Vertex[1] = Vertex(-winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[2] = Vertex(winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	m_Vertex[3] = Vertex(winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,1.0f);*/

	m_pDeviceMgr->GetDevice()->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, FVF_VERTEX, D3DPOOL_MANAGED, &m_pVB, 0);
	Vertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	v[0] = m_Vertex[0];
	v[1] = m_Vertex[1];
	v[2] = m_Vertex[2];
	v[3] = m_Vertex[3];

	m_pVB->Unlock();

	m_pDeviceMgr->GetDevice()->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);

	WORD* i = 0;
	m_pIB->Lock(0, 0, (void**)&i, 0);
	i[0] = 0, i[1] = 2, i[2] = 1;
	i[3] = 1, i[4] = 2, i[5] = 3;


	m_pIB->Unlock();
}


int CMiniView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	cout << "On Create" << endl;
	return 0;
}
