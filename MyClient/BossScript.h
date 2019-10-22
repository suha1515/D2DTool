#pragma once
#include "EnemyScripts.h"
class CBossScript :
	public CEnemyScripts
{
	enum STATE {IDLE,DASH_READY,MOVE,DASH,DASH_STOP,STOMP,GRIND_READY,GRIND,THROWER,HIT,DEAD};
	enum PHASE {NONE,PHASE_IN,PHASE1,PHASE2,PHASE3};
	enum DIR   {RIGHT,UP,DOWN,LEFT, RIGHT_UP_45,LEFT_UP_45,RIGHT_DOWN_45,LEFT_DOWN_45};
	enum TYPE  {NO,ICE,FIRE};
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

	void AttackState();
	void Hit();

	void SetPhaseIn();
	void PhaseIn();

	void StompSkill();
	void GrindSkill();
	void ThrowerSkill();
	void DashSkill();
	void DashReady();
	void DeadEffect();

	void PhaseState();
	void PhaseStae2();
private:
	void				TrackPlayer();
	void				GetDirPlayer();

private:
	float				m_HitCool = 0.0f;

	CKeyMgr*			m_pKeyMgr;
	D3DXVECTOR3			m_DirVec;			//방향벡터
	D3DXVECTOR3			m_PrePos;
	D3DXVECTOR3         m_PrePlayerPos;
	CTransform*			m_PlayerPos;

	DIR					m_CurDir;
	DIR					m_PreDir;

	PHASE				m_CurPhase;
	PHASE				m_PrePhase;

	STATE				m_CurState;
	STATE				m_PreState;
private:
	//베지어 제어점
	D3DXVECTOR3			m_BezierControl[3];
private:
	float				m_fWaitTime = 0.0f;
	int					m_EventCount = 0;
	bool				m_bIsPhaseIn = false;

	int					m_iPhase1Count = 0;
	int					m_PreiPhase1Count = 0;
	float				m_fPhase1GapTime = 0.0f;
	int					m_iPhase2Count = 0;
	float				m_fPhase2GapTime = 0.0f;

	D3DXVECTOR3			m_SkillPos;

	bool				m_MoveInit = false;
	D3DXVECTOR3			m_MovePos;
	float				m_fMoveTIme=0.0f;
	float				m_fMoveTotal = 0.0f;
	//스킬관련 함수들
	// 얼음 / 불 추적스킬
	float				m_fIceSkillSpawnCool;
	float				m_fIceSkillTime;
	int					m_iceSkillCount;
	bool				m_bStompSkill = false;

	//불,얼음 던지기
	float				m_fFireGrindSkillSpawnCool;
	float				m_fFireGrindSkillTime;
	int					m_fFireGrindCount;
	bool				m_bGrindSkill = false;

	//대쉬 준비
	float			   m_fDashReadyTime=0.0f;
	float			   m_fFlickerTime=0.0f;
	int				   m_iFlickerCount=0;

	//대쉬 스킬
	float			    m_fShadowSpawnTime;
	float			    m_fDashTime;
	bool			    m_bDashSkill = false;
	D3DXVECTOR3			m_tempPos;

	//돌내려찍기 스킬
	float			   m_fRockSpawnTime;
	float			   m_fRadius;
	int				   m_iRockCount;
	bool			   m_bRockSkill = false;
	bool			   m_bDustOnce = false;

	//죽는이펙트
	float			   m_fDeadEffTime=0.0f;
	float			   m_fExploSpawTime=0.0f;
	bool			   m_bIsDead = false;
	bool			   m_bIsDeadEffect = false;
	bool			   m_bExplosive = false;
	float			   m_fDeadAlpha;
	float			   m_fDeadRestTime;
	D3DXVECTOR3		   m_DeadColor;


private:
	D3DXVECTOR3*		m_Pos;
	CGameObject*		m_pPlayer;
	D3DXVECTOR3*		playerPos;
	float				m_fAngle;


private:
	float				m_fHitCoolTime;
	bool				m_bIsHit;

	TYPE				m_Type;
};

