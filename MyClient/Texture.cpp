#include "stdafx.h"
#include "Texture.h"


CTexture::CTexture()
	: m_pDeviceMgr(CDeviceMgr::GetInstance())
{
}


CTexture::~CTexture()
{
	Release();
}

TEX_INFO * CTexture::GetTexInfo(const wstring & wstrStateKey)
{	
	auto iter = m_mapTexInfo.find(wstrStateKey);
	if (m_mapTexInfo.end() == iter)
		return nullptr;
	return m_mapTexInfo[wstrStateKey];
}

HRESULT CTexture::LoadTexture(const wstring & wstrFilePath, const wstring & wstrStateKey)
{
	//�ߺ��Ǵ� ���� ������ FAIL ��ȯ
	auto iter_find = m_mapTexInfo.find(wstrStateKey);
	if (m_mapTexInfo.end() != iter_find)
		return E_FAIL;

	HRESULT hr = 0;
	LPDIRECT3DDEVICE9 pDevice = m_pDeviceMgr->GetDevice();
	NULL_CHECK_RETURN(pDevice, E_FAIL);

	D3DXIMAGE_INFO tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));

	//���Ϸκ��� �̹��� ������ ������ �Լ�

	hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &tImgInfo);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	//������ ���� �ؽ�ó�� �ҷ��� IDirect3DTexture9 ��ü�� �����ϴ� �Լ�.
	hr = D3DXCreateTextureFromFile(pDevice, wstrFilePath.c_str(), &pTexture);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	TEX_INFO* m_pInfo = new TEX_INFO;
	m_pInfo->pTexture = pTexture;
	m_pInfo->tImgInfo = tImgInfo;
	m_pInfo->textureName = wstrStateKey.c_str();

	m_mapTexInfo[wstrStateKey] = m_pInfo;

	return S_OK;
}

HRESULT CTexture::RemoveTexture(const wstring & wstrStateKey)
{
	auto iter_find = m_mapTexInfo.find(wstrStateKey);

	if (m_mapTexInfo.end() != iter_find)
	{
		iter_find->second->pTexture->Release();
		m_mapTexInfo.erase(iter_find);
	}
	else
		return E_FAIL;

	return S_OK;
}

CTexture * CTexture::Create(const wstring & wstrFilePath, const wstring & wstrStateKey)
{

	CTexture* pInstance = new CTexture;

	if (FAILED(pInstance->LoadTexture(wstrFilePath, wstrStateKey)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTexture::Release()
{
	for (auto &i : m_mapTexInfo)
	{
		i.second->pTexture->Release();
	}
	m_mapTexInfo.clear();
}
