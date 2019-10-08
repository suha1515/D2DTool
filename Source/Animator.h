#pragma once
#include "Component.h"
class CTextureRenderer;
class CTransform;
class CAnimation;
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
	void		Play(const wstring& clipName,ANIMATION_TYPE type);		//애니메이션 플레이
	void		Loop();													//애니메이션 루프
	void		Stop();													//애니메이션 중지
	void		ReSet();												//애니메이션 리셋.
	bool		IsPlaying(const wstring& _clipName);					//애니메이션 플레이 체크.
public:
	HRESULT		SetClips(const wstring& _clipName);
	void		LoadClips(const wstring& _animation);
public:
	wstring		GetAnimationName();


private:
	CTextureRenderer*					m_pTexture;				//텍스처 렌더러 컴포넌트 포인터
	CTransform*							m_pTransform;			//트랜스폼 렌더러 컴포넌트 포인터.

	ANIMATION_TYPE						m_AnimationType;		//애니메이션 타입
	wstring								m_ClipName;				//클립 네임
	CAnimation*							m_pAnimation;			//애니메이션 객체.

	float								m_Time;					//지나간 시간
	int									m_StartIndex;			//첫인덱스
	int									m_EndIndex;				//마지막 인덱스

	bool								m_bIsPlaying;			//플레이 불 변수.

	wstring								m_AnimationName;		//애니메이션이름
	
};

