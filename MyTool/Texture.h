#pragma once
class CTexture
{
protected:
	CTexture();

public:
	virtual ~CTexture();

public:
	TEX_INFO*	GetTexInfo(const wstring& wstrStateKey = L"");

public:
	HRESULT LoadTexture(const wstring& wstrFilePath,
						const wstring& wstrStateKey = L"");
public:
	HRESULT RemoveTexture(const wstring& wstrStateKey);
public:
	static CTexture* Create(
		const wstring& wstrFilePath,
		const wstring& wstrStateKey);
private:
	void Release();

private:
	//TEX_INFO*	m_pTexInfo;
	CDeviceMgr*	m_pDeviceMgr;
	map<wstring, TEX_INFO*> m_mapTexInfo;
};

