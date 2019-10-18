#include "stdafx.h"
#include "NormalBotScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"

#include "TextureRenderer.h"

#include "Effect.h"


CNormalBotScript::CNormalBotScript()
{
}


CNormalBotScript::~CNormalBotScript()
{
}

void CNormalBotScript::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();
	m_pGameObject->SetObjectTag(L"Enemy");
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Normal_Bot");
		//초기 클립세트 설정
		m_pAnimator->Play(L"Normal_Bot_Idle_Up", ANIMATION_TYPE::ANIMATION_LOOP);
	}

	m_pPlayer = CObjectMgr::GetInstance()->m_pPlayer;

	m_CurDir = UP;
	m_PreDir = m_CurDir;
	m_DirVec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_CurState = IDLE;
	m_PreState = m_CurState;

	m_Pos = &m_pTransform->GetLocalPosition();

	m_Hp = 50.f;
}

void CNormalBotScript::OnEnable()
{
}

void CNormalBotScript::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move )
{
}

void CNormalBotScript::OnInput()
{
}

int CNormalBotScript::OnUpdate()
{

	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}
	CheckTiles();
	GetDirPlayer();
	if (m_IsActive)
	{
		if (m_Hp < 0.0f)
		{
			m_pGameObject->SetObjectDestroy(true);
			CEffect::Create(*m_Pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 1.0f), L"Explosion_Effect", L"Small_Explosion", ANIMATION_ONCE);
			return 0;
		}
		
		TrackPlayer();
		AttackState();
	}
	DirState();
	Move();
	AnimState();
	m_NearTiles.clear();
	return 0;
}

void CNormalBotScript::OnLateUpdate()
{
}

void CNormalBotScript::OnRender()
{
}

void CNormalBotScript::OnDisable()
{
}

void CNormalBotScript::OnDestroy()
{
}

void CNormalBotScript::DirState()
{

	if (m_CurDir != m_PreDir)
	{
		switch (m_CurDir)
		{
		case UP:
			cout << "위" << endl;
			break;
		case DOWN:
			cout << "아래" << endl;
			break;
		case LEFT_UP_45:
			cout << "왼쪽" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			//m_JumpControlPos = D3DXVECTOR3(playerPos->x - 10.f, playerPos->y + 40.f, 0.0f);
			break;
		case RIGHT_UP_45:
			cout << "오른쪽" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			//m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 40.f, 0.0f);
			break;
		case LEFT:
			cout << "왼쪽" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			//m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 20.f, 0.0f);
			break;
		case RIGHT:
			cout << "오른쪽" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			//m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 20.f, 0.0f);
			break;
		case LEFT_DOWN_45:
			cout << "왼쪽" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));

			//m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 10.f, 0.0f);
			break;
		case RIGHT_DOWN_45:
			cout << "오른쪽" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			//m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 10.f, 0.0f);
			break;
		}
	}
}

void CNormalBotScript::AnimState()
{
	if (m_CurState == HIT )
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
				m_pAnimator->Play(L"Normal_Bot_Idle_Up", ANIMATION_TYPE::ANIMATION_LOOP);
			break;
			case DOWN:
				m_pAnimator->Play(L"Normal_Bot_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);
			break;
			case LEFT_UP_45:
			case LEFT_DOWN_45:
			case RIGHT_UP_45:
			case RIGHT_DOWN_45:
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Normal_Bot_Idle_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case HIT:
			m_pTexture->SetPass(1);
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Normal_Bot_Hit_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				m_pAnimator->Play(L"Normal_Bot_Hit_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case LEFT_DOWN_45:
			case RIGHT_UP_45:
			case RIGHT_DOWN_45:
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Normal_Bot_Hit_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case ATTACK_READY:
			cout << "공격 대기상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Normal_Bot_Attack_Ready_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				m_pAnimator->Play(L"Normal_Bot_Attack_Ready_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case LEFT_DOWN_45:
			case RIGHT_UP_45:
			case RIGHT_DOWN_45:
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Normal_Bot_Attack_Ready_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case ATTACK:
			cout << "공격" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Normal_Bot_Attack_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				m_pAnimator->Play(L"Normal_Bot_Attack_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
			case LEFT_DOWN_45:
			case RIGHT_UP_45:
			case RIGHT_DOWN_45:
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Normal_Bot_Attack_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case MOVE:
			cout << "이동상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Normal_Bot_Idle_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				m_pAnimator->Play(L"Normal_Bot_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
			case LEFT_DOWN_45:
			case RIGHT_UP_45:
			case RIGHT_DOWN_45:
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Normal_Bot_Idle_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
				break;
		}
		m_PreState = m_CurState;
		m_PreDir = m_CurDir;
	}
}

void CNormalBotScript::Move()
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
		*m_Pos = m_PrePos;
		m_pBoxCollider->SetBoxCollider();
	}

		if (m_CurState == MOVE)
		{
			/*m_fVelocity = 50.f;*/
			m_fVelocity += (1.5f*powf(m_fAcc, 2.0f));
			m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime()*1.2f;

			m_fAcc = __min(2.0f, m_fAcc);
			m_fVelocity = __min(50.f, m_fVelocity);
		}
		else
		{
			m_fVelocity -= (3.0f*powf(m_fAcc, 2.0f));
			m_fAcc -= CTimeMgr::GetInstance()->GetDeltaTime();

			m_fAcc = __max(1.f, m_fAcc);
			m_fVelocity = __max(0.f, m_fVelocity);
	}
}

void CNormalBotScript::GetHit(D3DXVECTOR3 dirVec, float power, float dmg)
{
	if (m_IsActive)
	{
		m_CurState = HIT;
		m_DirVec = dirVec;
		m_fVelocity = power;

		m_Hp -= dmg;

		m_fWhiteValue = 0.0f;
		m_pTexture->SetFadeColor(XMFLOAT3(1.0f, 0.0f, 0.0f));
	}
}

void CNormalBotScript::Hit()
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

void CNormalBotScript::TrackPlayer()
{
	
	if (m_pPlayer != nullptr)
	{
		D3DXVECTOR3 botPos = *m_pTransform->GetWorldPos();
		D3DXVECTOR3 playerPos = *m_pPlayer->GetComponent<CTransform>()->GetWorldPos();
		float dist = sqrtf((playerPos.x - botPos.x)*(playerPos.x - botPos.x) + (playerPos.y - botPos.y)*(playerPos.y - botPos.y));
		if (m_CurState != ATTACK&&m_CurState!=HIT)
		{
			if (dist < 200.f&&dist>50.f)
			{
				m_CurState = MOVE;
				m_fAngle = GetAngle(botPos, playerPos);
				D3DXVECTOR3 dir = playerPos - botPos;
				D3DXVec3Normalize(&m_DirVec, &dir);
			}
			else if (dist<50.f)
			{
				m_CurState = ATTACK_READY;
			}
			else if (dist > 200.f)
			{
				m_CurState = IDLE;
			}
		}
		//if(m_CurState==ATTACK_READY&&dist>)

		
	}
}

void CNormalBotScript::GetDirPlayer()
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
}

void CNormalBotScript::AttackState()
{
	if (m_CurState == ATTACK_READY && !m_pAnimator->IsPlaying())
	{
		m_CurState = ATTACK;
		cout << "대쉬공격!" << endl;
		Dash();
	}
	else if (m_CurState == ATTACK)
	{
		if (m_fVelocity == 0.f)
			m_CurState = IDLE;
		cout << m_fVelocity << endl;
	}
}

void CNormalBotScript::Dash()
{
	m_fVelocity = 200.f;
}
