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
#include "BossRockSkill.h"

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

	//플레이어 정보 얻기
	m_pPlayer = CObjectMgr::GetInstance()->m_pPlayer;
	playerPos = m_pPlayer->GetComponent<CTransform>()->GetWorldPos();

	m_CurDir = DOWN;
	m_PreDir = m_CurDir;
	m_DirVec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_CurState = IDLE;
	m_PreState = m_CurState;

	m_Pos = &m_pTransform->GetLocalPosition();
	m_PlayerPos = m_pPlayer->GetComponent<CTransform>();
	m_Hp = 3000.f;

	m_bIsHit = false;
	m_fHitCoolTime = 0.0f;

	m_bRockSkill = false;
	m_fRockSpawnTime = 0.0f;
	m_iRockCount = 0;
	m_fRadius = 30.f;

	m_Type = ICE;
}

void CBossScript::OnEnable()
{
}

void CBossScript::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move )
{
	if (pGameObject->GetObjectTag() == L"Player_Sweep")
	{
		if (!m_bIsHit)
		{
			GetHit(D3DXVECTOR3(0,0,0),10.f,20.f);
			m_bIsHit = true;
		}
			
	}
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

	if (m_bIsHit)
	{
		if (m_fHitCoolTime > 0.5f)
		{
			m_bIsHit = false;
			m_fHitCoolTime -= m_fHitCoolTime;
		}
		else
			m_fHitCoolTime += CTimeMgr::GetInstance()->GetDeltaTime();
	}
	
	AttackState();
	
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
			m_SkillPos = D3DXVECTOR3(m_Pos->x, m_Pos->y + 50.f, 0.0f);
			break;
		case DOWN:
			m_SkillPos = D3DXVECTOR3(m_Pos->x, m_Pos->y + 50.f, 0.0f);
			break;
		case LEFT_UP_45:
			m_SkillPos = D3DXVECTOR3(m_Pos->x-25.f, m_Pos->y+25.f, 0.0f);
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT_UP_45:
			m_SkillPos = D3DXVECTOR3(m_Pos->x+25.f, m_Pos->y + 25.f, 0.0f);
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		case LEFT:
			m_SkillPos = D3DXVECTOR3(m_Pos->x-50.f, m_Pos->y, 0.0f);
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT:
			m_SkillPos = D3DXVECTOR3(m_Pos->x+50.f, m_Pos->y, 0.0f);
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		case LEFT_DOWN_45:
			m_SkillPos = D3DXVECTOR3(m_Pos->x-25.f, m_Pos->y -25.f, 0.0f);
			m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT_DOWN_45:
			m_SkillPos = D3DXVECTOR3(m_Pos->x+25.f, m_Pos->y-25.f, 0.0f);
			m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		}
		m_PreDir = m_CurDir;
	}
}

void CBossScript::AnimState()
{
	if (m_CurState == HIT|| m_bIsHit)
	{
		Hit();
	}

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
		case DASH_READY:
			m_pTexture->SetFadeColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
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
			m_tempPos = *m_Pos;
			m_PrePlayerPos = *playerPos;
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				m_pAnimator->Play(L"Boss_Dash_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				m_pAnimator->Play(L"Boss_Dash_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;		
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Boss_Dash_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case DASH_STOP:
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				m_pAnimator->Play(L"Boss_Dash_Stop_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				m_pAnimator->Play(L"Boss_Dash_Stop_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Boss_Dash_Stop_Right", ANIMATION_TYPE::ANIMATION_ONCE);
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
		case GRIND_READY:
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				m_pAnimator->Play(L"Boss_Ready_Grind_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				m_pAnimator->Play(L"Boss_Ready_Grind_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Boss_Ready_Grind_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case GRIND:
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				m_pAnimator->Play(L"Boss_Attack_Grind_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				m_pAnimator->Play(L"Boss_Attack_Grind_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Boss_Attack_Grind_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case THROWER:
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
			case DEAD:

				break;
			}
			break;

		case DEAD:
			switch (m_CurDir)
			{
			case LEFT_UP_45:
			case RIGHT_UP_45:
			case UP:
				m_pAnimator->Play(L"Boss_Die_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
			case DOWN:
				m_pAnimator->Play(L"Boss_Die_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Boss_Die_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DEAD:

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
	if(m_CurState!=THROWER&&m_CurState!=GRIND&&m_CurState!=STOMP&&m_CurState!=DASH)
	m_CurState = HIT;
	//m_DirVec = dirVec;
	//m_fVelocity = power;

	//m_Hp -= dmg;

	m_fWhiteValue = 0.2f;
	m_pTexture->SetFadeColor(XMFLOAT3(1.0f, 0.0f, 0.0f));
	m_bIsHit = true;
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

void CBossScript::AttackState()
{

	StompSkill();
	GrindSkill();
	ThrowerSkill();
	DashSkill();
	DashReady();
	DeadEffect();
	if (m_CurState != STOMP&&m_CurState!=GRIND)
	{
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_V))
		{
			m_CurState = STOMP;
		}
		else if (CKeyMgr::GetInstance()->KeyPressing(KEY_P))
			m_CurState = GRIND_READY;
		else if (CKeyMgr::GetInstance()->KeyPressing(KEY_O))
			m_CurState = DASH_READY;
		else if (CKeyMgr::GetInstance()->KeyPressing(KEY_I))
			m_CurState = DEAD;
	}
	if (m_CurState == DASH_STOP && !m_pAnimator->IsPlaying())
	{
		m_CurState = IDLE;
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
		if(m_CurState==HIT)
			m_CurState = IDLE;
		m_bIsHit = false;
	}
}



void CBossScript::StompSkill()
{
	if (m_CurState == STOMP && !m_pAnimator->IsPlaying())
	{
		m_CurState = IDLE;
		m_PrePos = m_SkillPos;
		MakeIceSkillRoute();
		m_bStompSkill = true;
		if(m_CurDir==RIGHT||m_CurDir==RIGHT_UP_45||m_CurDir==RIGHT_DOWN_45)
		CEffect::Create(m_SkillPos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 2.0f, 0.0f), L"Dust_Effect", L"Dust_Big",ANIMATION_ONCE, 0,0,0,0,0,L"Effect",LAYER_2);
		else
		CEffect::Create(m_SkillPos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-2.0f, 2.0f, 0.0f), L"Dust_Effect", L"Dust_Big", ANIMATION_ONCE, 0, 0, 0, 0, 0, L"Effect", LAYER_2);
	}

	if (m_bStompSkill)
	{
		if (m_fIceSkillTime > 1.5f)
		{
			m_fIceSkillTime -= m_fIceSkillTime;
			m_bStompSkill = false;

		}
		else
		{
			if (m_fIceSkillSpawnCool > 0.1f)
			{
				D3DXVECTOR3 pos = BezierCuve4Dim(m_PrePos, *playerPos, m_BezierControl[0], m_BezierControl[1], m_BezierControl[2], m_fIceSkillTime/1.5f);
				if (pos != m_PrePlayerPos)
				{
					if (m_Type == ICE)
						CEffect::CreateEffect<CBossIceEffect>(pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Effect", LAYER_1);
					else
						CEffect::CreateEffect<CBossFireBreath>(pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Effect", LAYER_1);
				}
				m_fIceSkillSpawnCool -= m_fIceSkillSpawnCool;
			}
			m_fIceSkillSpawnCool += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fIceSkillTime += CTimeMgr::GetInstance()->GetDeltaTime();
		}
	}
}

void CBossScript::GrindSkill()
{
	if (m_CurState == GRIND_READY && !m_pAnimator->IsPlaying())
	{
		m_CurState = GRIND;
		m_bGrindSkill = true;
	}		
	if (m_bGrindSkill)
	{
		if (m_fFireGrindSkillTime < 1.5f)
		{
			if (m_fFireGrindSkillSpawnCool > 0.1f)
			{
				D3DXVECTOR3 dir = D3DXVECTOR3(cosf(D3DXToRadian(m_fAngle)), sinf(D3DXToRadian(m_fAngle)), 0.0f);
				CEffect::CreateMovable(m_SkillPos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 1.0f), L"Fire_Effect", L"Fire_Explosion", ANIMATION_ONCE, dir, 400.f, 0.5f, 0, 30, 30, 0, 0, L"Boss_FireThrower", LAYER_5);
				m_fFireGrindSkillSpawnCool -= m_fFireGrindSkillSpawnCool;
			}
			m_fFireGrindSkillSpawnCool += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fFireGrindSkillTime += CTimeMgr::GetInstance()->GetDeltaTime();
		}
		else
		{
			m_CurState = IDLE;
			m_fFireGrindSkillTime -= m_fFireGrindSkillTime;
			m_fFireGrindSkillSpawnCool -= m_fFireGrindSkillSpawnCool;
			m_bGrindSkill = false;
		}
	}
}

void CBossScript::ThrowerSkill()
{
	if (m_CurState == THROWER&&!m_pAnimator->IsPlaying())
	{
		if (!m_bDustOnce)
		{
			if (m_CurDir == RIGHT || m_CurDir == RIGHT_UP_45 || m_CurDir == RIGHT_DOWN_45)
				CEffect::Create(m_SkillPos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 2.0f, 0.0f), L"Dust_Effect", L"Dust_Big", ANIMATION_ONCE, 0, 0, 0, 0, 0, L"Effect", LAYER_2);
			else
				CEffect::Create(m_SkillPos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-2.0f, 2.0f, 0.0f), L"Dust_Effect", L"Dust_Big", ANIMATION_ONCE, 0, 0, 0, 0, 0, L"Effect", LAYER_2);
			m_bDustOnce = true;
		}

		if (m_iRockCount < 10)
		{
			if (m_fRockSpawnTime > 0.5f)
			{
				random_device	rn;
				mt19937_64 rnd(rn());
				uniform_real_distribution<float> nDir(-1.0f, 1.0f);

				D3DXVECTOR3 dir(nDir(rnd), nDir(rnd), 0.f);

				D3DXVec3Normalize(&dir, &dir);

				D3DXVECTOR3 pos = dir*m_fRadius + (*playerPos);
				CEffect::CreateEffect<CBossRockSkill>(pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Effect", LAYER_1);
				m_iRockCount++;
				m_fRockSpawnTime -= m_fRockSpawnTime;
			}
			else
			{
				m_fRockSpawnTime += CTimeMgr::GetInstance()->GetDeltaTime();
			}
		}
		else
		{
			m_fRockSpawnTime = 0.0f;
			m_iRockCount = 0;
			m_CurState = IDLE;
			m_bDustOnce = false;
		}
	}
}

void CBossScript::DashSkill()
{
	if (m_CurState == DASH)
	{
		if (m_fDashTime > 0.5f)
		{
			m_fDashTime -= m_fDashTime;
			m_CurState = DASH_STOP;
			m_fShadowSpawnTime -= m_fShadowSpawnTime;
		}
		else
		{
			if (m_fShadowSpawnTime > 0.05f)
			{
				if (m_CurDir == RIGHT || m_CurDir == RIGHT_UP_45 || m_CurDir == RIGHT_DOWN_45)
					CEffect::CreateDashEffect(*m_Pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Boss", L"Boss_Dash_Right", ANIMATION_LOOP, 1.0f, 1.0f, L"Boss_Dash", LAYER_1);
				else if(m_CurDir == LEFT || m_CurDir == LEFT_UP_45 || m_CurDir == LEFT_DOWN_45)
					CEffect::CreateDashEffect(*m_Pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 1.0f, 1.0f), L"Boss", L"Boss_Dash_Right", ANIMATION_LOOP, 1.0f, 1.0f, L"Boss_Dash", LAYER_1);
				else if(m_CurDir==UP)
					CEffect::CreateDashEffect(*m_Pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Boss", L"Boss_Dash_Up", ANIMATION_LOOP, 1.0f, 1.0f, L"Boss_Dash", LAYER_1);
				else if(m_CurDir==DOWN)
					CEffect::CreateDashEffect(*m_Pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Boss", L"Boss_Dash_Down", ANIMATION_LOOP, 1.0f, 1.0f, L"Boss_Dash", LAYER_1);
				m_fShadowSpawnTime -= m_fShadowSpawnTime;
			}
			m_fShadowSpawnTime += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fDashTime += CTimeMgr::GetInstance()->GetDeltaTime();
			D3DXVECTOR3 pos = Lerp(m_tempPos, m_PrePlayerPos, m_fDashTime*2.f);
			*m_Pos = pos;
		}
	}
}

void CBossScript::DashReady()
{
	if (m_CurState == DASH_READY)
	{
		if (m_iFlickerCount < 6)
		{
			if (m_fFlickerTime > 0.5f)
			{
				m_fFlickerTime -= m_fFlickerTime;
				m_iFlickerCount++;
			}
			else
			{
				m_fFlickerTime += CTimeMgr::GetInstance()->GetDeltaTime();
				m_pTexture->SetPass(1);
				m_pTexture->SetValue(1.0f - (m_fFlickerTime * 2.f));
			}		
		}
		else
		{
			m_pTexture->SetPass(0);
			m_iFlickerCount = 0;
			m_CurState = DASH;
		}
	}
}

void CBossScript::DeadEffect()
{
	if (m_CurState == DEAD)
	{
		if (m_fDeadEffTime > 5.0f)
		{
			for (int i = 0; i < 10; ++i)
			{
				random_device	rn;
				mt19937_64 rnd(rn());
				uniform_real_distribution<float> nDir(-1.0f, 1.0f);

				D3DXVECTOR3 dir(nDir(rnd), nDir(rnd), 0.f);

				D3DXVec3Normalize(&dir, &dir);

				D3DXVECTOR3 pos = dir*20.f + (*m_Pos);
				CEffect::Create(pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Ice_Effect", L"Ice_Explosive", ANIMATION_ONCE, 1.0f, 0, 0, 0, 0, L"Boss_DIE", LAYER_1);
			}	
			m_pGameObject->SetObjectDestroy(true);
		}
		else
		{
			if (m_fExploSpawTime > 0.2f)
			{
				random_device	rn;
				mt19937_64 rnd(rn());
				uniform_real_distribution<float> nDir(-1.0f, 1.0f);

				D3DXVECTOR3 dir(nDir(rnd), nDir(rnd), 0.f);

				D3DXVec3Normalize(&dir, &dir);

				D3DXVECTOR3 pos = dir*20.f + (*m_Pos);
				CEffect::Create(pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Explosion_Round", L"Explosion_Big", ANIMATION_ONCE, 1.0f, 0, 0, 0, 0, L"Boss_DIE", LAYER_1);
				m_fExploSpawTime -= m_fExploSpawTime;
			}
			m_fDeadEffTime += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fExploSpawTime += CTimeMgr::GetInstance()->GetDeltaTime();	
		}
	}
}

void CBossScript::TrackPlayer()
{
	if (m_pPlayer != nullptr)
	{
		D3DXVECTOR3 botPos = *m_pTransform->GetWorldPos();
		
		float dist = sqrtf((playerPos->x - botPos.x)*(playerPos->x - botPos.x) + (playerPos->y - botPos.y)*(playerPos->y - botPos.y));
		m_fAngle = GetAngle(botPos, *playerPos);
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
