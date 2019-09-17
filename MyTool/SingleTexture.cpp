#include "stdafx.h"
#include "SingleTexture.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

const TEX_INFO* CSingleTexture::GetTexInfo(
	const wstring& wstrStateKey, int iIndex)
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::LoadTexture(
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey/* = L""*/, 
	int iImgCount/* = 0*/)
{
	HRESULT hr = 0;

	LPDIRECT3DDEVICE9 pDevice = m_pDeviceMgr->GetDevice();
	NULL_CHECK_RETURN(pDevice, E_FAIL);

	D3DXIMAGE_INFO	tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));

	// 파일로부터 이미지 정보를 얻어오는 함수.
	hr = D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &tImgInfo);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	// 파일을 통해 텍스처를 불러와 IDirect3DTexture9객체를 생성하는 함수.
	hr = D3DXCreateTextureFromFileEx(
		pDevice, /* 장치 */
		wstrFilePath.c_str(), /* 이미지 파일 경로 */
		tImgInfo.Width, /* 텍스처 가로 크기 */
		tImgInfo.Height, /* 텍스처 세로 크기 */
		tImgInfo.MipLevels, /* 밉레벨, 밉맵의 단계 */
		0, /* 텍스처 사용 방식 */
		tImgInfo.Format, /* 텍스처  픽셀 포맷 */
		D3DPOOL_MANAGED, /* 메모리 사용 방식 */
		D3DX_DEFAULT, /* 필터링 방식 */
		D3DX_DEFAULT, /* 밉맵 필터링 방식 */
		0, /* 제거할 색상 */
		nullptr, /* D3DXIMAGE_INFO의 포인터 */
		nullptr, /* PALETTEENTRY의 포인터 */
		&pTexture /* LPDIRECT3DTEXTURE9의 포인터*/);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	m_pTexInfo = new TEX_INFO;
	m_pTexInfo->pTexture = pTexture;
	m_pTexInfo->tImgInfo = tImgInfo;

	return S_OK;
}

void CSingleTexture::Release()
{
	m_pTexInfo->pTexture->Release();
	SafeDelete(m_pTexInfo);
}

CSingleTexture* CSingleTexture::Create(const wstring & wstrFilePath)
{
	CSingleTexture* pInstance = new CSingleTexture;

	if (FAILED(pInstance->LoadTexture(wstrFilePath)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
