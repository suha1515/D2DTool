#pragma once
#include "EffectScript.h"
class CPlayerIceSkill :
	public CEffectScript
{
	enum STATE {SPREAD,AIM,SHOOT};
public:
	CPlayerIceSkill();
	~CPlayerIceSkill();

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

public:
	void    State();
	void	SetTarget();
	void	MakeBullet();
private:
	D3DXVECTOR3			m_ScaleFactor;
	float				m_WhiteValue;
	bool				m_Spawn = false;
	float				m_fRadius;
	float				m_fSpawnTime;
	float				m_fSpawnTimeEnd;

	float				m_fRotateTime;
	float				m_fAngle;
	int					m_FireCount;

	bool				m_bParticle;
	bool				m_bDead;
	bool				m_ParticleDone;

	CGameObject*		m_pPlayer;
	CGameObject*		m_pTarget;

	D3DXVECTOR3*		playerPos;
	D3DXVECTOR3*		effectPos;	
	D3DXVECTOR3*		m_pBossTarget;
	D3DXVECTOR3			m_DirVec;

	vector<pair<CGameObject*, float>> bullet;

	float				m_fSpeed;
	float				m_fAcc;

	STATE				m_CurState;
	STATE				m_PreState;
};

