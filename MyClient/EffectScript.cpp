#include "stdafx.h"
#include "EffectScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "TextureRenderer.h"

CEffectScript::CEffectScript()
{
	m_EffectType = NORMAL;
}


CEffectScript::~CEffectScript()
{
}

void CEffectScript::OnInit()
{
	m_fTime = 0.0f;
}

void CEffectScript::OnEnable()
{
}

void CEffectScript::OnCollision(CGameObject * pGameObject , XMFLOAT2 * move)
{
	//L"Boss_FireThrower"

	
}

void CEffectScript::OnInput()
{
}

int CEffectScript::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}
		
	if (m_EffectType == MOVABLE)
	{
		m_Pos->x += m_DirVec.x*m_fSpeed*CTimeMgr::GetInstance()->GetDeltaTime();
		m_Pos->y += m_DirVec.y*m_fSpeed*CTimeMgr::GetInstance()->GetDeltaTime();
	}
	else if (m_EffectType == DASH)
	{
		m_pTexture->SetColorToAdd(m_DashColor);
		m_pTexture->SetAlpha(1.0f - (m_fTime / m_fDeadTime));
	}


	if (m_AnimType == ANIMATION_ONCE)
	{
		if (!m_pAnimator->IsPlaying())
			m_pGameObject->SetObjectDestroy(true);
	}
	else if (m_AnimType == ANIMATION_LOOP)
	{
		if (m_fTime > m_fDeadTime)
			m_pGameObject->SetObjectDestroy(true);

		m_fTime += CTimeMgr::GetInstance()->GetDeltaTime();
	}

	return 0;
}

void CEffectScript::OnLateUpdate()
{
}

void CEffectScript::OnRender()
{
}

void CEffectScript::OnDisable()
{
}

void CEffectScript::OnDestroy()
{
}

void CEffectScript::SetEffect(const wstring & clips, const ANIMATION_TYPE& type,const float& deadTime)
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->SetClips(clips);
		m_pAnimator->Play(clips, type);
		m_AnimType = type;
		m_fDeadTime = deadTime;
	}
	m_Pos = &m_pTransform->GetLocalPosition();
}

void CEffectScript::SetType(EFFECT_TYPE type)
{
	m_EffectType = type;
}

void CEffectScript::SetDir(D3DXVECTOR3 dir)
{
	m_DirVec = dir;
}

void CEffectScript::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

void CEffectScript::SetColor(D3DXVECTOR3 color)
{
	m_DashColor = color;
}
