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
		const wstring& wstrStateKey = L"");

public:
	HRESULT LoadTexture(
		const wstring& wstrFilePath,
		const wstring& wstrObjectKey,
		const wstring& wstrStateKey = L"");

public:
	HRESULT RemoveTexture(const wstring& wstrObjectKey, const wstring& wstrStateKey);
	HRESULT RemoveTexture(const wstring& wstrObjectKey);

public:
	HRESULT LoadTextureFromList(const wstring& filePath);


private:
	void Release();

private:
	map<wstring, CTexture*>	m_mapTexture;
};

