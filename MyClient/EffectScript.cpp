#include "stdafx.h"
#include "EffectScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"

CEffectScript::CEffectScript()
{
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

	if (m_pAnimator != nullptr)
	{
		m_pAnimator->SetClips(clips);
		m_pAnimator->Play(clips, type);
		m_AnimType = type;
		m_fDeadTime = deadTime;
	}
}
