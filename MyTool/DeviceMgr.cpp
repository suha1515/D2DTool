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

	// 장치 초기화.

	// 1. IDirect3D9 객체 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	NULL_CHECK_MSG_RETURN(m_pSDK, L"Direct3DCreate9 Failed", E_FAIL);

	// 2. 장치 조사
	// 2-1. HAL을 통해 장치 정보를 얻어옴.
	D3DCAPS9	d3dcaps;
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	// GetDeviceCaps: 그래픽카드를 조사해서 정보를 D3DCAPS9 구조체에 담아낸다.
	// HAL(Hardware Abstraction Layer, 하드웨어 추상 계층)
	hr = m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);
	FAILED_CHECK_MSG_RETURN(hr, L"GetDeviceCaps Failed", E_FAIL);

	// 2-2. 현재 그래픽 장치가 버텍스 프로세싱을 지원하는가 조사
	// *버텍스 프로세싱: 정점 변환 + 조명 처리
	DWORD vp = 0;

	if (d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 3. IDirect3DDevice9 객체 생성
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	// 렌더링 환경 설정.
	d3dpp.BackBufferWidth = WINCX;	// 후면버퍼 가로 사이즈
	d3dpp.BackBufferHeight = WINCY; // 후면버퍼 세로 사이즈
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // 후면버퍼 픽셀 포맷
	d3dpp.BackBufferCount = 1; // 후면버퍼 개수

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // 스왑 체인 방식
	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = eMode; // TRUE 창모드, FALSE 전체화면모드
	d3dpp.EnableAutoDepthStencil = TRUE; // 깊이버퍼와 스텐실버퍼를 장치가 자동 관리.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	/* FullScreen_RefreshRateInHz must be zero for Windowed mode */
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // 현재 모니터 주사율에 맞춘다.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // 즉시 시연 한다.

	m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp,
		&m_pDevice);

	// 스프라이트 Com객체 생성
	hr = D3DXCreateSprite(m_pDevice, &m_pSprite);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateSprite Failed", E_FAIL);

	// 다이렉트 폰트 Com객체 생성
	D3DXFONT_DESC	tFontInfo;	// 폰트정보
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESC));

	tFontInfo.Width = 10;	// 폰트 가로
	tFontInfo.Height = 20;	// 폰트 세로
	tFontInfo.Weight = FW_HEAVY;	// 폰트 굵기
	tFontInfo.CharSet = HANGEUL_CHARSET;	// 지원 언어
	lstrcpy(tFontInfo.FaceName, L"궁서");	// 서체

	hr = D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont);
	FAILED_CHECK_MSG_RETURN(hr, L"D3DXCreateFontIndirect Failed", E_FAIL);

	return S_OK;
}

void CDeviceMgr::Render_Begin()
{
	// 장치 렌더링 과정

	// 1. 후면버퍼를 비운다. (clear)
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);

	// 2. 후면버퍼에 그리기를 시작한다. (begin)
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDeviceMgr::Render_End(HWND hWnd/* = nullptr*/)
{
	// 3. 후면버퍼에 그리기를 끝낸다. (end)
	m_pSprite->End();
	m_pDevice->EndScene();

	// 4. 후면버퍼를 전면버퍼로 교체하여 화면에 출력한다. (present)
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

void CDeviceMgr::Release()
{
	// Com객체 해제
	if (m_pFont->Release())
		::MessageBox(0, L"m_pFont Release Failed", L"System Error", MB_OK);
	if(m_pSprite->Release())
		::MessageBox(0, L"m_pSprite Release Failed", L"System Error", MB_OK);
	if (m_pDevice->Release())
		::MessageBox(0, L"m_pDevice Release Failed", L"System Error", MB_OK);
	if(m_pSDK->Release())
		::MessageBox(0, L"m_pSDK Release Failed", L"System Error", MB_OK);
}
