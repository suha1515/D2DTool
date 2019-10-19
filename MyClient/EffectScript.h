#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CBoxCollider;
class CTextureRenderer;
class CEffectScript :
	public CScripts
{
public:
	 enum  EFFECT_TYPE {MOVABLE,NORMAL};
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
	virtual void SetEffect(const wstring& clips,const ANIMATION_TYPE& type, const float& deadTime);
	void SetType(EFFECT_TYPE type);
	void SetDir(D3DXVECTOR3 dir);
	void SetSpeed(float speed);
protected:
	CAnimator*		  m_pAnimator;
	CTransform*		  m_pTransform;
	CBoxCollider*	  m_pBoxCollider;
	CTextureRenderer* m_pTexture;
	ANIMATION_TYPE    m_AnimType;

	float			  m_fDeadTime;
	float			  m_fTime;

	D3DXVECTOR3		  m_DirVec;
	float			  m_fSpeed;

private:
	EFFECT_TYPE m_EffectType;
	D3DXVECTOR3* m_Pos;
};

