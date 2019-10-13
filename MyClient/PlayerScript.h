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
	CBoxCollider* pBoxCollider;
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

	//타일 체크.
	void CheckTiles();
	//타일 충돌
	bool CollideTiles();
	//계단 올라가기
	bool StepStair();
	void Jump();
	void JumpSetUp(const D3DXVECTOR3& startPos,const D3DXVECTOR3& endPos,const float& endTime);
	//현재 높이 체크
	void CheckLayer();

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
	
	bool		m_bIsDebug=false;	//디버그모드
	bool		m_bIsCollide = false;

	D3DXVECTOR3	 m_PreScale;		//이전 값.
	D3DXVECTOR3*  playerPos;		//플레이어 위치값
	D3DXVECTOR3  m_PrePos;			//이전 위치

	D3DXVECTOR3  m_JumpControlPos;	//컨트롤 위치
	D3DXVECTOR3  m_JumpStartPos;	//점프시작 위치
	D3DXVECTOR3  m_JumpEndPos;		//점프끝 위치
	CBoxCollider* m_playerFoot;		//플레이어 발쪽위치
	float		m_posFootGap;		//플레이어 위치와 발사이의 거리.

	Layer		m_CurLayer;			//최근 층
	Layer		m_PreLayer;			//이전 층
	Layer		m_ChangeLayer;
	CBoxCollider*	playerUpBox;
	CBoxCollider*	playerDownBox;


	float		m_MouseAngle;		//마우스 각도
	float		m_BulletAngle;		//총알 각도
	float		m_MeeleCool;		//근접공격쿨타임.


	//==포물선 점프를 위한 변수들==
	float		m_fJumpSpeedX	= 0.0f;		//점프 x축속도
	float		m_fJumpSpeedY   = 0.0f;		//점프 y축 속도
	float		m_fJumpEndTime  = 0.0f;		//점프 종료시간
	float		m_fJumpGravity  = 0.0f;		//점프 가속도
	float		m_fJumpTime		= 0.0f;		//점프 시간.

	int			m_HeightLevel = 0;			//플레이어 높이.
	

	vector<CGameObject*>		m_NearTiles;
	vector<CGameObject*>		m_CollideTiles;
	vector<COL_DEPTH>			m_Depths;
};

