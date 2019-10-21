#include "stdafx.h"
#include "BossFireBreath.h"


#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "BoxCollider.h"
#include "Effect.h"

#include "PlayerScript.h"

CBossFireBreath::CBossFireBreath()
{
	m_FireCount = 0;
	m_fRadius = 30.f;
	m_fSpawnTimeEnd = 0.2f;
}


CBossFireBreath::~CBossFireBreath()
{
}

void CBossFireBreath::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Fire_Effect");
		m_pAnimator->SetClips(L"Fire_Explosion");
		m_pAnimator->Play(L"Fire_Explosion", ANIMATION_ONCE);
		m_fDeadTime = 3.0f;
	}
	m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pBoxCollider->SetBoxSize(30, 30);
	m_pBoxCollider->SetBoxCollider();

	m_bIsInit = true;
	m_pPlayer = CObjectMgr::GetInstance()->m_pPlayer;
}

void CBossFireBreath::OnEnable()
{
}

void CBossFireBreath::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move)
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
			script->GetHit(dir, 3.f, 10.f);
		}
	}
}

void CBossFireBreath::OnInput()
{
}

int CBossFireBreath::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}

	if (m_FireCount <7)
	{
		
		for(int i=0;i<7;++i)
		{
			random_device		rn;
			mt19937_64 rnd(rn());
			uniform_real_distribution<float> nSpawnTime(0.04f, 0.2f);
			uniform_real_distribution<float> nDir(-1.0f, 1.0f);
			D3DXVECTOR3 dir(nDir(rnd), nDir(rnd), 0.f);
			m_fSpawnTimeEnd = nSpawnTime(rnd);
			D3DXVECTOR3 randompos = dir*m_fRadius + *m_pTransform->GetWorldPos();
			CEffect::Create(randompos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Fire_Effect", L"Fire_Explosion", ANIMATION_ONCE, 1.5f,30, 30, 0, 0, L"Boss_FireBreath", LAYER_5);

			m_FireCount++;		
		}
	}
	else
	{
		m_pGameObject->SetObjectDestroy(true);
	}
	
	return 0;
}

void CBossFireBreath::OnLateUpdate()
{
}

void CBossFireBreath::OnRender()
{
}

void CBossFireBreath::OnDisable()
{
}

void CBossFireBreath::OnDestroy()
{
}
