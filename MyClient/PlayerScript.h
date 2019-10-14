#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CPlayerScript :
	public CScripts
{
	enum STATE { IDLE, RUN_START, RUN, RUN_END,THROW,THROW_END,MEELE,JUMP,AIM_WALK };
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
	void	MoveInput();			//�̵� �Է�Ű
	void	MouseInput();			//���콺 �Է�Ű
	void	MeeleAttack();			//���� ����
	void	Moving();				//�̵�


	void AnimState();
	void DirState();
	void AtkState();


	//���Ÿ� ����
	void AttackBullet();
	//���콺 ����üũ
	void MouseDir();
	//�����Ÿ� Ÿ��üũ
	void CheckRange(D3DXVECTOR3 point, list<CGameObject*>* objlist);
	void CheckRangeCollide();
	bool CheckLineRange(LINE* line,CGameObject* pobject, D3DXVECTOR3* points);

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

	bool		m_bIsJump;
	
	bool		m_bIsDebug=false;			//����׸��
	bool		m_bIsLayerDebug = false;
	bool		m_bIsCollide = false;

	D3DXVECTOR3	 m_PreScale;				//���� ��.
	D3DXVECTOR3*  playerPos;				//�÷��̾� ��ġ��
	D3DXVECTOR3  m_PrePos;					//���� ��ġ

	D3DXVECTOR3  m_JumpControlPos;			//��Ʈ�� ��ġ
	D3DXVECTOR3  m_JumpStartPos;			//�������� ��ġ
	D3DXVECTOR3  m_JumpEndPos;				//������ ��ġ
	CBoxCollider* m_playerFoot;				//�÷��̾� ������ġ
	float		m_posFootGap;				//�÷��̾� ��ġ�� �߻����� �Ÿ�.

	Layer		m_CurLayer;					//�ֱ� ��
	Layer		m_PreLayer;					//���� ��
	Layer		m_ChangeLayer;
	CBoxCollider*	playerUpBox;
	CBoxCollider*	playerDownBox;

	//==���Ÿ� ������ ���� ������ ==
	float		m_MouseAngle;				//���콺 ����
	float		m_BulletAngle;				//�Ѿ� ����
	float		m_MeeleCool;				//����������Ÿ��.

	D3DXVECTOR3 m_GuideLineLeftEndPoint;	//������ ��������
	D3DXVECTOR3 m_GuideLineRightEndPoint;	//������ ��������
	D3DXVECTOR3	m_GuideLineEndPoint;		//������ ����.
	float		m_GuideRange;				//������ �����Ÿ�
	float		m_GuideAngle;				//���� ������ ����
	float		m_LeftGuideAngle;			//���� ���̵� ������ ����
	float		m_LefttempAngle;
	float		m_RightGuideAngle;			//���� ���̵� ������ ����
	float		m_RighttempAngle;

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


	list<CGameObject*>		m_NearTiles;
	vector<CGameObject*>		m_CollideTiles;
	vector<COL_DEPTH>			m_Depths;
};

