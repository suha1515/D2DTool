#pragma once
#include "Component.h"
class CAnimator :
	public CComponent
{
public:
	CAnimator();
	~CAnimator();

	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize(CGameObject *pObject) override;
	virtual void Action() override;

public:
	bool		Play(const wstring& clipName,ANIMATION_TYPE type);		//�ִϸ��̼� �÷���
	void		Stop();													//�ִϸ��̼� ����
	void		ReSet();												//�ִϸ��̼� ����.
	bool		IsPlaying(const wstring& clipName);						//�ִϸ��̼� �÷��� üũ.
public:
	void		SetClips(const wstring& _animation);


private:
	map < wstring, vector<ANIM_CLIP>>	m_AnimationClip;		//�ִϸ��̼� Ŭ��
	ANIMATION_TYPE						m_AnimationType;		//�ִϸ��̼� Ÿ��
	wstring								m_ClipName;				//Ŭ�� ����
};

