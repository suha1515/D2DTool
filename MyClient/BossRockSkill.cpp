#include "stdafx.h"
#include "BossRockSkill.h"


#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "BoxCollider.h"
#include "Effect.h"
#include "PlayerScript.h"

CBossRockSkill::CBossRockSkill()
{
}


CBossRockSkill::~CBossRockSkill()
{
}

void CBossRockSkill::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Boss");
		m_pAnimator->SetClips(L"Boss_Rock_LockOn");
		m_pAnimator->Play(L"Boss_Rock_LockOn", ANIMATION_LOOP);
		m_fDeadTime = 3.0f;
	}
	m_pBoxCollider->SetOn(false);
	m_pBoxCollider->SetBoxSize(30, 30);
	m_pBoxCollider->SetBoxCollider();
	
	m_CurState = LOCKING;
	m_PreState = m_CurState;

	m_bIsInit = true;
	m_bFlicker = false;
	m_fFlickerTIme = 0.0f;
	m_bRockFalling = false;
	m_fFallingTime = 0.0f;
	m_fRemainTime = 0.0f;
	m_fRadius = 35.f;

	m_bRemain = false;
	m_Pos = &m_pTransform->GetComponent<CTransform>()->GetLocalPosition();
	m_pPlayer = CObjectMgr::GetInstance()->m_pPlayer;


	m_fRemainTime = 0.0f;
}

void CBossRockSkill::OnEnable()
{
}

void CBossRockSkill::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move )
{
	if (pGameObject == m_pPlayer)
	{
		CPlayerScript* script = dynamic_cast<CPlayerScript*>(pGameObject->GetScript("CPlayerScript"));
		if (!script->m_bIsHit)
		{
			D3DXVECTOR3 effectPos = *m_pTransform->GetWorldPos();
			D3DXVECTOR3 playerPos = *m_pPlayer->GetComponent<CTransform>()->GetWorldPos();
			float m_fAngle = GetAngle(effectPos, playerPos);
			//360도로 변환하기 위한것
			if (m_fAngle < 0.0f)
				m_fAngle = m_fAngle + 360.f;
			D3DXVECTOR3 dir = D3DXVECTOR3(cosf(D3DXToRadian(m_fAngle)), sinf(D3DXToRadian(m_fAngle)), 0.0f);
			D3DXVec3Normalize(&dir, &dir);
			script->GetHit(dir, 6.f, 35.f);
		}
	}
}

void CBossRockSkill::OnInput()
{
}

int CBossRockSkill::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}

	AttackState();


	if (!m_bRockFalling)
	{
		
	}


	if (m_bRockFalling)
	{
		
	}

	
	
	return 0;
}

void CBossRockSkill::OnLateUpdate()
{
}

void CBossRockSkill::OnRender()
{
}

void CBossRockSkill::OnDisable()
{
}

void CBossRockSkill::OnDestroy()
{
}

void CBossRockSkill::AttackState()
{
	

	if (m_CurState != m_PreState)
	{
		switch (m_CurState)
		{
		case LOCKING:
			break;
		case FALLING:
			m_PreRockPos = D3DXVECTOR3(m_Pos->x, m_Pos->y + 200.f, 0.0f);
			m_pRock = CEffect::Create(m_PreRockPos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.f), L"Boss", L"Boss_Rock_Fragment", ANIMATION_LOOP, 0.5f, 0, 0, 0, 0, L"Effect", LAYER_1);
			m_RockPos = &m_pRock->GetComponent<CTransform>()->GetLocalPosition();
			break;
		case CRUSHING:
			CCameraMgr::GetInstance()->ShakeCamera(1.5f, 0.3f);
			break;
		case PARTICLE:
			break;
		case DEAD:
		default:
			break;
		}
		m_PreState = m_CurState;
	}
	if (m_CurState == PARTICLE)
	{
		Throw();
	}

	if (m_CurState == CRUSHING)
	{
		CEffect::Create(D3DXVECTOR3(m_Pos->x - 10.f, m_Pos->y + 10.f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.f), L"Dust_Effect", L"Dust_Very_Small", ANIMATION_ONCE, 1.0f, 0, 0, 0, 0, L"Effect", LAYER_1);
		CEffect::Create(D3DXVECTOR3(m_Pos->x - 10.f, m_Pos->y - 10.f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.f), L"Dust_Effect", L"Dust_Very_Small", ANIMATION_ONCE, 1.0f, 0, 0, 0, 0, L"Effect", LAYER_1);
		CEffect::Create(D3DXVECTOR3(m_Pos->x + 20.f, m_Pos->y - 20.f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.f), L"Dust_Effect", L"Dust_Very_Small", ANIMATION_ONCE, 1.0f, 0, 0, 0, 0, L"Effect", LAYER_1);
		CEffect::Create(D3DXVECTOR3(m_Pos->x + 20.f, m_Pos->y + 10.f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.f), L"Dust_Effect", L"Dust_Very_Small", ANIMATION_ONCE, 1.0f, 0, 0, 0, 0, L"Effect", LAYER_1);
		m_pBoxCollider->SetOn(true);
		m_CurState = PARTICLE;	
	}

	if (m_CurState == FALLING)
	{
		if (m_fFallingTime > 0.5f)
		{
			m_CurState = CRUSHING;
			m_fFallingTime -= m_fFallingTime;
			m_pBoxCollider->SetOn(true);
			Destruct();
		}
		else
		{
			D3DXVECTOR3 pos = Lerp(m_PreRockPos, *m_Pos, m_fFallingTime*2.0f);
			*m_RockPos = pos;
			m_fFallingTime += CTimeMgr::GetInstance()->GetDeltaTime();
		}
	}

	if (m_CurState == LOCKING)
	{
		if (m_bFlicker)
		{
			m_pTexture->SetOn(true);
		}
		else
			m_pTexture->SetOn(false);
		if (m_fLockOnTime > 2.0f)
		{
			m_CurState = FALLING;
			m_fFlickerTIme -= m_fFlickerTIme;
			m_pTexture->SetOn(false);
		}
		else
		{
			if (m_fFlickerTIme > 0.2f)
			{
				if (!m_bFlicker)
					m_bFlicker = true;
				else
					m_bFlicker = false;
				m_fFlickerTIme -= m_fFlickerTIme;
			}
			m_fFlickerTIme += CTimeMgr::GetInstance()->GetDeltaTime();
			m_fLockOnTime += CTimeMgr::GetInstance()->GetDeltaTime();
		}
	}

	

	

}

void CBossRockSkill::Destruct()
{
	random_device		rn;
	mt19937_64 rnd(rn());
	wstring name[8];
	name[0] = L"Boss_Rock_Fragment1";
	name[1] = L"Boss_Rock_Fragment1";
	name[2] = L"Boss_Rock_Fragment2";
	name[3] = L"Boss_Rock_Fragment2";
	name[4] = L"Boss_Rock_Fragment2";
	name[5] = L"Boss_Rock_Fragment3";
	name[6] = L"Boss_Rock_Fragment3";
	name[7] = L"Boss_Rock_Fragment3";

	for (int i = 0;i < 8; ++i)
	{
		uniform_real_distribution<float> nHeight(70.f, 100.f);

		uniform_real_distribution<float> nDir(-1.0f, 1.0f);

		uniform_real_distribution<float> nSpeed(5.f, 30.f);

		D3DXVECTOR3 dir(nDir(rnd), nDir(rnd), 0.f);

		D3DXVec3Normalize(&dir, &dir);
		D3DXVECTOR3 pos = *m_pTransform->GetWorldPos();
		D3DXVECTOR3 randompos = dir*m_fRadius + *m_pTransform->GetWorldPos();
		D3DXVECTOR3 jumpControl;

		float speed = nSpeed(rnd);

		if (randompos.x > pos.x)
			jumpControl = pos + (randompos - pos) / 2;
		else
			jumpControl = randompos + (pos - randompos) / 2;
		float randHeight = nHeight(rnd);
		jumpControl.y += randHeight;

		D3DXVECTOR3 newPos = D3DXVECTOR3(pos.x + 20.f, pos.y + 20.f, 0.0f);
		CGameObject* particle = CEffect::Create(pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Boss", name[i], ANIMATION_LOOP, 1.1f);
		m_fragment.push_back(tuple<CGameObject*, D3DXVECTOR3, D3DXVECTOR3, float>(particle, randompos, jumpControl, speed));
	}
}

void CBossRockSkill::Throw()
{
	if (m_fRemainTime > 1.0f)
	{
		m_fRemainTime -= m_fRemainTime;
		m_CurState = DEAD;
		m_pBoxCollider->SetOn(false);
		m_pGameObject->SetObjectDestroy(true);
		for (int i = 0; i < 8; ++i)
		{
			CGameObject* pGameObject = get<0>(m_fragment[i]);
			pGameObject->GetComponent<CTextureRenderer>()->SetOn(false);
		}
	}
	{
		for (int i = 0; i < 8; ++i)
		{
			CGameObject* pGameObject = get<0>(m_fragment[i]);
			D3DXVECTOR3 randPos = get<1>(m_fragment[i]);
			D3DXVECTOR3 jumpPos = get<2>(m_fragment[i]);
			D3DXVECTOR3 pos = *m_pTransform->GetWorldPos();
			D3DXVECTOR3 newPos = BezierCurve(pos, randPos, jumpPos, (m_fRemainTime));
			float		speed = get<3>(m_fragment[i]);	
			XMFLOAT3	rot = XMFLOAT3(0.0f, 0.0f, speed*m_fRemainTime);
			pGameObject->GetComponent<CTransform>()->SetPosition(newPos);
			pGameObject->GetComponent<CTransform>()->SetRotation(rot);
			pGameObject->GetComponent<CTextureRenderer>()->SetAlpha(1.0f - m_fRemainTime);
		}
		m_fRemainTime += CTimeMgr::GetInstance()->GetDeltaTime();
	}
}
