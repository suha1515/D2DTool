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
		MessageBox(0,L"애니메이터 없음", L"Error",0);
	
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

void CPlayerScript::OnUpdate()
{
	if (!bIsInit)
		OnInit();
	CTransform* pTransform = m_pGameObject->GetComponent<CTransform>();
	if (pTransform != nullptr)
	{
		m_PrePos = *playerPos;
		//타일확인
		CheckTiles();
		//방향키 입력
		MoveInput();
		//마우스 입력
		MouseInput();
		//근접공격 입력
		MeeleAttack();
		//방향 변화.
		DirState();
		//공격 상태 변화
		AtkState();
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
	
}

void CPlayerScript::OnLateUpdate()
{
}

void CPlayerScript::OnRender()
{
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
			m_fVelocity += (3.0f*powf(m_fAcc, 2.0f));						//가속도 공식 조절할 필요가있음
			m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime();

			m_fAcc = __min(2.0f, m_fAcc);
			m_fVelocity = __min(10.f, m_fVelocity);
			if (m_CurState == IDLE || m_CurState == RUN_END)
				m_CurState = RUN_START;
			else if (m_CurState == RUN_START && !pAnimator->IsPlaying())
			{
				if (m_CurState != THROW_END && !pAnimator->IsPlaying())
					m_CurState = RUN;
			}
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
		if (!m_bIsThrow)
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
	m_Right = false;
	m_Left = false;
	m_Down = false;
	m_Up = false;
}

void CPlayerScript::MouseInput()
{
	if (pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		cout << "던지기!" << endl;
	
		D3DXVECTOR3 mousePos = pKeyMgr->GetMouse()->GetMousePos();
		

		m_BulletAngle = GetAngle(*playerPos, mousePos);

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

		m_CurState = THROW;
		m_bIsThrow = true;
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
	float m_fSpeed=0.5f;
	float m_moveX = 0.0f;
	float m_moveY = 0.0f;
	if (m_CurState == THROW|| m_CurState == THROW_END)
	{
		m_fSpeed = 0.2f;
	}
	switch (m_CurMoveDir)
	{
	case UP:
		m_moveY = m_fSpeed*m_fVelocity;
		break;
	case DOWN:
		m_moveY = -1*(m_fSpeed*m_fVelocity);
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
	if (CollideTiles())
	{
		*playerPos = m_PrePos;
		m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
		cout << "x충돌중" << endl;
	}
	m_PrePos = *playerPos;
	playerPos->y += m_moveY;
	m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
	if (CollideTiles())
	{
		*playerPos = m_PrePos;
		m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
		cout << "y충돌중" << endl;
	}
	

}

void CPlayerScript::AnimState()
{
	if (m_CurState != m_PreState||m_CurDir != m_PreDir)
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
		}
		m_PreState = m_CurState;
		m_PreDir = m_CurDir;
	}


}

void CPlayerScript::DirState()
{
	if (m_CurDir != m_PreDir)
	{
		switch (m_CurDir)
		{
		case UP:
			cout << "윗방향" << endl;
			break;
		case DOWN:
			cout << "아랫방향" << endl;
			break;
		case LEFT_UP_45:
			cout << "왼쪽위" << endl;
			pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT_UP_45:
			cout << "오른위" << endl;
			pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		case LEFT:
			cout << "왼쪽" << endl;
			pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT:
			cout << "오른쪽" << endl;
			pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		case LEFT_DOWN_45:
			cout << "왼쪽아래" << endl;
			pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT_DOWN_45:
			cout << "오른아래." << endl;	
			pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		}
	}
}

void CPlayerScript::AtkState()
{
	//공격이 끝난경우.
	if ((m_CurState == THROW && !pAnimator->IsPlaying())|| (m_CurState == MEELE && !pAnimator->IsPlaying()))
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
	CGameObject* pBullet = CObjectMgr::GetInstance()->AddCopy(L"Small_Ball", L"my_Bullet");
	pBullet->GetComponent<CTransform>()->SetPosition(*pTransform->GetWorldPos());

	pBullet->AddScripts(CBulletScript::Create(m_BulletAngle, 400.f, pBullet));
}

void CPlayerScript::CheckTiles()
{
	const vector<CGameObject*>& tiles = CObjectMgr::GetInstance()->GetTiles();

	int mapSizex = CObjectMgr::GetInstance()->m_MapSizeX;
	int mapSizey = CObjectMgr::GetInstance()->m_MapSizeY;

	D3DXVECTOR3 pos = *pTransform->GetWorldPos();
	int indexX = ((( mapSizex / 2)+ playerPos->x)/ 16);
	int indexY = ((( mapSizey / 2)- playerPos->y)/ 16);
	int index = indexX + indexY*mapSizex;

	m_NearTiles.push_back(tiles[(indexX-1)+ (indexY-1)*mapSizex]);
	m_NearTiles.push_back(tiles[ indexX + (indexY-1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX+1) + (indexY-1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX-1) + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX+1) + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX-1) + (indexY+1)*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + (indexY+1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX+1) + (indexY+1)*mapSizex]);

}

bool CPlayerScript::CollideTiles()
{
	D3DXVECTOR3& playerPos = pTransform->GetLocalPosition();
	for (auto& i : m_NearTiles)
	{
		if (i != nullptr)
		{
			CBoxCollider* pDestBoxCollider = i->GetComponent<CBoxCollider>();
			D3DXVECTOR3	  destPos = *i->GetComponent<CTransform>()->GetWorldPos();
			//타일컬링되는 오브젝트들 확인. ( 충돌체 있는녀석만)
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
				else if(coltype!=NORMAL)
				{
					if (CCollisionMgr::GetInstance()->CheckLineBox(pBoxCollider, pDestBoxCollider))
						return true;					
				}		
			}
		}
	}
	return false;
}

