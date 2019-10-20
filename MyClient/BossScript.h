#pragma once
#include "EnemyScripts.h"
class CBossScript :
	public CEnemyScripts
{
	enum STATE {IDLE,DASH,STOMP,GRIND_READY,GRIND,THROWER,HIT};
	enum DIR {RIGHT,UP,DOWN,LEFT, RIGHT_UP_45,LEFT_UP_45,RIGHT_DOWN_45,LEFT_DOWN_45};
	enum TYPE {ICE,FIRE};
public:
	CBossScript();
	~CBossScript();

	// CEnemyScripts��(��) ���� ��ӵ�
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

	void StompSkill();
	void GrindSkill();
private:
	void				TrackPlayer();
	void				GetDirPlayer();

private:
	CKeyMgr*			m_pKeyMgr;
	D3DXVECTOR3			m_DirVec;			//���⺤��
	D3DXVECTOR3			m_PrePos;
	D3DXVECTOR3         m_PrePlayerPos;
	CTransform*			m_PlayerPos;

	DIR					m_CurDir;
	DIR					m_PreDir;

	STATE				m_CurState;
	STATE				m_PreState;
private:
	//������ ������
	D3DXVECTOR3			m_BezierControl[3];
private:
	D3DXVECTOR3			m_SkillPos;
	//��ų���� �Լ���
	// ���� / �� ������ų
	float				m_fIceSkillSpawnCool;
	float				m_fIceSkillTime;
	int					m_iceSkillCount;
	bool				m_bStompSkill = false;

	//��,���� ������
	float				m_fFireGrindSkillSpawnCool;
	float				m_fFireGrindSkillTime;
	int					m_fFireGrindCount;
	bool				m_bGrindSkill = false;

	//�뽬 ��ų

	//��������� ��ų

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
