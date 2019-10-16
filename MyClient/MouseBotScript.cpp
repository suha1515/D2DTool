#include "stdafx.h"
#include "MouseBotScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"

CMouseBotScript::CMouseBotScript()
{

}


CMouseBotScript::~CMouseBotScript()
{
}

void CMouseBotScript::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Mouse_Bot");
		//초기 클립세트 설정
		m_pAnimator->Play(L"Mouse_Bot_Down", ANIMATION_TYPE::ANIMATION_LOOP);
	}
		
	m_CurDir = DOWN;
	m_PreDir = m_CurDir;
	m_DirVec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_CurState = IDLE;
	m_PreState = m_CurState;

	m_Pos = &m_pTransform->GetLocalPosition();

	m_Hp = 50.f;
	m_fRaidus = 70.f;

	m_fRandomMoveTime = 0.f;
	m_fWaitTime = 2.0f;
	
}

void CMouseBotScript::OnEnable()
{
}

void CMouseBotScript::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move )
{

}

void CMouseBotScript::OnInput()
{
}

int CMouseBotScript::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}

	if (m_Hp < 0.0f)
	{
		m_pGameObject->SetObjectDestroy(true);
		return 0;
	}
	CheckTiles();

	DirState();
	AnimState();
	Move();
	return 0;
}

void CMouseBotScript::OnLateUpdate()
{
}

void CMouseBotScript::OnRender()
{
}

void CMouseBotScript::OnDisable()
{
}

void CMouseBotScript::OnDestroy()
{
}

void CMouseBotScript::DirState()
{
	if (m_CurDir != m_PreDir&&m_CurState!=HIT)
	{
		switch (m_CurDir)
		{
		case UP:
			cout << "윗방향" << endl;
			m_DirVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		case DOWN:
			cout << "아랫방향" << endl;
			m_DirVec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			break;
		case LEFT_UP_45:
			cout << "왼쪽위" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			m_DirVec = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&m_DirVec, &m_DirVec);
			//m_JumpControlPos = D3DXVECTOR3(playerPos->x - 10.f, playerPos->y + 40.f, 0.0f);
			break;
		case RIGHT_UP_45:
			cout << "오른위" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_DirVec = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&m_DirVec, &m_DirVec);
			//m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 40.f, 0.0f);
			break;
		case LEFT:
			cout << "왼쪽" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			m_DirVec = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			
			//m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 20.f, 0.0f);
			break;
		case RIGHT:
			cout << "오른쪽" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 0.0f, 1.0f));
			m_DirVec = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
			
			//m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 20.f, 0.0f);
			break;
		case LEFT_DOWN_45:
			cout << "왼쪽아래" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			m_DirVec = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&m_DirVec, &m_DirVec);
			//m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 10.f, 0.0f);
			break;
		case RIGHT_DOWN_45:
			cout << "오른아래." << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			m_DirVec = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&m_DirVec, &m_DirVec);
			//m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 10.f, 0.0f);
			break;
		}
		m_PreDir = m_CurDir;
	}
}

void CMouseBotScript::AnimState()
{
	if (m_CurState == HIT && !m_pAnimator->IsPlaying())
	{
		cout << "맞는 애니메이션 끝" << endl;
		m_CurState = IDLE;
	}
		

	if (m_CurState != m_PreState || m_CurDir != m_PreDir)
	{
		switch (m_CurState)
		{
		case IDLE:
			//cout << "대기상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Mouse_Bot_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				m_pAnimator->Play(L"Mouse_Bot_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				m_pAnimator->Play(L"Mouse_Bot_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Mouse_Bot_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				m_pAnimator->Play(L"Mouse_Bot_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case HIT:
			//cout << "대기상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Mouse_Bot_Hit_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				m_pAnimator->Play(L"Mouse_Bot_Hit_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				m_pAnimator->Play(L"Mouse_Bot_Hit_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Mouse_Bot_Hit_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				m_pAnimator->Play(L"Mouse_Bot_Hit_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		}
		m_PreState = m_CurState;
	}
}

void CMouseBotScript::GetHit(D3DXVECTOR3 dirVec,float power, float dmg)
{
	m_CurState = HIT;
	m_DirVec = dirVec;
	m_fVelocity = power;

	m_Hp -= dmg;

	cout << "맞음" << endl;
}

void CMouseBotScript::Move()
{
	m_PrePos = *m_Pos;
	m_Pos->x += m_DirVec.x*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	m_pBoxCollider->SetBoxCollider();
	if (CollideTiles())
	{
		*m_Pos = m_PrePos;
		m_pBoxCollider->SetBoxCollider();
	}
	m_PrePos = *m_Pos;
	m_Pos->y += m_DirVec.y*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	m_pBoxCollider->SetBoxCollider();
	if (CollideTiles())
	{
		*m_Pos = m_PrePos ;
		m_pBoxCollider->SetBoxCollider();
	}

	if (m_CurState == MOVE)
	{
		/*if (m_fTime > m_fRandomMoveTime)
		{
			m_fTime -= m_fTime;
			m_CurState = IDLE;
		}
		else
		{
			m_fVelocity += (1.5f*powf(m_fAcc, 2.0f));
			m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime();

			m_fAcc = __max(2.0f, m_fAcc);
			m_fVelocity = __max(5.f, m_fVelocity);

			m_fTime += CTimeMgr::GetInstance()->GetDeltaTime();
		}*/
	}
	else
	{
		m_fVelocity -= (2.0f*powf(m_fAcc, 2.0f));
		m_fAcc -= CTimeMgr::GetInstance()->GetDeltaTime();

		m_fAcc = __max(0.5f, m_fAcc);
		m_fVelocity = __max(0.f, m_fVelocity);
	}
	
	if (m_CurState == IDLE)
	{
		if (m_fTime > m_fWaitTime)
		{
			m_fTime -= m_fTime;
			RandomMove();
		}
		m_fTime += CTimeMgr::GetInstance()->GetDeltaTime();
	}
}

void CMouseBotScript::RandomMove()
{
	//random_device		rn;
	//mt19937_64 rnd(rn());

	//uniform_real_distribution<float> nTime(2.0f, 5.f);
	//float randTime = nTime(rnd);

	//m_fRandomMoveTime = randTime;

	//m_fWaitTime = nTime(rnd);
	////랜덤의 범위 0~360.0f;
	//uniform_real_distribution<float> ndist(0.0f, 360.f);
	//float degree = ndist(rnd);

	//D3DXMATRIX rotMat;
	//D3DXMatrixIdentity(&rotMat);
	//D3DXMatrixRotationZ(&rotMat, D3DXToRadian(degree));

	//D3DXVECTOR4 newDir;
	//D3DXVec3Transform(&newDir, &m_DirVec, &rotMat);

	//m_DirVec = D3DXVECTOR3(newDir.x, newDir.y, 0.0f);

	//D3DXVec3Normalize(&m_DirVec, &m_DirVec);
	///*uniform_real_distribution<float> fDist(0.0f, 360.f);
	//float radius = fDist(rnd);*/

	m_CurState = MOVE;
}

