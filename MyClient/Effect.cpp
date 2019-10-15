#include "stdafx.h"
#include "Effect.h"

#include "GameObject.h"
#include "TextureRenderer.h"
#include "Transform.h"
#include "Animator.h"
#include "EffectScript.h"

CEffect::CEffect()
{
}


CEffect::~CEffect()
{
}

void CEffect::Initialize()
{
	
	
}

void CEffect::SetPosition(const D3DXVECTOR3 & pos)
{
}

void CEffect::SetRotiation(const XMFLOAT3 & rot)
{
}

void CEffect::SetScale(const D3DXVECTOR3 & scale)
{
}

void CEffect::SetEffectClip(const wstring & _ClipName, ANIMATION_TYPE type)
{
}

void CEffect::SetDeadTime(const float & time)
{
}

CGameObject* CEffect::Create(D3DXVECTOR3 & pos, XMFLOAT3 & rot, D3DXVECTOR3 & scale, const wstring & _ClipName, const wstring& _AnimName, ANIMATION_TYPE type, const float & time)
{

	CGameObject* pEffectObj = new CGameObject;
	pEffectObj->SetObjectLayer(LAYER_5);
	pEffectObj->SetObjectTag(L"Effect");
	CTransform* m_pTransform = new CTransform;
	m_pTransform->Initialize(pEffectObj);
	m_pTransform->SetPosition(pos);
	m_pTransform->SetRotation(rot);
	m_pTransform->SetScaling(scale);
	pEffectObj->AddComponent(m_pTransform);

	CTextureRenderer* m_pTexture = new CTextureRenderer;
	m_pTexture->Initialize(pEffectObj);
	pEffectObj->AddComponent(m_pTexture);

	CAnimator* m_pAnimator = new CAnimator;
	m_pAnimator->Initialize(pEffectObj);
	m_pAnimator->LoadClips(_ClipName);
	pEffectObj->AddComponent(m_pAnimator);

	CEffectScript* pEffectScript = new CEffectScript;
	pEffectScript->SetGameObject(pEffectObj);
	pEffectScript->SetEffect(_AnimName,type,time);
	pEffectObj->AddScripts(pEffectScript);

	CObjectMgr::GetInstance()->AddObject(pEffectObj);

	return pEffectObj;
}
