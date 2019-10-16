#pragma once
#include "EnemyScripts.h"
class CNormalBotScript :
	public CEnemyScripts
{
	enum STATE { IDLE, HIT,MOVE, ATTACK_READY,ATTACK};
public:
	CNormalBotScript();
	~CNormalBotScript();

	// CEnemyScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr) override;
	virtual void OnInput() override;
	virtual int OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;
	//========================================================
	virtual void DirState() override;
	virtual void AnimState() override;
	virtual void Move() override;
	virtual void GetHit(D3DXVECTOR3 dirVec, float power, float dmg) override;
	

private:
	void				TrackPlayer();
	void				GetDirPlayer();
	void				AttackState();
	void				Dash();
private:
	D3DXVECTOR3			m_DirVec;			//방향벡터

	DIR					m_CurDir;
	DIR					m_PreDir;

	STATE				m_CurState;
	STATE				m_PreState;
private:
	D3DXVECTOR3*		m_Pos;
	CGameObject*		m_pPlayer;
	float				m_fAngle;
};

