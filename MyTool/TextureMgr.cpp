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

	// ������Ʈ Ű�� ���ٸ� ���� �Ҵ� �� �ؽ�ó �ε� �� �� ����.
	if (m_mapTexture.end() == iter_find)
	{
		CTexture* pTexture = nullptr;

		pTexture = CTexture::Create(wstrFilePath, wstrStateKey);

		NULL_CHECK_RETURN(pTexture, E_FAIL);
		m_mapTexture.insert({ wstrObjectKey, pTexture });
	}
	else
	{
		// ������Ʈ Ű�� �̹� �����Ѵٸ�.
		HRESULT hr = m_mapTexture[wstrObjectKey]->LoadTexture(
			wstrFilePath, wstrStateKey);
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
