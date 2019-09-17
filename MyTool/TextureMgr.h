#pragma once

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

public:
	enum TEXTURE_TYPE { SINGLE_TEXTURE, MULTI_TEXTURE };

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEX_INFO* GetTexInfo(
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"",
		int iIndex = 0);

public:
	HRESULT LoadTexture(
		TEXTURE_TYPE eTexType,
		const wstring& wstrFilePath,
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"",
		int iImgCount = 0);

private:
	void Release();

private:
	map<wstring, CTexture*>	m_mapTexture;
};

