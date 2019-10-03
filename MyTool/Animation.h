#pragma once
class CAnimation
{
public:
	CAnimation();
	~CAnimation();

public:
	vector<ANIM_CLIP>*					GetClip(const wstring& wstrClipkey);
public:
	HRESULT								LoadClip(const wstring& wstrFilePath,
												 const wstring&	wstrStateKey=L"");
public:
	HRESULT								RemoveClip(const wstring& wstrClipkey);
public:
	static	CAnimation*					Create(const wstring&	wstrFilePath,
											   const wstring&	wstrStateKey);

private:
	map<wstring, vector<ANIM_CLIP>>		m_mapClipInfo;
};

