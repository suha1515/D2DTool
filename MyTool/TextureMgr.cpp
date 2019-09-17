#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEX_INFO* CTextureMgr::GetTexInfo(
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	int iIndex)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if (m_mapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexInfo(wstrStateKey, iIndex);
}

HRESULT CTextureMgr::LoadTexture(
	TEXTURE_TYPE eTexType, 
	const wstring& wstrFilePath, 
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey, 
	int iImgCount)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	// 오브젝트 키가 없다면 싱글이건 멀티건 최초 할당 후 텍스처 로드 및 맵 삽입.
	if (m_mapTexture.end() == iter_find)
	{
		CTexture* pTexture = nullptr;

		switch (eTexType)
		{
		case SINGLE_TEXTURE:
			pTexture = CSingleTexture::Create(wstrFilePath);
			break;
		case MULTI_TEXTURE:
			pTexture = CMultiTexture::Create(wstrFilePath, wstrStateKey, iImgCount);
			break;
		}

		NULL_CHECK_RETURN(pTexture, E_FAIL);
		m_mapTexture.insert({ wstrObjectKey, pTexture });
	}
	// 오브젝트 키가 이미 존재하고 멀티라면
	else if (MULTI_TEXTURE == eTexType)
	{
		HRESULT hr = m_mapTexture[wstrObjectKey]->LoadTexture(
			wstrFilePath, wstrStateKey, iImgCount);
		FAILED_CHECK_RETURN(hr, E_FAIL);
	}

	return S_OK;
}

void CTextureMgr::Release()
{
	for (auto& MyPair : m_mapTexture)
		SafeDelete(MyPair.second);

	m_mapTexture.clear();
}
