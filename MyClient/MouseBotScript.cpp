#include "stdafx.h"
#include "MouseBotScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "TextureRenderer.h"

#include "Effect.h"

CMouseBotScript::CMouseBotScript()
{
	m_fAngle = 0.0f;
}


CMouseBotScript::~CMouseBotScript()
{
}

void CMouseBotScript::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();
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
		CEffect::Create(*m_Pos, XMFLOAT3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.5f,1.5f,1.0f), L"Explosion_Effect", L"Small_Explosion", ANIMATION_ONCE);
		return 0;
	}
	CheckTiles();

	DirState();
	AnimState();
	Move();

	m_NearTiles.clear();
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

		//360도로 변환하기 위한것
		if (m_fAngle < 0.0f)
			m_fAngle = m_fAngle + 360.f;

		//우측 
		if (m_fAngle > 0.0f&&m_fAngle < 22.5f)
			m_CurDir = RIGHT;
		//우측 나머지
		else if (m_fAngle > 337.5f&&m_fAngle < 360.f)
			m_CurDir = RIGHT;
		//우측 상단
		else if (m_fAngle > 22.5f&&m_fAngle < 67.5f)
			m_CurDir = RIGHT_UP_45;
		//상단
		else if (m_fAngle > 67.5f&&m_fAngle < 112.5f)
			m_CurDir = UP;
		//좌측 상단
		else if (m_fAngle > 112.5f&&m_fAngle < 157.5f)
			m_CurDir = LEFT_UP_45;
		//좌측
		else if (m_fAngle > 157.5f&&m_fAngle < 202.5f)
			m_CurDir = LEFT;
		//좌측 하단
		else if (m_fAngle > 202.5f&&m_fAngle < 247.5f)
			m_CurDir = LEFT_DOWN_45;
		//하단
		else if (m_fAngle > 247.5f&&m_fAngle < 292.5f)
			m_CurDir = DOWN;
		//우측하단
		else if (m_fAngle > 292.5f&&m_fAngle < 337.5f)
			m_CurDir = RIGHT_DOWN_45;

	if (m_CurDir != m_PreDir&&m_CurState!=HIT)
	{
		switch (m_CurDir)
		{
		case UP:
			break;
		case DOWN:
			break;
		case LEFT_UP_45:
			break;
		case RIGHT_UP_45:
			break;
		case LEFT:
			break;
		case RIGHT:
			break;
		case LEFT_DOWN_45:
			break;
		case RIGHT_DOWN_45:
			break;
		}
		m_PreDir = m_CurDir;
	}
}

void CMouseBotScript::AnimState()
{
	if (m_CurState == HIT)
	{
		Hit();
	}
		

	if (m_CurState != m_PreState || m_CurDir != m_PreDir)
	{
		switch (m_CurState)
		{
		case IDLE:
			m_pTexture->SetPass(0);
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
			m_pTexture->SetPass(1);
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

	m_fWhiteValue = 0.0f;

}

void CMouseBotScript::Hit()
{
	if (m_fWhiteValue < 0.5f)
	{
		m_pTexture->SetAlpha(m_fWhiteValue*2.f);
		m_fWhiteValue += CTimeMgr::GetInstance()->GetDeltaTime();
	}
	else
	{
		m_fWhiteValue -= m_fWhiteValue;
		m_CurState = IDLE;
	}
}

void CMouseBotScript::Move()
{
	m_PrePos = *m_Pos;
	m_Pos->x += m_DirVec.x*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	m_pBoxCollider->SetBoxCollider();
	if (CollideTiles())
	{
		RandomMove();
		*m_Pos = m_PrePos;
		m_pBoxCollider->SetBoxCollider();
	}
	m_PrePos = *m_Pos;
	m_Pos->y += m_DirVec.y*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	m_pBoxCollider->SetBoxCollider();
	if (CollideTiles())
	{
		RandomMove();
		*m_Pos = m_PrePos ;
		m_pBoxCollider->SetBoxCollider();
	}

	if (m_CurState == MOVE)
	{
		if (m_fTime > m_fRandomMoveTime)
		{
			m_fTime -= m_fTime;
			m_CurState = IDLE;
		}
		else
		{
			m_fVelocity += (1.5f*powf(m_fAcc, 2.0f));
			m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime();

			m_fAcc = __min(2.0f, m_fAcc);
			m_fVelocity = __min(40.f, m_fVelocity);

			m_fTime += CTimeMgr::GetInstance()->GetDeltaTime();
		}
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
	random_device		rn;
	mt19937_64 rnd(rn());

	uniform_real_distribution<float> nTime(2.0f, 5.f);
	float randTime = nTime(rnd);

	m_fRandomMoveTime = randTime;

	m_fWaitTime = nTime(rnd);
	//랜덤의 범위 0~360.0f;
	uniform_real_distribution<float> ndist(0.0f, 360.f);
	float degree = ndist(rnd);
	m_fAngle = degree;
	D3DXMATRIX rotMat;
	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixRotationZ(&rotMat, D3DXToRadian(degree));


	m_DirVec = D3DXVECTOR3(cosf(degree), sinf(degree), 0.0f);

	D3DXVec3Normalize(&m_DirVec, &m_DirVec);
	/*uniform_real_distribution<float> fDist(0.0f, 360.f);
	float radius = fDist(rnd);*/

	m_CurState = MOVE;
}

