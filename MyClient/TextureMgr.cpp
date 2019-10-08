#include "stdafx.h"
#include "TextureMgr.h"
#include "Texture.h"


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
	const wstring& wstrStateKey)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	if (m_mapTexture.end() == iter_find)
		return nullptr;

	return iter_find->second->GetTexInfo(wstrStateKey);
}

HRESULT CTextureMgr::LoadTexture(
	const wstring& wstrFilePath, 
	const wstring& wstrObjectKey, 
	const wstring& wstrStateKey)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);

	// 오브젝트 키가 없다면 최초 할당 후 텍스처 로드 및 맵 삽입.
	if (m_mapTexture.end() == iter_find)
	{
		CTexture* pTexture = nullptr;

		pTexture = CTexture::Create(wstrFilePath, wstrStateKey);

		NULL_CHECK_RETURN(pTexture, E_FAIL);
		m_mapTexture.insert({ wstrObjectKey, pTexture });
	}
	else
	{
		// 오브젝트 키가 이미 존재한다면.
		HRESULT hr = m_mapTexture[wstrObjectKey]->LoadTexture(
			wstrFilePath, wstrStateKey);
		FAILED_CHECK_RETURN(hr, E_FAIL);
	}
	return S_OK;
}

HRESULT CTextureMgr::RemoveTexture(const wstring & wstrObjectKey, const wstring & wstrStateKey)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);
	HRESULT hr = 0;
	if (m_mapTexture.end() != iter_find)
	{
		hr = iter_find->second->RemoveTexture(wstrStateKey);
		FAILED_CHECK_MSG_RETURN(hr, L"Remove State Texture Failed",E_FAIL);
	}
	else
		return E_FAIL;
	return S_OK;
}

HRESULT CTextureMgr::RemoveTexture(const wstring & wstrObjectKey)
{
	auto iter_find = m_mapTexture.find(wstrObjectKey);
	if (m_mapTexture.end() != iter_find)
	{
		SafeDelete(iter_find->second);
		m_mapTexture.erase(iter_find);
	}
	else
		return E_FAIL;
	return S_OK;
}

HRESULT CTextureMgr::LoadTextureFromList(const wstring & filePath)
{
	HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	for (auto& MyPair : m_mapTexture)
		SafeDelete(MyPair.second);

	m_mapTexture.clear();

	DWORD dwBytes = 0;

	MAP_SAVE info;
	while (true)
	{
		ReadFile(hFile, &info, sizeof(MAP_SAVE), &dwBytes, nullptr);
		if (0 == dwBytes)
			break;

		HRESULT hr = 0;
		//텍스쳐 매니저에 입력
		hr =LoadTexture(info.filePath, L"TILE_MAP", info.fileName);
		FAILED_CHECK_MSG_RETURN(hr, L"Adding TileMap Failed",E_FAIL);
	}
	CloseHandle(hFile);
	return S_OK;
}

void CTextureMgr::Release()
{
	for (auto& MyPair : m_mapTexture)
		SafeDelete(MyPair.second);

	m_mapTexture.clear();
}
