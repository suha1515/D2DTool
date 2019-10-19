#pragma once
#include "EnemyScripts.h"
class CBossScript :
	public CEnemyScripts
{
	enum STATE {IDLE,DASH,STOMP,GRIND,THROWER,HIT};
	enum DIR {RIGHT,UP,DOWN,LEFT, RIGHT_UP_45,LEFT_UP_45,RIGHT_DOWN_45,LEFT_DOWN_45};
public:
	CBossScript();
	~CBossScript();

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
	virtual void DirState() override;
	virtual void AnimState() override;
	virtual void Move() override;
	virtual void GetHit(D3DXVECTOR3 dirVec, float power, float dmg) override;

	void	MakeIceSkillRoute();


	void Hit();
private:
	void				TrackPlayer();
	void				GetDirPlayer();

private:
	CKeyMgr*			m_pKeyMgr;
	D3DXVECTOR3			m_DirVec;			//방향벡터
	D3DXVECTOR3			m_PrePos;
	D3DXVECTOR3         m_PrePlayerPos;
	CTransform*			m_PlayerPos;

	DIR					m_CurDir;
	DIR					m_PreDir;

	STATE				m_CurState;
	STATE				m_PreState;
private:
	D3DXVECTOR3			m_BezierControl[3];
private:
	float				m_fIceSkillSpawnCool;
	float				m_fIceSkillTime;
	int					m_iceSkillCount;
	bool				m_bIceSkill = false;
private:
	D3DXVECTOR3*		m_Pos;
	CGameObject*		m_pPlayer;
	float				m_fAngle;
};

