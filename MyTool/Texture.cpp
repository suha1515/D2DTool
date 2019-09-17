#include "stdafx.h"
#include "Texture.h"


CTexture::CTexture()
	: m_pDeviceMgr(CDeviceMgr::GetInstance())
{
}


CTexture::~CTexture()
{
}

TEX_INFO * CTexture::GetTexInfo(const wstring & wstrStateKey)
{
	
	
	return nullptr;
}

HRESULT CTexture::LoadTexture(const wstring & wstrFilePath, const wstring & wstrStateKey)
{
	HRESULT hr = 0;
	LPDIRECT3DDEVICE9 pDevice = m_pDeviceMgr->GetDevice();
	NULL_CHECK_RETURN(pDevice, E_FAIL);

	D3DXIMAGE_INFO tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));

	//파일로부터 이미지 정보를 얻어오는 함수

	hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &tImgInfo);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	//파일을 통해 텍스처를 불러와 IDirect3DTexture9 객체를 생성하는 함수.
	hr = D3DXCreateTextureFromFile(pDevice, wstrFilePath.c_str(), &pTexture);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pTexInfo = new TEX_INFO;
	m_pTexInfo->pTexture = pTexture;
	m_pTexInfo->tImgInfo = tImgInfo;

	return S_OK;
}
