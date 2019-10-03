#include "stdafx.h"
#include "AnimationMgr.h"

IMPLEMENT_SINGLETON(CAnimationMgr)
CAnimationMgr::CAnimationMgr()
{
}


CAnimationMgr::~CAnimationMgr()
{
}

const vector<ANIM_CLIP>* CAnimationMgr::GetAnimation(const wstring & clipName)
{
	auto iter_find = m_Clips.find(clipName);

	if (m_Clips.end() == iter_find)
		return nullptr;

	return &iter_find->second;
}

HRESULT CAnimationMgr::LoadAnimation(const wstring & wstrFilePath)
{
	CStdioFile	file;
	CFileException kEx;

	

	return S_OK;
}
