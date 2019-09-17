#pragma once
#include "Texture.h"
class CMultiTexture :
	public CTexture
{
private:
	CMultiTexture();

public:
	virtual ~CMultiTexture();

public:
	// CTexture을(를) 통해 상속됨
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", 
		int iIndex = 0) override;

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", 
		int iImgCount = 0) override;

private:
	virtual void Release() override;

public:
	static CMultiTexture* Create(
		const wstring& wstrFilePath,
		const wstring& wstrStateKey,
		int iImgCount);

private:
	map<wstring, vector<TEX_INFO*>>	m_mapMultiTexture;
};

