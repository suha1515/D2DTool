#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CEffectScript :
	public CScripts
{
public:
	CEffectScript();
	~CEffectScript();

	// CScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr) override;
	virtual void OnInput() override;
	virtual int  OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;
public:
	void SetEffect(const wstring& clips,const ANIMATION_TYPE& type, const float& deadTime);
private:
	CAnimator*		  m_pAnimator;
	CTransform*		  m_pTransform;
	ANIMATION_TYPE    m_AnimType;

	float			  m_fDeadTime;
	float			  m_fTime;
};

