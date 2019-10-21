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
	//��ġ �ޱ�
	m_pDevice = CDeviceMgr::GetInstance()->GetDevice();

	HRESULT hr = 0;
	hr = D3DXCreateTexture(m_pDevice, WINCX, WINCY, 1, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &m_pTexture);
	FAILED_CHECK_MSG_RETURN(hr, L"Ÿ�� �ؽ��� ���� ����", E_FAIL);
	//�����̽� ���� �ޱ�
	hr = m_pTexture->GetSurfaceLevel(0, &m_pCurSurface);
	FAILED_CHECK_MSG_RETURN(hr, L" �����̽� �ޱ� ����", E_FAIL);

	//�ʱ�ȭ ����
	m_ClearColor = color;

	return S_OK;
}

//�����ϵ���
HRESULT CTarget::ChangeNewDevice(int target)
{
	//�����̽��� �������� ��Ƶд�.
	m_pDevice->GetRenderTarget(target, &m_pPreSurface);

	HRESULT hr = 0;
	hr = m_pDevice->SetRenderTarget(target, m_pCurSurface);
	FAILED_CHECK_MSG_RETURN(hr, L" ����Ÿ�� �ٲٱ� ����",E_FAIL);

	return S_OK;
}

HRESULT CTarget::ChangeToPreDevice(int target)
{

	HRESULT hr = 0;
	hr = m_pDevice->SetRenderTarget(target, m_pPreSurface);
	FAILED_CHECK_MSG_RETURN(hr, L" ����Ÿ�� �ٲٱ� ����", E_FAIL);

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
	NULL_CHECK_MSG_RETURN(m_pCurSurface, L"�ֱ� �����̽� ��",E_FAIL);
	hr = m_pCurSurface->Release();
	FAILED_CHECK_MSG_RETURN(hr, L"�ֱ� �����̽� ���� ����", E_FAIL);
	NULL_CHECK_MSG_RETURN(m_pPreSurface, L"���� �����̽� ��",E_FAIL);
	hr = m_pPreSurface->Release();
	FAILED_CHECK_MSG_RETURN(hr, L"�ֱ� �����̽� ���� ����", E_FAIL);

	return S_OK;
}

LPDIRECT3DTEXTURE9 CTarget::GetTexture()
{
	return m_pTexture;
}
