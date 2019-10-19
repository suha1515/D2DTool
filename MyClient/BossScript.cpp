#include "stdafx.h"
#include "BossScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"

#include "TextureRenderer.h"

#include "Effect.h"
#include "BossIceEffect.h"
#include "BossFireBreath.h"

CBossScript::CBossScript()
{
}


CBossScript::~CBossScript()
{
}

void CBossScript::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();
	m_pGameObject->SetObjectTag(L"Enemy");
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Boss");
		//초기 클립세트 설정
		m_pAnimator->Play(L"Boss_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);
	}

	m_pPlayer = CObjectMgr::GetInstance()->m_pPlayer;

	m_CurDir = DOWN;
	m_PreDir = m_CurDir;
	m_DirVec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_CurState = IDLE;
	m_PreState = m_CurState;

	m_Pos = &m_pTransform->GetLocalPosition();
	m_PlayerPos = m_pPlayer->GetComponent<CTransform>();
	m_Hp = 3000.f;

}

void CBossScript::OnEnable()
{
}

void CBossScript::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move )
{

}

void CBossScript::OnInput()
{
}

int CBossScript::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}
	TrackPlayer();
	GetDirPlayer();
	AnimState();
	DirState();
	//if (m_CurState != STOMP)
	//{
	//	if (CKeyMgr::GetInstance()->KeyPressing(KEY_V))
	//	{
	//		m_CurState = STOMP;
	//	}
	//}
	//
	if (m_CurState == STOMP && !m_pAnimator->IsPlaying())
	{
		m_CurState = IDLE;
		m_PrePos = *m_pTransform->GetWorldPos();
		MakeIceSkillRoute();
		m_PrePlayerPos = *m_PlayerPos->GetWorldPos();
		m_bIceSkill = true;

		//m_BezierControl[0] = D3DXVECTOR3(m_PrePos.x+70.0f, m_PrePos.y -60.0f,0.0f);
		//m_BezierControl[1] = D3DXVECTOR3(m_PrePos.x+120.0f, m_PrePos.y -80.0f,0.0f);
		//m_BezierControl[2] = D3DXVECTOR3(m_PrePos.x-150.0f, m_PrePos.y -150.0f, 0.0f);
		//CEffect::CreateEffect<CBossIceEffect>(*m_pTransform->GetWorldPos(), XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Effect", LAYER_4);
	}

	if (m_bIceSkill)
	{
		if (m_fIceSkillTime > 1.f)
		{
			m_fIceSkillTime -= m_fIceSkillTime;
			m_bIceSkill = false;
		}
		else
		{
			if (m_fIceSkillSpawnCool > 0.1f)
			{
				
				D3DXVECTOR3 pos = BezierCuve4Dim(m_PrePos, m_PrePlayerPos, m_BezierControl[0], m_BezierControl[1], m_BezierControl[2], m_fIceSkillTime);
				// D3DXVECTOR3 pos =BezierCurve(m_PrePos, m_PrePlayerPos, m_BezierControl[1], m_fIceSkillTime);
				 if (pos != m_PrePlayerPos)
				 {
					 CEffect::CreateEffect<CBossFireBreath>(pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Effect", LAYER_1);
				 }
				m_fIceSkillSpawnCool -= m_fIceSkillSpawnCool;
			}
			m_fIceSkillSpawnCool += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fIceSkillTime += CTimeMgr::GetInstance()->GetDeltaTime();
		}
	}

	
	return 0;
}

void CBossScript::OnLateUpdate()
{
	//CEffect::CreateEffect<CBossIceEffect>(*playerPos, XMFLOAT3(0.0f, 0.0f, m_AttackAngle), D3DXVECTOR3(1.0f, 1.0f, 1.0f),L"Effect",LAYER_4);
}

void CBossScript::OnRender()
{
}

void CBossScript::OnDisable()
{
}

void CBossScript::OnDestroy()
{
}

void CBossScript::DirState()
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
			cout << "좌상단" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT_UP_45:
			cout << "우상단" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		case LEFT:
			cout << "좌측" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT:
			cout << "우측" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		case LEFT_DOWN_45:
			cout << "좌하단" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT_DOWN_45:
			cout << "우하단" << endl;
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		}
		m_PreDir = m_CurDir;
	}
}

void CBossScript::AnimState()
{
	if (m_CurState == HIT)
	{
		Hit();
	}
	//if (m_CurDir == LEFT_UP_45 || m_CurDir == LEFT_DOWN_45 || m_CurDir == RIGHT_DOWN_45 || m_CurDir == RIGHT_UP_45)
		//return;

	if (m_CurState != m_PreState)
	{
		switch (m_CurState)
		{
		case IDLE:
			m_pTexture->SetPass(0);
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				m_pAnimator->Play(L"Boss_Idle_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				m_pAnimator->Play(L"Boss_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Boss_Idle_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case DASH:
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				break;		
			case LEFT:
			case RIGHT:
				break;
			}
			break;
		case STOMP:
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				m_pAnimator->Play(L"Boss_Stomp_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				m_pAnimator->Play(L"Boss_Stomp_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
	
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Boss_Stomp_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case GRIND:
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				break;
			case LEFT:
			case RIGHT:
				break;
			}
			break;
		case THROWER:
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				break;
			case LEFT:
			case RIGHT:
				break;
			}
			break;
		case HIT:
			m_pTexture->SetPass(1);
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				m_pAnimator->Play(L"Boss_Hit_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				m_pAnimator->Play(L"Boss_Hit_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Boss_Hit_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;

		}
		m_PreState = m_CurState;
	}
}

void CBossScript::Move()
{
}

void CBossScript::GetHit(D3DXVECTOR3 dirVec, float power, float dmg)
{
	m_CurState = HIT;
	//m_DirVec = dirVec;
	//m_fVelocity = power;

	//m_Hp -= dmg;

	m_fWhiteValue = 0.2f;
	m_pTexture->SetFadeColor(XMFLOAT3(1.0f, 0.0f, 0.0f));
}

void CBossScript::MakeIceSkillRoute()
{
	random_device		rn;
	mt19937_64 rnd(rn());

	

	if (m_CurDir == UP)
	{
		uniform_real_distribution<float> control1X(-30.f, 30.f);
		uniform_real_distribution<float> control1Y(0, +60.0f);

		uniform_real_distribution<float> control2X(-70, 70.f);
		uniform_real_distribution<float> control2Y(+30.f, +80.f);

		uniform_real_distribution<float> control3X(-150.f, 150.0f);
		uniform_real_distribution<float> control3Y(+80.0f, +150.0f);

		m_BezierControl[0] = D3DXVECTOR3(m_PrePos.x + control1X(rnd), m_PrePos.y + control1Y(rnd), 0.0f);
		m_BezierControl[1] = D3DXVECTOR3(m_PrePos.x + control2X(rnd), m_PrePos.y + control2Y(rnd), 0.0f);
		m_BezierControl[2] = D3DXVECTOR3(m_PrePos.x + control3X(rnd), m_PrePos.y + control3Y(rnd), 0.0f);
	}
	else if (m_CurDir == DOWN)
	{
		uniform_real_distribution<float> control1X(-30.f, 30.f);
		uniform_real_distribution<float> control1Y(-60, 0.0f);

		uniform_real_distribution<float> control2X(-70, 70.f);
		uniform_real_distribution<float> control2Y(-80.f, -30.f);

		uniform_real_distribution<float> control3X(-150.f, -50.0f);
		uniform_real_distribution<float> control3Y(-150.0f, -80.0f);

		m_BezierControl[0] = D3DXVECTOR3(m_PrePos.x + control1X(rnd), m_PrePos.y + control1Y(rnd), 0.0f);
		m_BezierControl[1] = D3DXVECTOR3(m_PrePos.x + control2X(rnd), m_PrePos.y + control2Y(rnd), 0.0f);
		m_BezierControl[2] = D3DXVECTOR3(m_PrePos.x + control3X(rnd), m_PrePos.y + control3Y(rnd), 0.0f);
	}
	else if (m_CurDir == LEFT)
	{
		uniform_real_distribution<float> control1X(-30.f, -10.f);
		uniform_real_distribution<float> control1Y(-60.0f,60.f);

		uniform_real_distribution<float> control2X(-70, -40.f);
		uniform_real_distribution<float> control2Y(-120.f, 120.f);

		uniform_real_distribution<float> control3X(-170.f, -100.0f);
		uniform_real_distribution<float> control3Y(-150.0f, 150.0f);

		m_BezierControl[0] = D3DXVECTOR3(m_PrePos.x + control1X(rnd), m_PrePos.y + control1Y(rnd), 0.0f);
		m_BezierControl[1] = D3DXVECTOR3(m_PrePos.x + control2X(rnd), m_PrePos.y + control2Y(rnd), 0.0f);
		m_BezierControl[2] = D3DXVECTOR3(m_PrePos.x + control3X(rnd), m_PrePos.y + control3Y(rnd), 0.0f);
	}
	else if (m_CurDir == LEFT_UP_45)
	{
		uniform_real_distribution<float> control1X(-50.f, 10.f);
		uniform_real_distribution<float> control1Y(-10.0f, 60.f);

		uniform_real_distribution<float> control2X(-70, 30.f);
		uniform_real_distribution<float> control2Y(-20.f, 100.f);

		uniform_real_distribution<float> control3X(-170.f, 80.0f);
		uniform_real_distribution<float> control3Y(-40.0f, 180.0f);

		m_BezierControl[0] = D3DXVECTOR3(m_PrePos.x + control1X(rnd), m_PrePos.y + control1Y(rnd), 0.0f);
		m_BezierControl[1] = D3DXVECTOR3(m_PrePos.x + control2X(rnd), m_PrePos.y + control2Y(rnd), 0.0f);
		m_BezierControl[2] = D3DXVECTOR3(m_PrePos.x + control3X(rnd), m_PrePos.y + control3Y(rnd), 0.0f);
	}
	else if (m_CurDir == LEFT_DOWN_45)
	{
		uniform_real_distribution<float> control1X(-50.f, 10.f);
		uniform_real_distribution<float> control1Y(-60.0f, 10.f);

		uniform_real_distribution<float> control2X(-70, 30.f);
		uniform_real_distribution<float> control2Y(-100.f, 20.f);

		uniform_real_distribution<float> control3X(-170.f, 80.0f);
		uniform_real_distribution<float> control3Y(-180.0f, 40.0f);

		m_BezierControl[0] = D3DXVECTOR3(m_PrePos.x + control1X(rnd), m_PrePos.y + control1Y(rnd), 0.0f);
		m_BezierControl[1] = D3DXVECTOR3(m_PrePos.x + control2X(rnd), m_PrePos.y + control2Y(rnd), 0.0f);
		m_BezierControl[2] = D3DXVECTOR3(m_PrePos.x + control3X(rnd), m_PrePos.y + control3Y(rnd), 0.0f);
	}
	else if (m_CurDir == RIGHT)
	{
		uniform_real_distribution<float> control1X(40.f, 70.f);
		uniform_real_distribution<float> control1Y(-60.0f, 60.f);

		uniform_real_distribution<float> control2X(30, 120.f);
		uniform_real_distribution<float> control2Y(-120.f, 120.f);

		uniform_real_distribution<float> control3X(120.f, 200.0f);
		uniform_real_distribution<float> control3Y(-180.0f, 180.0f);

		m_BezierControl[0] = D3DXVECTOR3(m_PrePos.x + control1X(rnd), m_PrePos.y + control1Y(rnd), 0.0f);
		m_BezierControl[1] = D3DXVECTOR3(m_PrePos.x + control2X(rnd), m_PrePos.y + control2Y(rnd), 0.0f);
		m_BezierControl[2] = D3DXVECTOR3(m_PrePos.x + control3X(rnd), m_PrePos.y + control3Y(rnd), 0.0f);
	}
	else if (m_CurDir == RIGHT_UP_45)
	{
		uniform_real_distribution<float> control1X(-20.f, 40.f);
		uniform_real_distribution<float> control1Y(-40.0f, 80.f);

		uniform_real_distribution<float> control2X(-60, 120.f);
		uniform_real_distribution<float> control2Y(-80.f, 160.f);

		uniform_real_distribution<float> control3X(120.f, 240.0f);
		uniform_real_distribution<float> control3Y(-120.0f, 240.0f);

		m_BezierControl[0] = D3DXVECTOR3(m_PrePos.x + control1X(rnd), m_PrePos.y + control1Y(rnd), 0.0f);
		m_BezierControl[1] = D3DXVECTOR3(m_PrePos.x + control2X(rnd), m_PrePos.y + control2Y(rnd), 0.0f);
		m_BezierControl[2] = D3DXVECTOR3(m_PrePos.x + control3X(rnd), m_PrePos.y + control3Y(rnd), 0.0f);
	}
	else if (m_CurDir == RIGHT_DOWN_45)
	{
		uniform_real_distribution<float> control1X(-20.f, 40.f);
		uniform_real_distribution<float> control1Y(-80.0f, 40.f);

		uniform_real_distribution<float> control2X(-60, 120.f);
		uniform_real_distribution<float> control2Y(-160.f, 80.f);

		uniform_real_distribution<float> control3X(120.f, 240.0f);
		uniform_real_distribution<float> control3Y(-240.0f, 120.0f);

		m_BezierControl[0] = D3DXVECTOR3(m_PrePos.x + control1X(rnd), m_PrePos.y + control1Y(rnd), 0.0f);
		m_BezierControl[1] = D3DXVECTOR3(m_PrePos.x + control2X(rnd), m_PrePos.y + control2Y(rnd), 0.0f);
		m_BezierControl[2] = D3DXVECTOR3(m_PrePos.x + control3X(rnd), m_PrePos.y + control3Y(rnd), 0.0f);
	}

}

void CBossScript::Hit()
{
	if (m_fWhiteValue > 0.0f)
	{
		m_pTexture->SetValue(m_fWhiteValue*5.f);
		m_fWhiteValue -= CTimeMgr::GetInstance()->GetDeltaTime();
	}
	else
	{
		m_fWhiteValue = 0.2f;
		m_CurState = IDLE;
	}
}

void CBossScript::TrackPlayer()
{
	if (m_pPlayer != nullptr)
	{
		D3DXVECTOR3 botPos = *m_pTransform->GetWorldPos();
		D3DXVECTOR3 playerPos = *m_pPlayer->GetComponent<CTransform>()->GetWorldPos();
		float dist = sqrtf((playerPos.x - botPos.x)*(playerPos.x - botPos.x) + (playerPos.y - botPos.y)*(playerPos.y - botPos.y));
		m_fAngle = GetAngle(botPos, playerPos);
	}
}

void CBossScript::GetDirPlayer()
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
