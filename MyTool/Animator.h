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

	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize(CGameObject *pObject) override;
	virtual void Action() override;

public:
	void		Play(const wstring& clipName,ANIMATION_TYPE type);		//�ִϸ��̼� �÷���
	void		Loop();													//�ִϸ��̼� ����
	void		Stop();													//�ִϸ��̼� ����
	void		ReSet();												//�ִϸ��̼� ����.
	bool		IsPlaying(const wstring& _clipName);					//�ִϸ��̼� �÷��� üũ.
public:
	HRESULT		SetClips(const wstring& _clipName);
	void		LoadClips(const wstring& _animation);
public:
	wstring		GetAnimationName();


private:
	CTextureRenderer*					m_pTexture;				//�ؽ�ó ������ ������Ʈ ������
	CTransform*							m_pTransform;			//Ʈ������ ������ ������Ʈ ������.

	ANIMATION_TYPE						m_AnimationType;		//�ִϸ��̼� Ÿ��
	wstring								m_ClipName;				//Ŭ�� ����
	CAnimation*							m_pAnimation;			//�ִϸ��̼� ��ü.

	float								m_Time;					//������ �ð�
	int									m_StartIndex;			//ù�ε���
	int									m_EndIndex;				//������ �ε���

	bool								m_bIsPlaying;			//�÷��� �� ����.

	wstring								m_AnimationName;		//�ִϸ��̼��̸�
	
};

