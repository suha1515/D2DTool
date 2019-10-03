#include "stdafx.h"
#include "Animation.h"


CAnimation::CAnimation()
{
}


CAnimation::~CAnimation()
{
}

vector<ANIM_CLIP>* CAnimation::GetClip(const wstring & wstrClipkey)
{
	auto iter_find = m_mapClipInfo.find(wstrClipkey);
	return &iter_find->second;
}

HRESULT CAnimation::LoadClip(const wstring & wstrFilePath, const wstring & wstrStateKey)
{
	return E_NOTIMPL;
}

HRESULT CAnimation::RemoveClip(const wstring & wstrClipkey)
{
	return E_NOTIMPL;
}

CAnimation * CAnimation::Create(const wstring & wstrFilePath, const wstring & wstrStateKey)
{
	return nullptr;
}
