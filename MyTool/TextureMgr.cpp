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

	// ������Ʈ Ű�� ���ٸ� �̱��̰� ��Ƽ�� ���� �Ҵ� �� �ؽ�ó �ε� �� �� ����.
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
	// ������Ʈ Ű�� �̹� �����ϰ� ��Ƽ���
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
