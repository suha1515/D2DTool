#pragma once
#include "EffectScript.h"
class CBossFireBreath :
	public CEffectScript
{
public:
	CBossFireBreath();
	~CBossFireBreath();

	// CScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr) override;
	virtual void OnInput() override;
	virtual int OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;

private:
	D3DXVECTOR3			m_ScaleFactor;
	float				m_WhiteValue;
	bool				m_Spawn = false;
	float				m_fRadius;
	float				m_fSpawnTime;
	float				m_fSpawnTimeEnd;
	int					m_FireCount;

	bool				m_bParticle;
	bool				m_bDead;
	bool				m_ParticleDone;
};

