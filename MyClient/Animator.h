
#ifndef CANIMATOR_H
#define CANIMATOR_H
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
	bool		IsPlaying(const wstring& _clipName=L"");					//�ִϸ��̼� �÷��� üũ.

	void	    SetSpeed(const float& _Speed);
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
	float								m_fSpeed;
	int									m_StartIndex;			//ù�ε���
	int									m_EndIndex;				//������ �ε���

	bool								m_bIsPlaying;			//�÷��� �� ����.

	wstring								m_AnimationName;		//�ִϸ��̼��̸�
public:
	static CAnimator*	Create(CGameObject*pGameObject, const wstring& _clipName, const wstring& _animName);
};

#endif