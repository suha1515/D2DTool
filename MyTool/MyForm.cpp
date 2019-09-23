// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MyForm.h"
#include "Camera.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance())
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEX_VALUE, m_TexValue);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedUnitTool)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnBnClickedUnitTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_UnitTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//���̾�α� ����
	if (nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);

	HRESULT hr = 0;
	m_texInfo = m_pTextureMgr->GetTexInfo(L"MAP_TILE", L"AUTUMN_OUTSIDE");
	NULL_CHECK_MSG(m_texInfo, L"texinfo is nullptr");

	RECT rc = {};
	GetClientRect(&rc);
	float winX = rc.right - rc.left;
	float winY = rc.bottom - rc.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_Cam = new CCamera;
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(2.0f, 2.0f, 1.0f));

	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

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
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
}


void CMyForm::OnDraw(CDC* /*pDC*/)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//��ó ��Ʈ�� ���̾�α�.
	HWND hwnd;
	hwnd = (HWND)(*GetDlgItem(IDC_TILE_VIEW));


	m_pDeviceMgr->Render_Begin();
	
	m_pDeviceMgr->Render_End(hwnd);

}

void CMyForm::Renew(XMFLOAT2 * tex)
{
	//m_TexValue.SetWindowTextW(str);
	m_Tex[0] = tex[0];
	m_Tex[1] = tex[1];
	m_Tex[2] = tex[2];
	m_Tex[3] = tex[3];
}
