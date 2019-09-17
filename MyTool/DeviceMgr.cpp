#include "stdafx.h"
#include "DeviceMgr.h"

IMPLEMENT_SINGLETON(CDeviceMgr)

CDeviceMgr::CDeviceMgr()
	: m_pSDK(nullptr), m_pDevice(nullptr)
{
}


CDeviceMgr::~CDeviceMgr()
{
	Release();
}

LPDIRECT3DDEVICE9 CDeviceMgr::GetDevice()
{
	return m_pDevice;
}

LPD3DXSPRITE CDeviceMgr::GetSprite()
{
	return m_pSprite;
}

LPD3DXFONT CDeviceMgr::GetFont()
{
	return m_pFont;
}

HRESULT CDeviceMgr::InitDevice(MODE eMode)
{
	HRESULT hr = 0;

	// ��ġ �ʱ�ȭ.

	// 1. IDirect3D9 ��ü ����
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_MSG_RETURN(m_pSDK, L"Direct3DCreate9 Failed", E_FAIL);

	// 2. ��ġ ����
	// 2-1. HAL�� ���� ��ġ ������ ����.
	D3DCAPS9	d3dcaps;
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	// GetDeviceCaps: �׷���ī�带 �����ؼ� ������ D3DCAPS9 ����ü�� ��Ƴ���.
	// HAL(Hardware Abstraction Layer, �ϵ���� �߻� ����)
	hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);
	FAILED_CHECK_MSG_RETURN(hr, L"GetDeviceCaps Failed", E_FAIL);

	// 2-2. ���� �׷��� ��ġ�� ���ؽ� ���μ����� �����ϴ°� ����
	// *���ؽ� ���μ���: ���� ��ȯ + ���� ó��
	DWORD vp = 0;

	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 3. IDirect3DDevice9 ��ü ����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	// ������ ȯ�� ����.
	d3dpp.BackBufferWidth = WINCX;	// �ĸ���� ���� ������
	d3dpp.BackBufferHeight = WINCY; // �ĸ���� ���� ������
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // �ĸ���� �ȼ� ����
	d3dpp.BackBufferCount = 1; // �ĸ���� ����

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // ���� ü�� ���
	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = eMode; // TRUE â���, FALSE ��üȭ����
	d3dpp.EnableAutoDepthStencil = TRUE; // ���̹��ۿ� ���ٽǹ��۸� ��ġ�� �ڵ� ����.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // ���� ����� �ֻ����� �����.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // ��� �ÿ� �Ѵ�.

	m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp,
		&m_pDevice);

	// ��������Ʈ Com��ü ����
	hr = D3DXCreateSprite(m_pDevice, &m_pSprite);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateSprite Failed", E_FAIL);

	// ���̷�Ʈ ��Ʈ Com��ü ����
	D3DXFONT_DESC	tFontInfo;	// ��Ʈ����
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));

	tFontInfo.Width = 10;	// ��Ʈ ����
	tFontInfo.Height = 20;	// ��Ʈ ����
	tFontInfo.Weight = FW_HEAVY;	// ��Ʈ ����
	tFontInfo.CharSet = HANGEUL_CHARSET;	// ���� ���
	lstrcpy(tFontInfo.FaceName, L"�ü�");	// ��ü

	hr = D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateFontIndirect Failed", E_FAIL);

	return S_OK;
}

void CDeviceMgr::Render_Begin()
{
	// ��ġ ������ ����

	// 1. �ĸ���۸� ����. (clear)
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);

	// 2. �ĸ���ۿ� �׸��⸦ �����Ѵ�. (begin)
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDeviceMgr::Render_End(HWND hWnd/* = nullptr*/)
{
	// 3. �ĸ���ۿ� �׸��⸦ ������. (end)
	m_pSprite->End();
	m_pDevice->EndScene();

	// 4. �ĸ���۸� ������۷� ��ü�Ͽ� ȭ�鿡 ����Ѵ�. (present)
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDeviceMgr::Release()
{
	// Com��ü ����
	if (m_pFont->Release())
		::MessageBox(0, L"m_pFont Release Failed", L"System Error", MB_OK);
	if(m_pSprite->Release())
		::MessageBox(0, L"m_pSprite Release Failed", L"System Error", MB_OK);
	if (m_pDevice->Release())
		::MessageBox(0, L"m_pDevice Release Failed", L"System Error", MB_OK);
	if(m_pSDK->Release())
		::MessageBox(0, L"m_pSDK Release Failed", L"System Error", MB_OK);
}
