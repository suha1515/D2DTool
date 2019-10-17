#include "stdafx.h"
#include "TurretScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "BulletScript.h"

#include "TextureRenderer.h"

#include "Effect.h"

CTurretScript::CTurretScript()
{
	m_ChargeEffect = nullptr;

}


CTurretScript::~CTurretScript()
{
}

void CTurretScript::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();

	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Turret");
		//초기 클립세트 설정
		m_pAnimator->Play(L"Turret_Aim_Down", ANIMATION_TYPE::ANIMATION_LOOP);
	}

	m_pPlayer = CObjectMgr::GetInstance()->m_pPlayer;

	m_CurDir = DOWN;
	m_PreDir = m_CurDir;
	m_DirVec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_CurState = IDLE;
	m_PreState = m_CurState;

	m_Pos = &m_pTransform->GetLocalPosition();

	m_Hp = 100.f;
	m_fChargeCool = 0.0f;
	m_fCoolTime = 0.0f;
}

void CTurretScript::OnEnable()
{
}

void CTurretScript::OnCollision(CGameObject * pGameObject , XMFLOAT2 * move)
{
}

void CTurretScript::OnInput()
{
}

int CTurretScript::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}

	if (m_Hp < 0.0f)
	{
		m_pGameObject->SetObjectDestroy(true);
		CEffect::Create(*m_Pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 1.0f), L"Explosion_Effect", L"Small_Explosion", ANIMATION_ONCE);
		return 0;
	}

	TrackPlayer();
	GetDirPlayer();
	DirState();
	AttackState();
	AnimState();

	return 0;
}

void CTurretScript::OnLateUpdate()
{
}

void CTurretScript::OnRender()
{
}

void CTurretScript::OnDisable()
{
}

void CTurretScript::OnDestroy()
{
}

void CTurretScript::DirState()
{
	//360도로 변환하기 위한것
	if (m_fAngle < 0.0f)
		m_fAngle = m_fAngle + 360.f;

	//우측 
	if (m_fAngle > 0.0f&&m_fAngle < 11.25f)
	{
		m_CurDir = RIGHT;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x + 10.f, m_Pos->y,0.0f);
	}
	//우측 나머지
	else if (m_fAngle > 348.75f&&m_fAngle < 360.f)
	{
		m_CurDir = RIGHT;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x + 10.f, m_Pos->y, 0.0f);
	}	
	//우측 상단
	else if (m_fAngle > 11.25f&&m_fAngle < 33.75f)
	{
		m_CurDir = RIGHT_22;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x + 7.5f, m_Pos->y+2.5f, 0.0f);
	}
	//우측 상단2 
	else if (m_fAngle > 33.75f&&m_fAngle < 56.25f)
	{
		m_CurDir = RIGHT_45;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x + 5.f, m_Pos->y+5.f, 0.0f);
	}
	//상단
	else if (m_fAngle > 56.25f&&m_fAngle < 78.75f)
	{
		m_CurDir = RIGHT_67;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x + 2.5f, m_Pos->y+7.5f, 0.0f);
	}
	//좌측 상단
	else if (m_fAngle > 78.75f&&m_fAngle < 101.25f)
	{
		m_CurDir = UP;
		m_BarrelPos = D3DXVECTOR3(m_Pos->x, m_Pos->y + 10.f, 0.0f);
	}
	//좌측
	else if (m_fAngle > 101.25f&&m_fAngle < 123.75f)
	{
		m_CurDir = LEFT_112;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x-2.5f, m_Pos->y + 7.5f, 0.0f);
	}
	//좌측 하단
	else if (m_fAngle > 123.75f&&m_fAngle < 146.25f)
	{
		m_CurDir = LEFT_135;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x - 5.0f, m_Pos->y + 5.0f, 0.0f);
	}
	//하단
	else if (m_fAngle > 146.25f&&m_fAngle < 168.75f)
	{
		m_CurDir = LEFT_157;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x - 7.5f, m_Pos->y + 2.5f, 0.0f);
	}
	//우측하단
	else if (m_fAngle > 168.75f&&m_fAngle < 191.25f)
	{
		m_CurDir = LEFT;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x - 10.f, m_Pos->y, 0.0f);
	}
	else if (m_fAngle > 191.25f&&m_fAngle < 213.75f)
	{
		m_CurDir = LEFT_202;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x - 7.5f, m_Pos->y-2.5f, 0.0f);
	}
	else if (m_fAngle > 213.75f&&m_fAngle < 236.25f)
	{
		m_CurDir = LEFT_225;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x - 5.0f, m_Pos->y - 5.0f, 0.0f);
	}
	else if (m_fAngle > 236.25f&&m_fAngle < 258.75f)
	{
		m_CurDir = LEFT_247;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x - 2.5f, m_Pos->y - 7.5f, 0.0f);
	}
	else if (m_fAngle > 258.75f&&m_fAngle < 281.25f)
	{
		m_CurDir = DOWN;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x, m_Pos->y - 10.f, 0.0f);
	}
	else if (m_fAngle > 281.25f&&m_fAngle < 303.75f)
	{
		m_CurDir = RIGHT_292;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x + 2.5f, m_Pos->y - 7.5f, 0.0f);
	}
	else if (m_fAngle > 303.75f&&m_fAngle < 326.25f)
	{
		m_CurDir = RIGHT_315;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x + 5.0f, m_Pos->y - 5.0f, 0.0f);
	}
	else if (m_fAngle > 326.25f&&m_fAngle < 348.75f)
	{
		m_CurDir = RIGHT_337;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_BarrelPos = D3DXVECTOR3(m_Pos->x + 7.5f, m_Pos->y - 2.5f, 0.0f);
	}
}

void CTurretScript::AnimState()
{

	if (m_CurState != m_PreState || m_CurDir != m_PreDir)
	{
		switch (m_CurState)
		{
		case HIT:
			m_pTexture->SetPass(1);
			break;
		case AIM:
			m_pTexture->SetPass(0);
		case IDLE:
			//cout << "대기상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Turret_Aim_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				m_pAnimator->Play(L"Turret_Aim_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_112:
			case RIGHT_67:
				m_pAnimator->Play(L"Turret_Aim_Right_Up3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_135:
			case RIGHT_45:
				m_pAnimator->Play(L"Turret_Aim_Right_Up2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_157:
			case RIGHT_22:
				m_pAnimator->Play(L"Turret_Aim_Right_Up1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Turret_Aim_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_202:
			case RIGHT_337:
				m_pAnimator->Play(L"Turret_Aim_Right_Down1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_225:
			case RIGHT_315:
				m_pAnimator->Play(L"Turret_Aim_Right_Down2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_247:
			case RIGHT_292:
				m_pAnimator->Play(L"Turret_Aim_Right_Down3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case SHOOT:
			cout << "발사상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Turret_Aim_Up_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				m_pAnimator->Play(L"Turret_Aim_Down_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_112:
			case RIGHT_67:
				m_pAnimator->Play(L"Turret_Aim_Right_Up3_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_135:
			case RIGHT_45:
				m_pAnimator->Play(L"Turret_Aim_Right_Up2_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_157:
			case RIGHT_22:
				m_pAnimator->Play(L"Turret_Aim_Right_Up1_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Turret_Aim_Right_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_202:
			case RIGHT_337:
				m_pAnimator->Play(L"Turret_Aim_Right_Down1_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_225:
			case RIGHT_315:
				m_pAnimator->Play(L"Turret_Aim_Right_Down2_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_247:
			case RIGHT_292:
				m_pAnimator->Play(L"Turret_Aim_Right_Down3_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case CHARGE:
			//cout << "대기상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Turret_Charge_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				m_pAnimator->Play(L"Turret_Charge_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_112:
			case RIGHT_67:
				m_pAnimator->Play(L"Turret_Charge_Right_Up3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_135:
			case RIGHT_45:
				m_pAnimator->Play(L"Turret_Charge_Right_Up2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_157:
			case RIGHT_22:
				m_pAnimator->Play(L"Turret_Charge_Right_Up1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Turret_Charge_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_202:
			case RIGHT_337:
				m_pAnimator->Play(L"Turret_Charge_Right_Down1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_225:
			case RIGHT_315:
				m_pAnimator->Play(L"Turret_Charge_Right_Down2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_247:
			case RIGHT_292:
				m_pAnimator->Play(L"Turret_Charge_Right_Down3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case CHARGE_SHOOT:
			//cout << "대기상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_112:
			case RIGHT_67:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Up3", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_135:
			case RIGHT_45:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Up2", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_157:
			case RIGHT_22:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Up1", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_202:
			case RIGHT_337:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Down1", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_225:
			case RIGHT_315:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Down2", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_247:
			case RIGHT_292:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Down3", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		}

		m_PreState = m_CurState;
		m_PreDir = m_CurDir;
	}
}

void CTurretScript::Move()
{
}

void CTurretScript::GetHit(D3DXVECTOR3 dirVec, float power, float dmg)
{
	m_CurState = HIT;

	m_Hp -= dmg;

	m_fWhiteValue = 0.0f;

}

void CTurretScript::Hit()
{
	if (m_fWhiteValue < 0.5f)
	{
		m_pTexture->SetAlpha(m_fWhiteValue*2.f);
		m_fWhiteValue += CTimeMgr::GetInstance()->GetDeltaTime();
	}
	else
	{
		m_fWhiteValue -= m_fWhiteValue;
		m_CurState = AIM;
	}
}

void CTurretScript::AttackState()
{
		switch (m_CurState)
		{
		case AIM:
			if (m_fCoolTime >= 2.0f)
			{
				m_fCoolTime -= m_fCoolTime;
				m_CurState = SHOOT;
			}
			m_fCoolTime += CTimeMgr::GetInstance()->GetDeltaTime();
			break;
		case SHOOT:
			if (m_CurState == SHOOT && !m_pAnimator->IsPlaying())
			{
				Shoot(SINGLE);
				m_CurState = AIM;
			}
			break;
		case CHARGE:			
			if (m_fChargeCool > 3.0f)
			{
				m_CurState = CHARGE_SHOOT;
				m_fChargeCool -= m_fChargeCool;
				m_ChargeEffect = nullptr;
			}
			else
			{
				if (m_ChargeEffect == nullptr)
				{
					m_ChargeEffect = CEffect::Create(m_BarrelPos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Turret_Effect", L"Turrect_Charge", ANIMATION_LOOP, 3.0f);
				}
				else
				{
					m_ChargeEffect->GetComponent<CTransform>()->SetPosition(m_BarrelPos);
				}
				m_fChargeCool += CTimeMgr::GetInstance()->GetDeltaTime();
			}
			break;
		case CHARGE_SHOOT:
				Shoot(CHARGED);
				m_CurState = AIM;	
				break;
		case HIT:
			if (m_CurState == HIT)
			{
				Hit();
			}
			break;
		}
}

void CTurretScript::GetDirPlayer()
{
}

void CTurretScript::TrackPlayer()
{
	if (m_pPlayer != nullptr)
	{
		D3DXVECTOR3 botPos = *m_pTransform->GetWorldPos();
		D3DXVECTOR3 playerPos = *m_pPlayer->GetComponent<CTransform>()->GetWorldPos();
		float dist = sqrtf((playerPos.x - botPos.x)*(playerPos.x - botPos.x) + (playerPos.y - botPos.y)*(playerPos.y - botPos.y));
		
		m_fAngle = GetAngle(botPos, playerPos);
		D3DXVECTOR3 dir = playerPos - botPos;
		D3DXVec3Normalize(&m_DirVec, &dir);

		if (m_CurState != CHARGE&&m_CurState!=CHARGE_SHOOT&&m_CurState!=SHOOT&&m_CurState != HIT)
		{
			if (m_pPlayer->GetObjectLayer() == m_pGameObject->GetObjectLayer())
			{
				if (dist < 150.f&&dist>50.f)
				{
					m_CurState = AIM;
				}
				else if (dist < 50.f)
				{
				}
				else if (dist > 150.f&&dist < 200.f)
				{
					if (m_fChargeCool > 2.0f)
					{
						m_CurState = CHARGE;
						m_fChargeCool -= m_fChargeCool;
					}
					m_fChargeCool += CTimeMgr::GetInstance()->GetDeltaTime();
				}
				else
					m_CurState = IDLE;

			}
		}
	}
}

void CTurretScript::Shoot(FIRE_MODE type)
{
	CGameObject* pBullet = nullptr;
	if (type == SINGLE)
	{
		pBullet = CObjectMgr::GetInstance()->AddCopy(L"Turret_Small_Bullet", L"Turret_Bullet");
		pBullet->AddScripts(CBulletScript::Create(m_fAngle, 20.f, 200.f, pBullet, CBulletScript::BULLET_TYPE::TURRET));
	}
	else if (type == CHARGED)
	{
		pBullet = CObjectMgr::GetInstance()->AddCopy(L"Turret_Small_Bullet", L"Turret_Bullet");
		pBullet->AddScripts(CBulletScript::Create(m_fAngle, 20.f, 200.f, pBullet, CBulletScript::BULLET_TYPE::TURRET_CHARGE));
	}
	if (pBullet != nullptr)
	{
		pBullet->GetComponent<CTransform>()->SetPosition(m_BarrelPos);
		pBullet->SetObjectLayer(m_pGameObject->GetObjectLayer());
	}
	
}
