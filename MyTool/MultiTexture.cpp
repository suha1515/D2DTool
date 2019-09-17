#include "stdafx.h"
#include "MultiTexture.h"


CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEX_INFO* CMultiTexture::GetTexInfo(
	const wstring& wstrStateKey/* = L""*/, 
	int iIndex/* = 0*/)
{
	auto iter_find = m_mapMultiTexture.find(wstrStateKey);

	if (m_mapMultiTexture.end() == iter_find)
		return nullptr;

	return iter_find->second[iIndex];
}

// wstrFilePath:
// D:\김태윤\99C\4개월차\Framework99C\Texture\Stage\Terrain\Tile\Tile%d.png
HRESULT CMultiTexture::LoadTexture(
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey/* = L""*/, 
	int iImgCount/* = 0*/)
{
	auto iter_find = m_mapMultiTexture.find(wstrStateKey);

	if (m_mapMultiTexture.end() != iter_find)
		return E_FAIL;

	HRESULT hr = 0;
	TEX_INFO* pTexInfo = nullptr;

	LPDIRECT3DDEVICE9 pDevice = m_pDeviceMgr->GetDevice();
	NULL_CHECK_RETURN(pDevice, E_FAIL);

	D3DXIMAGE_INFO	tImgInfo;
	ZeroMemory(&tImgInfo, sizeof(D3DXIMAGE_INFO));

	TCHAR szFullPath[MAX_STR] = L"";

	for (int i = 0; i < iImgCount; ++i)
	{
		swprintf_s(szFullPath, wstrFilePath.c_str(), i);

		// 파일로부터 이미지 정보를 얻어오는 함수.
		hr = D3DXGetImageInfoFromFile(szFullPath, &tImgInfo);
		FAILED_CHECK_RETURN(hr, E_FAIL);

		LPDIRECT3DTEXTURE9 pTexture = nullptr;

		// 파일을 통해 텍스처를 불러와 IDirect3DTexture9객체를 생성하는 함수.
		hr = D3DXCreateTextureFromFileEx(
			pDevice, /* 장치 */
			szFullPath, /* 이미지 파일 경로 */
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

		pTexInfo = new TEX_INFO;
		pTexInfo->pTexture = pTexture;
		pTexInfo->tImgInfo = tImgInfo;

		m_mapMultiTexture[wstrStateKey].push_back(pTexInfo);
	}

	return S_OK;
}

void CMultiTexture::Release()
{
	for (auto& MyPair : m_mapMultiTexture)
	{
		for (auto& pTexInfo : MyPair.second)
		{
			pTexInfo->pTexture->Release();
			SafeDelete(pTexInfo);
		}

		MyPair.second.clear();
		MyPair.second.shrink_to_fit();
	}

	m_mapMultiTexture.clear();
}

CMultiTexture* CMultiTexture::Create(
	const wstring& wstrFilePath, 
	const wstring& wstrStateKey, 
	int iImgCount)
{
	CMultiTexture* pInstance = new CMultiTexture;

	if (FAILED(pInstance->LoadTexture(wstrFilePath, wstrStateKey, iImgCount)))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
