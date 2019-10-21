#include "stdafx.h"
#include "Target.h"

CTarget::CTarget()
{
}


CTarget::~CTarget()
{
}

HRESULT CTarget::Initialize(const int& width, const int& height, const D3DFORMAT& format, const D3DXCOLOR& color)
{
	//장치 받기
	m_pDevice = CDeviceMgr::GetInstance()->GetDevice();

	HRESULT hr = 0;
	hr = D3DXCreateTexture(m_pDevice, WINCX, WINCY, 1, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &m_pTexture);
	FAILED_CHECK_MSG_RETURN(hr, L"타겟 텍스쳐 생성 실패", E_FAIL);
	//서페이스 정보 받기
	hr = m_pTexture->GetSurfaceLevel(0, &m_pCurSurface);
	FAILED_CHECK_MSG_RETURN(hr, L" 서페이스 받기 실패", E_FAIL);

	//초기화 색깔
	m_ClearColor = color;

	return S_OK;
}

//정리하도록
HRESULT CTarget::ChangeNewDevice(int target)
{
	//서페이스에 이전정보 담아둔다.
	m_pDevice->GetRenderTarget(target, &m_pPreSurface);

	HRESULT hr = 0;
	hr = m_pDevice->SetRenderTarget(target, m_pCurSurface);
	FAILED_CHECK_MSG_RETURN(hr, L" 렌더타겟 바꾸기 실패",E_FAIL);

	return S_OK;
}

HRESULT CTarget::ChangeToPreDevice(int target)
{

	HRESULT hr = 0;
	hr = m_pDevice->SetRenderTarget(target, m_pPreSurface);
	FAILED_CHECK_MSG_RETURN(hr, L" 렌더타겟 바꾸기 실패", E_FAIL);

	m_pPreSurface->Release();
	m_pPreSurface = nullptr;

	return S_OK;
}

HRESULT CTarget::ClearColor()
{
	ChangeNewDevice(0);
	m_pDevice->Clear(0, nullptr, D3DCLEAR_TARGET, m_ClearColor, 1.0f, 0.0f);
	ChangeToPreDevice(0);

	return S_OK;
}

HRESULT CTarget::Release()
{
	HRESULT hr = 0;
	NULL_CHECK_MSG_RETURN(m_pCurSurface, L"최근 서페이스 널",E_FAIL);
	hr = m_pCurSurface->Release();
	FAILED_CHECK_MSG_RETURN(hr, L"최근 서페이스 해제 실패", E_FAIL);
	NULL_CHECK_MSG_RETURN(m_pPreSurface, L"이전 서페이스 널",E_FAIL);
	hr = m_pPreSurface->Release();
	FAILED_CHECK_MSG_RETURN(hr, L"최근 서페이스 해제 실패", E_FAIL);

	return S_OK;
}

LPDIRECT3DTEXTURE9 CTarget::GetTexture()
{
	return m_pTexture;
}
