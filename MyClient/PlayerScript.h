#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CPlayerScript :
	public CScripts
{
	enum STATE { IDLE, RUN_START, RUN, RUN_END,THROW,THROW_END,MEELE,MEELE_CIRCLE,JUMP,AIM,AIM_WALK,HIT,
				  SKILL_CHARGE,SKILL_USE};
	enum DIR {
		UP, RIGHT_UP_45, RIGHT, RIGHT_DOWN_45, DOWN,
		LEFT_UP_45, LEFT, LEFT_DOWN_45
	};
	enum AttackDir{RIGHT_ATK,LEFT_ATK};
	enum SKILL{NONE,FIRE,ICE};
public:
	CPlayerScript();
	~CPlayerScript();
	// CScripts��(��) ���� ��ӵ�
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2* move = nullptr) override;
	virtual void OnInput() override;
	virtual int OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;

private:
	CTransform* pTransform;
	CAnimator*	pAnimator;
	CBoxCollider* pBoxCollider;
	CKeyMgr*	pKeyMgr;

public:
	void	MouseInput();			//���콺 �Է�Ű
	void	MoveInput();			//�̵� �Է�Ű
	void	MeeleAttack();			//���� ����
	void	Moving();				//�̵�


	void AnimState();
	void DirState();
	void AtkState();
	void GetHit(D3DXVECTOR3 dirVec, float power, float dmg);

	void FireBreathSkill();
	void IceBulletSkill();

	SKILL	m_SkillType;

	//���Ÿ� ����
	void AttackBullet();
	//���콺 ����üũ
	void MouseDir();
	//�����Ÿ� Ÿ��üũ
	bool CheckRangeCollide(D3DXVECTOR3& originPos,D3DXVECTOR3& guideLine,float * range, D3DXVECTOR3* points=nullptr, D3DXVECTOR3* normal=nullptr);
	bool CheckLineRange(LINE* line, CGameObject* pobject, D3DXVECTOR3* points, float* range, D3DXVECTOR3* _point, D3DXVECTOR3* normal);

	//Ÿ�� üũ.
	void CheckTiles();
	//Ÿ�� �浹
	bool CollideTiles();
	//��� �ö󰡱�
	bool StepStair();
	void Jump();
	void JumpSetUp(const D3DXVECTOR3& startPos,const D3DXVECTOR3& endPos,const float& endTime);
	//���� ���� üũ
	void CheckLayer();
	//������ �����
	void DrawGuideLine();
	bool		bIsInit = false;
public:
	bool		m_bIsHit = false;						//�÷��̾� �ǰݽ�
private:
	DIR			m_CurDir;
	DIR			m_PreDir;

	DIR			m_CurMoveDir;
	DIR			m_PreMoveDir;

	D3DXVECTOR3   m_AttackPos;
	float		  m_AttackAngle;

	D3DXVECTOR3 m_DirVec;
	D3DXVECTOR3 m_PreVec;

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

	//�÷��̾� ��ų
	float		m_fSkillCool = 0.0f;
	float		m_fShadowCool = 0.0f;
	int			m_iShadowCount = 0;
	int			m_iFireCount = 0;

	D3DXVECTOR3 m_SkillPos;

	bool		m_bIsJump;
	bool	    m_bIsMove = false;
	bool		m_bIsDebug=false;				//����׸��
	bool		m_bIsLayerDebug = false;

	bool		m_bIsCollide=false;				//Ÿ���ǿ� �浹�˻縦���� �Һ���

	
	float		m_fHitCool;						//�ǰ���Ÿ��

	D3DXVECTOR3	 m_PreScale;					//���� ��.
	D3DXVECTOR3*  playerPos;					//�÷��̾� ��ġ��
	D3DXVECTOR3  m_PrePos;						//���� ��ġ

	D3DXVECTOR3     m_JumpControlPos;			//��Ʈ�� ��ġ
	D3DXVECTOR3     m_JumpStartPos;				//�������� ��ġ
	D3DXVECTOR3		m_JumpEndPos;				//������ ��ġ
	CBoxCollider*   m_playerFoot;				//�÷��̾� ������ġ
	float		    m_posFootGap;				//�÷��̾� ��ġ�� �߻����� �Ÿ�.

	Layer		m_CurLayer;						//�ֱ� ��
	Layer		m_PreLayer;						//���� ��
	Layer		m_ChangeLayer;
	CBoxCollider*	playerUpBox;
	CBoxCollider*	playerDownBox;

	//==�ٰŸ� ������ ���� ������ ==
	int			m_ComboCount;				//���ӱ⸦ ���� ī��Ʈ.

	//==���Ÿ� ������ ���� ������ ==
	float		m_MouseAngle;				//���콺 ����
	float		m_BulletAngle;				//�Ѿ� ����
	float		m_MeeleCool;				//����������Ÿ��.

	D3DXVECTOR3 m_GuideLineLeftEndPoint;	//������ ��������
	D3DXVECTOR3 m_GuideLineRightEndPoint;	//������ ��������
	D3DXVECTOR3	m_GuideLineEndPoint;		//������ ����.

	float		m_LeftGuideRange;			//������ �����Ÿ�
	float		m_RightGuideRange;			//������ �����Ÿ�
	float		m_GuideRange;				//������ �����Ÿ�
	float		m_GuideAngle;				//���� ������ ����
	float		m_LeftGuideAngle;			//���� ���̵� ������ ����
	float		m_LefttempAngle;
	float		m_RightGuideAngle;			//���� ���̵� ������ ����
	float		m_RighttempAngle;
	bool		m_bIsHitSomething = false;


	vector< pair<D3DXVECTOR3, D3DXVECTOR3>>	m_ChargeLine;

	list<CGameObject*>m_MidPointCollide;	//��� ����Ʈ �浹ü �˻� �����̳�
	list<CGameObject*>m_LeftPointCollide;	//���� ����Ʈ �浹ü�˻� �����̳�
	list<CGameObject*>m_RightPointCollide;  //���� ����Ʈ �浹ü �˻� �����̳�


	bool		m_bIsCharging = false;		//��¡�� ���� �Һ���
	bool		m_bIsCharged = false;		//���� �Ϸ�
	bool		m_bIsTimetoCancle = false;	//���� ��� ����.
	float		m_ChargeTime = 0.0f;		//���� �ð�
	float		m_ChargeCancle=0.0f;		//���� ��ҽð�( �̽ð��ȿ� ������ ������� ���� ���)
	float		m_TimeToCancle = 0.0f;		//���ʵ��� �ȴ����� ��Ұ� ���۵ȴ�.

	//������

	//==������ ������ ���� ������==
	float		m_fJumpSpeedX	= 0.0f;		//���� x��ӵ�
	float		m_fJumpSpeedY   = 0.0f;		//���� y�� �ӵ�
	float		m_fJumpEndTime  = 0.0f;		//���� ����ð�
	float		m_fJumpGravity  = 0.0f;		//���� ���ӵ�
	float		m_fJumpTime		= 0.0f;		//���� �ð�.

	int			m_HeightLevel = 0;			//�÷��̾� ����.


	//�÷��̾� ��Ÿ ����
	float		m_fHp;

	list<CGameObject*>			m_NearTiles;
	vector<CGameObject*>		m_CollideTiles;
	vector<COL_DEPTH>			m_Depths;
};

