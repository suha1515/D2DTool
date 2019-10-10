#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CPlayerScript :
	public CScripts
{
	enum STATE { IDLE, RUN_START, RUN, RUN_END,THROW,THROW_END,MEELE };
	enum DIR {
		UP, RIGHT_UP_45, RIGHT, RIGHT_DOWN_45, DOWN,
		LEFT_UP_45, LEFT, LEFT_DOWN_45
	};
	enum AttackDir{RIGHT_ATK,LEFT_ATK};
public:
	CPlayerScript();
	~CPlayerScript();
	// CScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2* move = nullptr) override;
	virtual void OnInput() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;

private:
	CTransform* pTransform;
	CAnimator*	pAnimator;
	CKeyMgr*	pKeyMgr;

public:
	void	MoveInput();			//이동 입력키
	void	MouseInput();			//마우스 입력키
	void	MeeleAttack();			//근접 공격
	void	Moving();				//이동


	void AnimState();
	void DirState();
	void AtkState();

	//원거리 공격
	void AttackBullet();

	bool		bIsInit = false;

private:
	DIR			m_CurDir;
	DIR			m_PreDir;

	DIR			m_CurMoveDir;
	DIR			m_PreMoveDir;

	STATE		m_PreState;
	STATE		m_CurState;

	AttackDir   m_AtkDir;

	float		m_fVelocity;
	float		m_fAcc;

	bool		m_Right;
	bool		m_Left;
	bool		m_Down;
	bool		m_Up;
	bool		m_bIsThrow;
	
	bool		m_bIsDebug=false;	//디버그모드

	D3DXVECTOR3	 m_PreScale;		//이전 값.
	D3DXVECTOR3*  playerPos;		//플레이어 위치값

	float		m_MouseAngle;		//마우스 각도
	float		m_BulletAngle;		//총알 각도
	float		m_MeeleCool;		//근접공격쿨타임.
};

