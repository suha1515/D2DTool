#include "stdafx.h"
#include "PlayerScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Animator.h"
#include "Camera.h"
#include "BulletScript.h"
#include "Mouse.h"

CPlayerScript::CPlayerScript()
{
	m_ScriptName = "CPlayerScript";
}


CPlayerScript::~CPlayerScript()
{
	delete m_playerFoot;
}

//라이프 사이클.
void CPlayerScript::OnInit()
{
	cout << "플레이어 초기화" << endl;
	//키매니저및 오브젝트 컴포넌트 포인터 가져오기
	pKeyMgr = CKeyMgr::GetInstance();
	pAnimator = m_pGameObject->GetComponent<CAnimator>();
	pTransform = m_pGameObject->GetComponent<CTransform>();
	pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	pBoxCollider->SetCollideColor(D3DCOLOR_XRGB(164, 73, 164));

	
	//초기 방향설정
	m_CurDir = DOWN;
	m_PreDir = m_CurDir;

	//초기 공격방향설정 (플레이어는 왼쪽오른쪽팔을 휘두른다.)
	m_AtkDir = RIGHT_ATK;

	//초기 애니메이션 상태 설정
	m_CurState = IDLE;
	m_PreState = m_CurState;

	//애니메이터가 없을경우 경고
	if (nullptr == pAnimator)
		MessageBox(0, L"애니메이터 없음", L"Error", 0);

	//카메라가 플레이어를 따라오도록
	CCameraMgr::GetInstance()->GetMainCamera()->Follow(m_pGameObject);

	//초기속도와 가속도
	m_fVelocity = 0.0f;
	m_fAcc = 0.0f;
	//초기화를 하기위한 불변수.
	bIsInit = true;

	//초기 클립세트 설정
	pAnimator->Play(L"Player_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);

	m_Right = false;
	m_Left = false;
	m_Down = true;
	m_Up = false;

	playerPos = &pTransform->GetLocalPosition();

	//초기층 설정
	int layer = m_pGameObject->GetObjectLayer() - 1;

	m_CurLayer = (Layer)layer;
	m_PreLayer = m_CurLayer;

	//플레이어 발렉트설정
	m_playerFoot = new CBoxCollider;
	m_playerFoot->Initialize(m_pGameObject);
	m_playerFoot->SetBoxOffset( 0.0f, - 16.f);
	m_playerFoot->SetBoxSize(7.f, 5.f);
	m_playerFoot->SetBoxCollider();
	m_playerFoot->SetCollideColor(D3DCOLOR_XRGB(164, 73, 164));

	playerUpBox = pBoxCollider;
	playerDownBox = m_playerFoot;

	//유도선설정
	m_GuideRange = 200.f;

}


void CPlayerScript::OnEnable()
{
}

void CPlayerScript::OnCollision(CGameObject * pGameObject, XMFLOAT2* move)
{
	if (nullptr != pGameObject)
	{

	}
}

void CPlayerScript::OnInput()
{
}

int CPlayerScript::OnUpdate()
{
	if (!bIsInit)
		OnInit();
	CTransform* pTransform = m_pGameObject->GetComponent<CTransform>();
	if (pTransform != nullptr)
	{
		m_PrePos = *playerPos;
		//타일확인
		CheckTiles();
		//레이어 확인
		CheckLayer();
		
		if (!m_bIsJump)
		{
			//마우스 입력
			MouseInput();
			//방향키 입력
			MoveInput();
			//근접공격 입력
			MeeleAttack();
			//방향 변화.
			DirState();
			//공격 상태 변화
			AtkState();
		}
		//이동 애니메이션 상태변화
		AnimState();

		//이동
		Moving();
	}

	m_Right = false;
	m_Left = false;
	m_Down = false;
	m_Up = false;

	
	//타일제거
	m_NearTiles.clear();

	m_playerFoot->SetBoxCollider();

	m_LeftPointCollide.clear();
	m_RightPointCollide.clear();
	m_MidPointCollide.clear();
	return NO_EVENT;
}

void CPlayerScript::OnLateUpdate()
{
}

void CPlayerScript::OnRender()
{
	if (m_bIsDebug)
	{
		m_playerFoot->DrawCollide();
		DrawGuideLine();
	}
	m_ChargeLine.clear();
}

void CPlayerScript::OnDisable()
{
}

void CPlayerScript::OnDestroy()
{
}

void CPlayerScript::MoveInput()
{
	//어떤 키든 눌렀을경우 (방향키)
	if (m_CurState != MEELE)
	{
		if (pKeyMgr->KeyPressing(KEY_LEFT) || pKeyMgr->KeyPressing(KEY_RIGHT)
			|| pKeyMgr->KeyPressing(KEY_UP) || pKeyMgr->KeyPressing(KEY_DOWN))
		{
			//cout << "키눌렀다" << endl;
			m_fVelocity += (1.5f*powf(m_fAcc, 2.0f));						//가속도 공식 조절할 필요가있음
			m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime();

			m_fAcc = __min(3.0f, m_fAcc);
			m_fVelocity = __min(7.f, m_fVelocity);
			if (m_CurState == IDLE || m_CurState == RUN_END)
				m_CurState = RUN_START;
			else if (m_CurState == RUN_START && !pAnimator->IsPlaying())
			{
				if (m_CurState != THROW_END && !pAnimator->IsPlaying())
					m_CurState = RUN;
			}
			else if (m_CurState == AIM)
				m_CurState = AIM_WALK;
		}
	}
	//방향 키를 놨경우 (방향키) 모두!
	if (!pKeyMgr->KeyPressing(KEY_LEFT) && !pKeyMgr->KeyPressing(KEY_RIGHT)
		&& !pKeyMgr->KeyPressing(KEY_UP) && !pKeyMgr->KeyPressing(KEY_DOWN))
	{
		m_fVelocity -= (2.0f*powf(m_fAcc, 2.0f));
		m_fAcc -= CTimeMgr::GetInstance()->GetDeltaTime();

		m_fAcc = __max(0.5f, m_fAcc);
		m_fVelocity = __max(0.f, m_fVelocity);

		if (m_CurState == RUN || m_CurState == RUN_START)
			m_CurState = RUN_END;
		else if (m_CurState == RUN_END && !pAnimator->IsPlaying())
			m_CurState = IDLE;
		else if (m_CurState == AIM_WALK)
			m_CurState = AIM;
	}
	//근접공격중에는 움직이지 않는다
	if (m_CurState != MEELE)
	{
		if (pKeyMgr->KeyPressing(KEY_LEFT))
		{
			m_Left = true;
		}
		else if (pKeyMgr->KeyPressing(KEY_RIGHT))
		{
			m_Right = true;
		}
		if (pKeyMgr->KeyPressing(KEY_UP))
		{
			m_Up = true;
		}
		else if (pKeyMgr->KeyPressing(KEY_DOWN))
		{
			m_Down = true;
		}
		//던지기 모션중에는 방향을 바꾸지 않는다.
		if (!m_bIsThrow&&!m_bIsCharged)
		{
			if (m_Right)
				m_CurDir = RIGHT;
			else if (m_Left)
				m_CurDir = LEFT;
			if (m_Up)
				m_CurDir = UP;
			else if (m_Down)
				m_CurDir = DOWN;
			if (m_Right&&m_Up)
				m_CurDir = RIGHT_UP_45;
			else if (m_Right&&m_Down)
				m_CurDir = RIGHT_DOWN_45;
			else if (m_Left&&m_Up)
				m_CurDir = LEFT_UP_45;
			else if (m_Left&&m_Down)
				m_CurDir = LEFT_DOWN_45;
		}
		if (m_Right)
			m_CurMoveDir = RIGHT;
		else if (m_Left)
			m_CurMoveDir = LEFT;
		if (m_Up)
			m_CurMoveDir = UP;
		else if (m_Down)
			m_CurMoveDir = DOWN;
		if (m_Right&&m_Up)
			m_CurMoveDir = RIGHT_UP_45;
		else if (m_Right&&m_Down)
			m_CurMoveDir = RIGHT_DOWN_45;
		else if (m_Left&&m_Up)
			m_CurMoveDir = LEFT_UP_45;
		else if (m_Left&&m_Down)
			m_CurMoveDir = LEFT_DOWN_45;
	}
	else
		m_fVelocity = 0.0f;

	//디버그모드
	if (pKeyMgr->KeyDown(KEY_P))
	{
		if (!m_bIsDebug)
		{
			CObjectMgr::GetInstance()->SetDebug(true);
			m_bIsDebug = true;
		}
		else
		{
			CObjectMgr::GetInstance()->SetDebug(false);
			m_bIsDebug = false;
		}
	}
	if (pKeyMgr->KeyDown(KEY_O))
	{
		if (!m_bIsLayerDebug)
		{
			CObjectMgr::GetInstance()->SetLayerDebug(true);
			m_bIsLayerDebug = true;
		}
		else
		{
			CObjectMgr::GetInstance()->SetLayerDebug(false);
			m_bIsLayerDebug = false;
		}
	}
	m_Right = false;
	m_Left = false;
	m_Down = false;
	m_Up = false;
}

void CPlayerScript::MouseInput()
{
	D3DXVECTOR3 mousePos = pKeyMgr->GetMouse()->GetMousePos();
	m_BulletAngle = GetAngle(*playerPos, mousePos);
	if (pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		cout << "던지기!" << endl;
		
		MouseDir();

		m_CurState = THROW;
		m_bIsThrow = true;
	}
	if (pKeyMgr->KeyPressing(KEY_LBUTTON))
	{
		if (m_bIsCharged)
		{
			cout << "차지됨" << endl;
			//m_CurState = AIM_WALK;
			MouseDir();

			m_CurState = AIM;

			////남은거리만큼 다시 라인을 체크한다 다만 반사벡터를 고려한다.
			//D3DXVECTOR3 originPos = *playerPos;
			//D3DXVECTOR3 otherPoints = m_GuideLineEndPoint;				//충돌면에 찍힌 점.
			//D3DXVECTOR3 resultPoint;
			//D3DXVECTOR3 normalVec;										//해당충돌면의 노말벡터
			//float		range;
			//float leftRange = 200.0f;
			//while (CheckRangeCollide(originPos, otherPoints, &range, &resultPoint, &normalVec))
			//{
			//	m_ChargeLine.push_back({ originPos, resultPoint });   //해당 점들을 벡터에 넣어서 따로 출력준비를한다.
			//	D3DXVECTOR3 reflect;										//새로만들어진 반사벡터
			//	D3DXVECTOR3 dirVec;											//반사된 방향 벡터
			//	leftRange -= range;
			//	//남은 거리가 있을경우 (차지샷의 경우 가이드라인을 보여줘야한다)
			//	if (leftRange > 0.0f)
			//	{
			//		dirVec = resultPoint - originPos;					//처음시작한 점으로부터 충돌면 점까지의 방향을구한다.
			//		D3DXVec3Normalize(&dirVec, &dirVec);				//정규화하여 방향만 남긴다
			//		reflect = GetReflectVector(&dirVec, &normalVec);	//해당방향과 노말벡터를이용하여 반사벡터를 구한다.
			//		reflect = reflect* leftRange;						//해당 방향으로 x,y축으로의 남은 거리만큼 곱해주면 남은 벡터완성.
			//		originPos = resultPoint;							//처음 포지션은 남은방향이있다면 부딪힌 지점의 점이되고
			//		otherPoints = reflect;								//다른방향의 점은 반사된 벡터가 된다.
			//	}
			//	else
			//	{//남은 거리가없다면 루프를 종료한다.
			//		break;
			//	}
			//}
		}
		else
		{
			m_ChargeCancle = 0.0f;
			m_bIsCharging = true;
			CheckRangeCollide(*playerPos,m_GuideLineLeftEndPoint, &m_LeftGuideRange);
			CheckRangeCollide(*playerPos,m_GuideLineRightEndPoint, &m_RightGuideRange);

			if (m_ChargeTime > 1.3f)
			{
				m_bIsCharged = true;
				m_ChargeTime -= m_ChargeTime;
			}
			else
			{
				m_ChargeTime += CTimeMgr::GetInstance()->GetDeltaTime();
				m_LeftGuideAngle += (LerpFloat(m_LefttempAngle, m_GuideAngle, m_ChargeTime / 1.3f) - m_LeftGuideAngle);
				if (m_GuideAngle - 30.f<0.0f)
					m_RightGuideAngle += (LerpFloat(m_RighttempAngle, m_GuideAngle + 360.f, m_ChargeTime / 1.3f) - m_RightGuideAngle);
				else
					m_RightGuideAngle += (LerpFloat(m_RighttempAngle, m_GuideAngle, m_ChargeTime / 1.3f) - m_RightGuideAngle);
			}
		}
	}
	else
	{
		if (m_bIsCharged)
		{
			cout << "차지 풀림" << endl;
			m_CurState = THROW;
			m_bIsThrow = true;
		}
			
		if (m_bIsCharging)
		{
			if (m_ChargeCancle > 0.7f)
			{
				m_bIsCharging = false;
				m_ChargeTime -= m_ChargeTime;
				m_LeftGuideAngle = m_LefttempAngle;
				m_RightGuideAngle = m_RighttempAngle;
				m_ChargeCancle -= m_ChargeCancle;
			}
			else
				m_ChargeCancle += CTimeMgr::GetInstance()->GetDeltaTime();	
		}
	}
	
}

void CPlayerScript::MeeleAttack()
{
	if (pKeyMgr->GetInstance()->KeyDown(KEY_V))
	{
		m_CurState = MEELE;
	}
}

void CPlayerScript::Moving()
{
	float m_fSpeed = 0.5f;
	float m_moveX = 0.0f;
	float m_moveY = 0.0f;
	
	if (!m_bIsJump)
	{
		if (m_CurState == THROW || m_CurState == THROW_END||m_CurState==AIM|| m_CurState == AIM_WALK)
		{
			m_fSpeed = 0.2f;
		}
		switch (m_CurMoveDir)
		{
		case UP:
			m_moveY = m_fSpeed*m_fVelocity;
			break;
		case DOWN:
			m_moveY = -1 * (m_fSpeed*m_fVelocity);
			break;
		case LEFT_UP_45:
			m_moveX = -1 * (m_fSpeed*m_fVelocity);
			m_moveY = m_fSpeed*m_fVelocity;
			break;
		case RIGHT_UP_45:
			m_moveX = m_fSpeed*m_fVelocity;
			m_moveY = m_fSpeed*m_fVelocity;
			break;
		case LEFT:
			m_moveX = -1 * (m_fSpeed*m_fVelocity);
			break;
		case RIGHT:
			m_moveX = m_fSpeed*m_fVelocity;
			break;
		case LEFT_DOWN_45:
			m_moveX = -1 * (m_fSpeed*m_fVelocity);
			m_moveY = -1 * (m_fSpeed*m_fVelocity);
			break;
		case RIGHT_DOWN_45:
			m_moveX = m_fSpeed*m_fVelocity;
			m_moveY = -1 * (m_fSpeed*m_fVelocity);
			break;
		}

		playerPos->x += m_moveX;
		m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
		if (StepStair())
			return;
		if (CollideTiles())
		{
			*playerPos = m_PrePos;
			m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
			cout << "x충돌중" << endl;
		}
		m_PrePos = *playerPos;
		playerPos->y += m_moveY;
		m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
		//계단 확인
		if (StepStair())
			return;
		if (CollideTiles())
		{
			*playerPos = m_PrePos;
			m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
			cout << "y충돌중" << endl;
		}
	}
	else
	{
		Jump();
	}

}

void CPlayerScript::AnimState()
{
	if (m_CurState != m_PreState || m_CurDir != m_PreDir)
	{
		switch (m_CurState)
		{
		case IDLE:
			//cout << "대기상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_Idle_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				pAnimator->Play(L"Player_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_Idle_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_Idle_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_Idle_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case RUN_START:
			//cout << "뛰기 준비" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_ReadyRun_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				pAnimator->Play(L"Player_ReadyRun_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_ReadyRun_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_ReadyRun_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_ReadyRun_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case RUN:
			//cout << "뛰기" << endl;
			switch (m_CurDir)
			{
			case UP:
				cout << "윗방향 뛰기" << endl;
				pAnimator->Play(L"Player_Run_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				cout << "아랫방향 뛰기" << endl;
				pAnimator->Play(L"Player_Run_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
				cout << "왼쪽위방향 뛰기" << endl;
			case RIGHT_UP_45:
				cout << "오른쪽위방향 뛰기" << endl;
				pAnimator->Play(L"Player_Run_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
				cout << "왼쪽방향 뛰기" << endl;
			case RIGHT:
				cout << "오른쪽방향 뛰기" << endl;
				pAnimator->Play(L"Player_Run_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
				cout << "왼쪽아래방향 뛰기" << endl;
			case RIGHT_DOWN_45:
				cout << "오른쪽아래방향 뛰기" << endl;
				pAnimator->Play(L"Player_Run_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case  RUN_END:
			//cout << "뛰기 종료" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_RunStop_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				pAnimator->Play(L"Player_RunStop_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_RunStop_RightUp", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_RunStop_Righ", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_RunStop_RightDown", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
			//원거리 공격
		case THROW:
			//cout << "원거리 공격" << endl;
			if (m_AtkDir == RIGHT_ATK)
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Throw_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Throw_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Throw_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Throw_Right", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Throw_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			else
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Throw_Up2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Throw_Down2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Throw_Right2_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Throw_Right2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Throw_Right2_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			break;
		case THROW_END:
			//cout << "원거리 공격 중단." << endl;
			if (m_AtkDir == RIGHT_ATK)
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Throw_Up_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Throw_Down_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Throw_Right_Up_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Throw_Right_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Throw_Right_Down_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			else
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Throw_Up2_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Throw_Down2_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Throw_Right2_Up_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Throw_Right2_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Throw_Right2_Down_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			break;
		case MEELE:
			cout << "근거리 공격" << endl;
			if (m_AtkDir == RIGHT_ATK)
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Attack_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Attack_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Attack_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Attack_Right", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Attack_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			else
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Attack_Up2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Attack_Down2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Attack_Right_Up2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Attack_Right2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Attack_Right_Down2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			break;
		case JUMP:
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_Jump_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				pAnimator->Play(L"Player_Jump_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_Jump_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_Jump_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_Jump_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;

		case AIM:
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_Aim_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				pAnimator->Play(L"Player_Aim_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_Aim_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_Aim_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_Aim_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case AIM_WALK:
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_Aim_Walk_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				pAnimator->Play(L"Player_Aim_Walk_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_Aim_Walk_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_Aim_Walk_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_Aim_Walk_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		}
		
		m_PreState = m_CurState;
		m_PreDir = m_CurDir;
	}


}

void CPlayerScript::DirState()
{
	if (!m_bIsJump)
	{
		if (m_CurDir != m_PreDir)
		{
			switch (m_CurDir)
			{
			case UP:
				cout << "윗방향" << endl;
				m_JumpControlPos = D3DXVECTOR3(playerPos->x, playerPos->y, 0.0f);
				break;
			case DOWN:
				cout << "아랫방향" << endl;
				m_JumpControlPos = D3DXVECTOR3(playerPos->x, playerPos->y, 0.0f);
				
				break;
			case LEFT_UP_45:
				cout << "왼쪽위" << endl;
				pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3(playerPos->x - 10.f, playerPos->y + 40.f, 0.0f);
				break;
			case RIGHT_UP_45:
				cout << "오른위" << endl;
				pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 40.f, 0.0f);
				break;
			case LEFT:
				cout << "왼쪽" << endl;
				pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 20.f, 0.0f);
				break;
			case RIGHT:
				cout << "오른쪽" << endl;
				pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 20.f, 0.0f);
				break;
			case LEFT_DOWN_45:
				cout << "왼쪽아래" << endl;
				pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 10.f, 0.0f);
				break;
			case RIGHT_DOWN_45:
				cout << "오른아래." << endl;
				pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 10.f, 0.0f);
				break;
			}
		}
	}
}

void CPlayerScript::AtkState()
{
	//공격이 끝난경우.
	if ((m_CurState == THROW && !pAnimator->IsPlaying()) || (m_CurState == MEELE && !pAnimator->IsPlaying()))
	{
		//공격방향이 오른쪽일경우
		if (m_AtkDir == RIGHT_ATK)
			m_AtkDir = LEFT_ATK;
		//왼쪽일 경우.
		else if (m_AtkDir == LEFT_ATK)
			m_AtkDir = RIGHT_ATK;
		//원거리 공격이 끝난경우.
		if (m_CurState == THROW)
		{
			m_CurState = THROW_END;
			AttackBullet();
			if(m_bIsCharged)
				m_bIsCharged = false;
		}
		//근접 공격이 끝난경우.
		if (m_CurState == MEELE)
		{
			m_CurState = IDLE;
		}
	}
	else if (m_CurState == THROW_END && !pAnimator->IsPlaying())
	{
		m_CurState = IDLE;
		m_bIsThrow = false;
	}


}

void CPlayerScript::AttackBullet()
{
	CGameObject* pBullet;
	if (!m_bIsCharged)
	{
		pBullet = CObjectMgr::GetInstance()->AddCopy(L"Small_Ball", L"my_Bullet");
		pBullet->AddScripts(CBulletScript::Create(m_BulletAngle, 400.f, pBullet, CBulletScript::BULLET_TYPE::SMALL));
	}
	else
	{
		pBullet = CObjectMgr::GetInstance()->AddCopy(L"Basic_Ball", L"my_Bullet");
		pBullet->AddScripts(CBulletScript::Create(m_BulletAngle, 400.f, pBullet, CBulletScript::BULLET_TYPE::CHARGED));
	}
	pBullet->GetComponent<CTransform>()->SetPosition(*pTransform->GetWorldPos());
	pBullet->SetObjectLayer(m_pGameObject->GetObjectLayer());

}

void CPlayerScript::MouseDir()
{
	//360도로 변환하기 위한것
	if (m_BulletAngle < 0.0f)
		m_MouseAngle = m_BulletAngle + 360.f;
	else
		m_MouseAngle = m_BulletAngle;

	//우측 
	if (m_MouseAngle > 0.0f&&m_MouseAngle < 22.5f)
		m_CurDir = RIGHT;
	//우측 나머지
	else if (m_MouseAngle > 337.5f&&m_MouseAngle < 360.f)
		m_CurDir = RIGHT;
	//우측 상단
	else if (m_MouseAngle > 22.5f&&m_MouseAngle < 67.5f)
		m_CurDir = RIGHT_UP_45;
	//상단
	else if (m_MouseAngle > 67.5f&&m_MouseAngle < 112.5f)
		m_CurDir = UP;
	//좌측 상단
	else if (m_MouseAngle > 112.5f&&m_MouseAngle < 157.5f)
		m_CurDir = LEFT_UP_45;
	//좌측
	else if (m_MouseAngle > 157.5f&&m_MouseAngle < 202.5f)
		m_CurDir = LEFT;
	//좌측 하단
	else if (m_MouseAngle > 202.5f&&m_MouseAngle < 247.5f)
		m_CurDir = LEFT_DOWN_45;
	//하단
	else if (m_MouseAngle > 247.5f&&m_MouseAngle < 292.5f)
		m_CurDir = DOWN;
	//우측하단
	else if (m_MouseAngle > 292.5f&&m_MouseAngle < 337.5f)
		m_CurDir = RIGHT_DOWN_45;

	m_GuideAngle = m_MouseAngle;
	m_LeftGuideAngle = m_MouseAngle + 30.f;
	m_LefttempAngle = m_LeftGuideAngle;
	if (m_GuideAngle - 30.f < 0.f)
	{
		m_RightGuideAngle = 330.f + m_GuideAngle;;
	}
	else
		m_RightGuideAngle = m_MouseAngle - 30.f;

	m_RighttempAngle = m_RightGuideAngle;
	
}

bool CPlayerScript::CheckRangeCollide(D3DXVECTOR3& originPos, D3DXVECTOR3& guideLine, float * range, D3DXVECTOR3* points, D3DXVECTOR3* normal)
{
	LINE leftLine;
	leftLine.startPoint = originPos;
	leftLine.endPoint = guideLine;

	int mapSizex = (CObjectMgr::GetInstance()->m_MapSizeX );
	int mapSizey = (CObjectMgr::GetInstance()->m_MapSizeY );

	int originIndexX = ((mapSizex / 2) + originPos.x) / 16;
	int originIndexY = ((mapSizey / 2) - originPos.y) / 16;

	int pointIndexX = ((mapSizex / 2) + guideLine.x) / 16;
	int pointIndexY = ((mapSizey / 2) - guideLine.y) / 16;

	int pointGapX = originIndexX - pointIndexX;
	int pointGapY = originIndexY - pointIndexY;
		const vector<CGameObject*>& tiles = CObjectMgr::GetInstance()->GetTiles();
			//플레이어위치와 해당 점까지의 기울기
		float dx = pointIndexX - originIndexX;
		float dy = pointIndexY - originIndexY;
		float slope = dy / dx;

		int indexX, indexY;
			if (originIndexX < pointIndexX)
			{
				for (int i = originIndexX; i < pointIndexX; ++i)
				{
					indexX = i;
					indexY = slope*i - slope*originIndexX + originIndexY;
					int index = indexX + indexY*mapSizex;
					if (tiles[index] != nullptr)
						tiles[index]->SetObjectCliked(true, D3DCOLOR_XRGB(0, 0, 255));
					if (CheckLineRange(&leftLine, tiles[index], &guideLine, range, points, normal))
						return true;
				}
			}
			else if (originIndexX >pointIndexX)
			{
				for (int i = originIndexX; i > pointIndexX; --i)
				{
					indexX = i;
					indexY = slope*i - slope*originIndexX + originIndexY;
					int index = indexX + indexY*mapSizex;
					if (tiles[index] != nullptr)
						tiles[index]->SetObjectCliked(true, D3DCOLOR_XRGB(0, 0, 255));
					if (CheckLineRange(&leftLine, tiles[index], &guideLine, range, points, normal))
						return true;
				}
			}
			else if (originIndexY == pointIndexY)
			{
				if (originIndexX < pointIndexX)
				{
					for (int i = originIndexX; i < pointIndexX; ++i)
					{
						indexX = i;
						indexY = originIndexY;
						int index = indexX + indexY*mapSizex;
						if (tiles[index] != nullptr)
							tiles[index]->SetObjectCliked(true, D3DCOLOR_XRGB(0, 0, 255));
						if (CheckLineRange(&leftLine, tiles[index], &guideLine, range, points, normal))
							return true;
					}
				}
				else
				{
					for (int i = originIndexX; i >= pointIndexX; --i)
					{
						indexX = i;
						indexY = originIndexY;
						int index = indexX + indexY*mapSizex;
						if (tiles[index] != nullptr)
							tiles[index]->SetObjectCliked(true, D3DCOLOR_XRGB(0, 0, 255));
						if (CheckLineRange(&leftLine, tiles[index], &guideLine, range, points, normal))
							return true;
					}

				}

			}
			else if (originIndexX == pointIndexX)
			{
				if (originIndexY < pointIndexY)
				{
					for (int i = originIndexY; i < pointIndexY; ++i)
					{
						indexX = originIndexX;
						indexY = i;
						int index = indexX + indexY*mapSizex;
						if (tiles[index] != nullptr)
							tiles[index]->SetObjectCliked(true, D3DCOLOR_XRGB(0, 0, 255));
						if (CheckLineRange(&leftLine, tiles[index], &guideLine, range, points, normal))
							return true;
					}
				}
				else
				{
					for (int i = originIndexY; i >= pointIndexY; --i)
					{
						indexX = originIndexX;
						indexY = i;
						int index = indexX + indexY*mapSizex;
						if (tiles[index] != nullptr)
							tiles[index]->SetObjectCliked(true, D3DCOLOR_XRGB(0, 0, 255));
						if (CheckLineRange(&leftLine, tiles[index], &guideLine, range, points, normal))
							return true;
					}
				}
			}
		return false;
}

bool CPlayerScript::CheckLineRange(LINE* line,CGameObject* pobject,D3DXVECTOR3* points,float* range,D3DXVECTOR3* _point,D3DXVECTOR3* normal)
{
	if (pobject != nullptr )
	{
		if (true)
		{
			CBoxCollider* pDestBoxCollider = pobject->GetComponent<CBoxCollider>();
			D3DXVECTOR3	  destPos = *pobject->GetComponent<CTransform>()->GetWorldPos();

			if (pDestBoxCollider != nullptr)
			{
				COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
				D3DXVECTOR3 point;
				if (CCollisionMgr::GetInstance()->CheckLineBox2(line, pDestBoxCollider, &point,normal))
				{
					cout << "레이저 충돌" << endl;
					cout << point.x << " , " << point.y << endl;
					if(_point!=nullptr)
					*_point = point;

					float length = sqrtf((playerPos->x - point.x)*(playerPos->x - point.x) + (playerPos->y - point.y)*(playerPos->y - point.y));
					
					*range = length;

					return true;
				}
			}
		}	
	}
	_point = nullptr;
	normal = nullptr;
	*range =  200.f;
	return false;
}

void CPlayerScript::CheckTiles()
{
	const vector<CGameObject*>& tiles = CObjectMgr::GetInstance()->GetTiles();

	int mapSizex = CObjectMgr::GetInstance()->m_MapSizeX;
	int mapSizey = CObjectMgr::GetInstance()->m_MapSizeY;

	D3DXVECTOR3 pos = *pTransform->GetWorldPos();
	int indexX = (((mapSizex / 2) + playerPos->x) / 16);
	int indexY = (((mapSizey / 2) - playerPos->y) / 16);
	int index = indexX + indexY*mapSizex;


	for (int i = -2; i < 2; ++i)
	{
		for (int j = -2; j < 2; ++j)
		{
			m_NearTiles.push_back(tiles[indexX + i + (indexY + j)*mapSizex]);
		}

	}
}

bool CPlayerScript::CollideTiles()
{
	if (!m_bIsJump)
	{
		D3DXVECTOR3& playerPos = pTransform->GetLocalPosition();
		for (auto& i : m_NearTiles)
		{
			//플레이어는 한층아래에있는 오브젝트하고는 충돌하지 않는다. Layer1이면 Layer1이상으로만 충돌. Layer2로 올라갈시 2이상의 것만.
			//같은레이어가되면 더이상 Y소팅을 할 필요가 없다.
			if (i != nullptr&&(m_pGameObject->GetObjectLayer() <= i->GetObjectLayer()||i->GetObjectLayer()==LAYER_GROUND))
			{
				CBoxCollider* pDestBoxCollider = i->GetComponent<CBoxCollider>();
				D3DXVECTOR3	  destPos = *i->GetComponent<CTransform>()->GetWorldPos();
				//타일컬링되는 오브젝트들 확인. ( 충돌체 있는녀석만)
				if (m_bIsDebug)
					i->SetObjectCliked(true, D3DCOLOR_XRGB(255, 0, 0));
				if (pDestBoxCollider != nullptr)
				{
					COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
					//사각형충돌과 삼각형충돌이 있다
					//사각형 충돌시
					if (coltype == NORMAL)
					{
						if (CCollisionMgr::GetInstance()->CheckAABB(pBoxCollider, pDestBoxCollider))
						{
							cout << "사각형충돌" << endl;
							return true;
						}
					}
					//삼각형 충돌시. 그외 삼각형 충돌
					else if (coltype != NORMAL)
					{
						if (CCollisionMgr::GetInstance()->CheckLineBox(pBoxCollider, pDestBoxCollider))
							return true;
					}
				}
			}
		}
	}
	return false;
}

bool CPlayerScript::StepStair()
{
	if (!m_bIsJump)
	{
		const map<Layer, vector<CGameObject*>>& stairs = CObjectMgr::GetInstance()->GetStairs();
		Layer playerLayer = m_pGameObject->GetObjectLayer();

		for (auto& j : stairs)
		{
			for (auto&i : j.second)
			{
				//점프 계단도 같은레이어에는 작동하지않는다 땅으로 작용한다. 다만 아래 레이어로 내려가야하므로 같은경우만 충돌확인하지 않는다.
				int Uplayer = m_CurLayer + 1;
				Layer layer = Layer(Uplayer);
				if (i != nullptr)
				{
					CBoxCollider* pDestBoxCollider = i->GetComponent<CBoxCollider>();
					D3DXVECTOR3	  destPos = *i->GetComponent<CTransform>()->GetWorldPos();
					D3DXVECTOR3  jumpPos;
					if (pDestBoxCollider != nullptr)
					{
						int current;
						Layer currentLayer;
						switch (playerLayer)
						{
						case LAYER_1:
							//같은 경우는 발판이되어 그단계로 올라간다.
							if (layer == i->GetObjectLayer())
							{
								if (CCollisionMgr::GetInstance()->CheckAABB(playerUpBox, pDestBoxCollider))
								{
									COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
									//사각형충돌과 삼각형충돌이 있다
									//사각형 충돌시
									if (coltype == NORMAL)
									{
										cout << " 윗 발판 충돌" << endl;
										current = m_pGameObject->GetObjectLayer() + 1;
										currentLayer = (Layer)current;
										m_pGameObject->SetObjectLayer(currentLayer);
										m_ChangeLayer = currentLayer;
										m_CurLayer = layer;
										JumpSetUp(*playerPos, destPos, 1.0f);
										return true;
									}
								}
							} 
							break;
						case LAYER_2:
							//같은 경우는 발판이되어 그단계로 올라간다.
							if (layer == i->GetObjectLayer())
							{
								if (CCollisionMgr::GetInstance()->CheckAABB(playerUpBox, pDestBoxCollider))
								{
									COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
									//사각형충돌과 삼각형충돌이 있다
									//사각형 충돌시
									if (coltype == NORMAL)
									{
										cout << " 윗 발판 충돌" << endl;
										current = m_pGameObject->GetObjectLayer() + 1;
										currentLayer = (Layer)current;
										m_pGameObject->SetObjectLayer(currentLayer);
										m_ChangeLayer = currentLayer;
										m_CurLayer = layer;
										JumpSetUp(*playerPos, destPos, 1.0f);
										return true;
									}
								}
							} //플레이어보다 낮은레이어의 발판을 만날경우.
							else if (m_CurLayer > i->GetObjectLayer())
							{
								if (CCollisionMgr::GetInstance()->CheckAABB(playerDownBox, pDestBoxCollider))
								{
									COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
									//사각형충돌과 삼각형충돌이 있다
									//사각형 충돌시
									if (coltype == NORMAL)
									{
										cout << " 아래 발판 충돌" << endl;
										if (i->GetObjectLayer() == LAYER_0)
										{
											current = m_pGameObject->GetObjectLayer() - 1;
											if (m_CurDir == DOWN)
												destPos.y -= 20.f;
											else
												destPos.y -= 20.f;
										}
										m_pGameObject->SetObjectLayer(LAYER_3);
										currentLayer = (Layer)current;		
										m_ChangeLayer = currentLayer;
										m_CurLayer = (Layer)(current - 1);
										JumpSetUp(*playerPos, destPos, 1.0f);
										return true;
									}
								}
							}
							break;
						case LAYER_3:
							//플레이어보다 낮은레이어의 발판을 만날경우.
						if (m_CurLayer > i->GetObjectLayer())
							{
								if (CCollisionMgr::GetInstance()->CheckAABB(playerDownBox, pDestBoxCollider))
								{
									COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
									//사각형충돌과 삼각형충돌이 있다
									//사각형 충돌시
									if (coltype == NORMAL)
									{
										cout << " 아래 발판 충돌" << endl;
										if (i->GetObjectLayer() == LAYER_0)
										{
											current = m_pGameObject->GetObjectLayer() - 2;
											if(m_CurDir == DOWN)
												destPos.y -= 40.f;
											else
												destPos.y -= 20.f;
										}	
										else
										{
											current = m_pGameObject->GetObjectLayer() - 1;
											if (m_CurDir == DOWN)
												destPos.y -= 20.f;
											else
												destPos.y -= 20.f;
										}
											
										currentLayer = (Layer)current;
										m_ChangeLayer = currentLayer;
										m_CurLayer = (Layer)(current - 1);
										JumpSetUp(*playerPos, destPos, 1.0f);
										return true;
									}
								}
							}
							break;
						}
		
					}
				}
			}
		}
	}
	return false;
}
void CPlayerScript::Jump()
{

	if (m_fJumpTime <= 0.5f)
	{
		m_CurState = JUMP;
		DIR dir = m_CurDir;
		D3DXVECTOR3 newPos = BezierCurve(m_JumpStartPos, m_JumpEndPos, m_JumpControlPos, (m_fJumpTime / 0.5f));
		newPos.y += 16.f;
		pTransform->SetPosition(newPos);
	}
	else
	{
		m_pGameObject->SetObjectLayer(m_ChangeLayer);
		m_bIsJump = false;
		m_fJumpTime -= m_fJumpTime;
		cout << "점프끝" << endl;
		m_CurState = IDLE;
	}
	m_fJumpTime += CTimeMgr::GetInstance()->GetDeltaTime();

}

void CPlayerScript::JumpSetUp(const D3DXVECTOR3 & startPos, const D3DXVECTOR3 & endPos, const float & endTime)
{
	switch (m_CurDir)
	{
	case UP:
		m_JumpControlPos = D3DXVECTOR3(playerPos->x, playerPos->y, 0.0f);
		break;
	case DOWN:
		m_JumpControlPos = D3DXVECTOR3(playerPos->x, playerPos->y-20.f, 0.0f);
		break;
	case LEFT_UP_45:
		m_JumpControlPos = D3DXVECTOR3(playerPos->x - 10.f, playerPos->y + 40.f, 0.0f);
		break;
	case RIGHT_UP_45:
		m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 40.f, 0.0f);
		break;
	case LEFT:
		m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 20.f, 0.0f);
		break;
	case RIGHT:
		pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 20.f, 0.0f);
		break;
	case LEFT_DOWN_45:
		m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 10.f, 0.0f);
		break;
	case RIGHT_DOWN_45:
		m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 10.f, 0.0f);
		break;
	}
	m_JumpStartPos = startPos;
	m_JumpEndPos = endPos;
	m_fJumpEndTime = endTime;
	m_bIsJump = true;
}

void CPlayerScript::CheckLayer()
{
	if (m_CurLayer != m_PreLayer)
	{
		switch (m_CurLayer)
		{
		case LAYER_0:
			cout << "1층" << endl;
			playerUpBox = pBoxCollider;
			playerDownBox = pBoxCollider;
			break;
		case LAYER_1:
			cout << "1.5층" << endl;
			playerUpBox = pBoxCollider;
			playerDownBox = m_playerFoot;
			break;
		case LAYER_2:
			cout << "2층" << endl;
			playerUpBox = m_playerFoot;
			playerDownBox = m_playerFoot;
			break;
		case LAYER_3:
			cout << "2.5층" << endl;
			break;
		case LAYER_4:
			cout << "3층" << endl;
			break;
		case LAYER_5:
			cout << "여긴 올라오면안되" << endl;
			break;
		}
		m_PreLayer = m_CurLayer;
	}
}

void CPlayerScript::DrawGuideLine()
{
	
	D3DXMATRIX*	mat = CCameraMgr::GetInstance()->GetViewProjMatrix();

	m_GuideLineEndPoint = D3DXVECTOR3(playerPos->x + m_GuideRange*cosf(D3DXToRadian(m_GuideAngle)), playerPos->y + m_GuideRange*sinf(D3DXToRadian(m_GuideAngle)), 0.0f);
	
	if (!m_bIsHitSomething)
	{
		m_GuideLineLeftEndPoint = D3DXVECTOR3(playerPos->x + m_LeftGuideRange*cosf(D3DXToRadian(m_LeftGuideAngle)), playerPos->y + m_LeftGuideRange*sinf(D3DXToRadian(m_LeftGuideAngle)), 0.0f);
		m_GuideLineRightEndPoint = D3DXVECTOR3(playerPos->x + m_RightGuideRange*cosf(D3DXToRadian(m_RightGuideAngle)), playerPos->y + m_RightGuideRange*sinf(D3DXToRadian(m_RightGuideAngle)), 0.0f);
	}
	
	CDeviceMgr::GetInstance()->GetLine()->SetWidth(2.f);
	CDeviceMgr::GetInstance()->GetLine()->Begin();
	{
		D3DXVECTOR3 m_Line[2];
		//m_Line[0] = *playerPos;
		//m_Line[1] = m_GuideLineEndPoint;
		//CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(0,0,255));
		if (!m_bIsCharged)
		{
			m_Line[0] = *playerPos;
			m_Line[1] = m_GuideLineLeftEndPoint;
			CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(255, 0, 255));

			m_Line[0] = *playerPos;
			m_Line[1] = m_GuideLineRightEndPoint;
			CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(255, 0, 255));

		}
		else
		{
			if (!m_ChargeLine.empty())
			{
				for (auto&i : m_ChargeLine)
				{
					m_Line[0] = i.first;
					m_Line[1] = i.second;
					CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(0, 0, 255));
				}
			}
		}
	}
	CDeviceMgr::GetInstance()->GetLine()->End();
}

