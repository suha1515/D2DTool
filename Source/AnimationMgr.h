#pragma once
class CAnimationMgr
{
	DECLARE_SINGLETON(CAnimationMgr)
public:
	CAnimationMgr();
	~CAnimationMgr();

public:
	const vector<ANIM_CLIP>*			   GetAnimation(const wstring& animName);
	const map<wstring, vector<ANIM_CLIP>>& GetClip();

public:
	HRESULT	LoadAnimationFromList(const wstring& wstrFilePath);		//txt 파일리스트에서 여러개 불러오기
	HRESULT LoadAnimation(const wstring& wstrFilePath);				//하나의 경로에서 불러오기


private:
	map<wstring,vector<ANIM_CLIP>>		m_Clips;					//클립들을 담을 컨테이너 클립들의 정보는 바뀌면 안된다.

};

