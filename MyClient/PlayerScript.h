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
	// CScripts��(��) ���� ��ӵ�
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
	void	MoveInput();			//�̵� �Է�Ű
	void	MouseInput();			//���콺 �Է�Ű
	void	MeeleAttack();			//���� ����
	void	Moving();				//�̵�


	void AnimState();
	void DirState();
	void AtkState();

	//���Ÿ� ����
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
	
	bool		m_bIsDebug=false;	//����׸��

	D3DXVECTOR3	 m_PreScale;		//���� ��.
	D3DXVECTOR3*  playerPos;		//�÷��̾� ��ġ��

	float		m_MouseAngle;		//���콺 ����
	float		m_BulletAngle;		//�Ѿ� ����
	float		m_MeeleCool;		//����������Ÿ��.
};

