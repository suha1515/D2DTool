#pragma once
#include "Component.h"
class CAnimator :
	public CComponent
{
public:
	CAnimator();
	~CAnimator();

	// CComponent을(를) 통해 상속됨
	virtual void Initialize(CGameObject *pObject) override;
	virtual void Action() override;

public:
	bool		Play(const wstring& clipName,ANIMATION_TYPE type);		//애니메이션 플레이
	void		Stop();													//애니메이션 중지
	void		ReSet();												//애니메이션 리셋.
	bool		IsPlaying(const wstring& clipName);						//애니메이션 플레이 체크.
public:
	void		SetClips(const wstring& _animation);


private:
	map < wstring, vector<ANIM_CLIP>>	m_AnimationClip;		//애니메이션 클립
	ANIMATION_TYPE						m_AnimationType;		//애니메이션 타입
	wstring								m_ClipName;				//클립 네임
};

