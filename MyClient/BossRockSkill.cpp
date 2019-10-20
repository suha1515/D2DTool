#include "stdafx.h"
#include "BossRockSkill.h"


#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "BoxCollider.h"
#include "Effect.h"

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
	m_pBoxCollider->SetBoxSize(20, 20);
	m_pBoxCollider->SetBoxCollider();
	
	m_bIsInit = true;
	m_bFlicker = false;
	m_fFlickerTIme = 0.0f;

}

void CBossRockSkill::OnEnable()
{
}

void CBossRockSkill::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move )
{
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

	if (m_bFlicker)
	{
		m_pTexture->SetOn(true);
	}
	else
		m_pTexture->SetOn(false);
	if (m_fLockOnTime > 2.0f)
	{
		m_pGameObject->SetObjectDestroy(true);
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
