#pragma once
#include "EnemyScripts.h"
class CTransform;
class CAnimator;
class CMouseBotScript :
	public CEnemyScripts
{
	enum STATE{IDLE,HIT,MOVE};
public:
	CMouseBotScript();
	~CMouseBotScript();

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


	// CEnemyScripts을(를) 통해 상속됨
	virtual void DirState() override;
	virtual void AnimState() override;
	virtual void GetHit(D3DXVECTOR3 dirVec, float power, float dmg) override;
	virtual void Move() override;

public:
	void				RandomMove();

private:
	D3DXVECTOR3			m_DirVec;			//방향벡터

	DIR					m_CurDir;
	DIR					m_PreDir;

	STATE				m_CurState;
	STATE				m_PreState;

	float				m_fRandomMoveTime;
	float				m_fWaitTime;
	float				m_fTime;

private:
	D3DXVECTOR3*		m_Pos;
	float				m_fRaidus;
	


};

