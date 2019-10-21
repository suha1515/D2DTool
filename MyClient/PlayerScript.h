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
	// CScripts을(를) 통해 상속됨
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
	void	MouseInput();			//마우스 입력키
	void	MoveInput();			//이동 입력키
	void	MeeleAttack();			//근접 공격
	void	Moving();				//이동


	void AnimState();
	void DirState();
	void AtkState();
	void GetHit(D3DXVECTOR3 dirVec, float power, float dmg);

	void FireBreathSkill();
	void IceBulletSkill();

	SKILL	m_SkillType;

	//원거리 공격
	void AttackBullet();
	//마우스 상태체크
	void MouseDir();
	//사정거리 타일체크
	bool CheckRangeCollide(D3DXVECTOR3& originPos,D3DXVECTOR3& guideLine,float * range, D3DXVECTOR3* points=nullptr, D3DXVECTOR3* normal=nullptr);
	bool CheckLineRange(LINE* line, CGameObject* pobject, D3DXVECTOR3* points, float* range, D3DXVECTOR3* _point, D3DXVECTOR3* normal);

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
	//유도선 디버그
	void DrawGuideLine();
	bool		bIsInit = false;
public:
	bool		m_bIsHit = false;						//플레이어 피격시
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

	//플레이어 스킬
	float		m_fSkillCool = 0.0f;
	float		m_fShadowCool = 0.0f;
	int			m_iShadowCount = 0;
	int			m_iFireCount = 0;

	D3DXVECTOR3 m_SkillPos;

	bool		m_bIsJump;
	bool	    m_bIsMove = false;
	bool		m_bIsDebug=false;				//디버그모드
	bool		m_bIsLayerDebug = false;

	bool		m_bIsCollide=false;				//타일의외 충돌검사를위한 불변수

	
	float		m_fHitCool;						//피격쿨타임

	D3DXVECTOR3	 m_PreScale;					//이전 값.
	D3DXVECTOR3*  playerPos;					//플레이어 위치값
	D3DXVECTOR3  m_PrePos;						//이전 위치

	D3DXVECTOR3     m_JumpControlPos;			//컨트롤 위치
	D3DXVECTOR3     m_JumpStartPos;				//점프시작 위치
	D3DXVECTOR3		m_JumpEndPos;				//점프끝 위치
	CBoxCollider*   m_playerFoot;				//플레이어 발쪽위치
	float		    m_posFootGap;				//플레이어 위치와 발사이의 거리.

	Layer		m_CurLayer;						//최근 층
	Layer		m_PreLayer;						//이전 층
	Layer		m_ChangeLayer;
	CBoxCollider*	playerUpBox;
	CBoxCollider*	playerDownBox;

	//==근거리 공격을 위한 변수들 ==
	int			m_ComboCount;				//연속기를 위한 카운트.

	//==원거리 공격을 위한 변수들 ==
	float		m_MouseAngle;				//마우스 각도
	float		m_BulletAngle;				//총알 각도
	float		m_MeeleCool;				//근접공격쿨타임.

	D3DXVECTOR3 m_GuideLineLeftEndPoint;	//유도선 좌측끝점
	D3DXVECTOR3 m_GuideLineRightEndPoint;	//유도선 우측끝점
	D3DXVECTOR3	m_GuideLineEndPoint;		//유도선 끝점.

	float		m_LeftGuideRange;			//좌측선 사정거리
	float		m_RightGuideRange;			//우측선 사정거리
	float		m_GuideRange;				//유도선 사정거리
	float		m_GuideAngle;				//메인 유도선 각도
	float		m_LeftGuideAngle;			//좌측 사이드 유도선 각도
	float		m_LefttempAngle;
	float		m_RightGuideAngle;			//우측 사이드 유도선 각도
	float		m_RighttempAngle;
	bool		m_bIsHitSomething = false;


	vector< pair<D3DXVECTOR3, D3DXVECTOR3>>	m_ChargeLine;

	list<CGameObject*>m_MidPointCollide;	//가운데 포인트 충돌체 검사 컨테이너
	list<CGameObject*>m_LeftPointCollide;	//좌측 포인트 충돌체검사 컨테이너
	list<CGameObject*>m_RightPointCollide;  //우측 포인트 충돌체 검사 컨테이너


	bool		m_bIsCharging = false;		//차징을 위한 불변수
	bool		m_bIsCharged = false;		//차지 완료
	bool		m_bIsTimetoCancle = false;	//차지 취소 시작.
	float		m_ChargeTime = 0.0f;		//차지 시간
	float		m_ChargeCancle=0.0f;		//차지 취소시간( 이시간안에 누르지 않을경우 차지 취소)
	float		m_TimeToCancle = 0.0f;		//몇초동안 안누르면 취소가 시작된다.

	//차지샷

	//==포물선 점프를 위한 변수들==
	float		m_fJumpSpeedX	= 0.0f;		//점프 x축속도
	float		m_fJumpSpeedY   = 0.0f;		//점프 y축 속도
	float		m_fJumpEndTime  = 0.0f;		//점프 종료시간
	float		m_fJumpGravity  = 0.0f;		//점프 가속도
	float		m_fJumpTime		= 0.0f;		//점프 시간.

	int			m_HeightLevel = 0;			//플레이어 높이.


	//플레이어 기타 정보
	float		m_fHp;

	list<CGameObject*>			m_NearTiles;
	vector<CGameObject*>		m_CollideTiles;
	vector<COL_DEPTH>			m_Depths;
};

