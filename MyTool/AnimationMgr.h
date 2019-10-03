#pragma once
class CAnimationMgr
{
	DECLARE_SINGLETON(CAnimationMgr)
public:
	CAnimationMgr();
	~CAnimationMgr();

public:
	const vector<ANIM_CLIP>*	GetAnimation(const wstring& clipName);

public:
	HRESULT LoadAnimation(const wstring& wstrFilePath);


private:
	map<wstring, vector<ANIM_CLIP>>		m_Clips;

};

