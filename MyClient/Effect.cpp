#include "stdafx.h"
#include "Effect.h"

#include "GameObject.h"
#include "TextureRenderer.h"
#include "Transform.h"
#include "Animator.h"
#include "EffectScript.h"
#include "BoxCollider.h"

#include "BossIceEffect.h"

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

CGameObject * CEffect::CreateDashEffect(D3DXVECTOR3 pos, XMFLOAT3 & rot, D3DXVECTOR3 & scale,
	const wstring & _ClipName, const wstring & _AnimName, ANIMATION_TYPE type,
	const float & animSpeed, const float & time,
	const wstring & tagName, Layer layer
)
{
	CGameObject* pEffectObj = new CGameObject;
	pEffectObj->SetObjectLayer(layer);
	pEffectObj->SetObjectTag(tagName);
	CTransform* m_pTransform = new CTransform;
	m_pTransform->Initialize(pEffectObj);
	m_pTransform->SetPosition(pos);
	m_pTransform->SetRotation(rot);
	m_pTransform->SetScaling(scale);
	pEffectObj->AddComponent(m_pTransform);

	CTextureRenderer* m_pTexture = new CTextureRenderer;
	m_pTexture->Initialize(pEffectObj);
	m_pTexture->SetPass(3);
	m_pTexture->SetColorToAdd(D3DXVECTOR3(0.1f, 0.1f, 0.1f));
	pEffectObj->AddComponent(m_pTexture);

	CAnimator* m_pAnimator = new CAnimator;
	m_pAnimator->Initialize(pEffectObj);
	m_pAnimator->LoadClips(_ClipName);
	m_pAnimator->SetSpeed(animSpeed);
	pEffectObj->AddComponent(m_pAnimator);

	CEffectScript* pEffectScript = new CEffectScript;
	pEffectScript->SetGameObject(pEffectObj);
	pEffectScript->SetEffect(_AnimName, type, time);
	pEffectScript->SetType(CEffectScript::EFFECT_TYPE::DASH);
	pEffectObj->AddScripts(pEffectScript);

	CObjectMgr::GetInstance()->AddObject(pEffectObj);

	return pEffectObj;
}

CGameObject * CEffect::CreateMovable(D3DXVECTOR3  pos, XMFLOAT3 & rot, D3DXVECTOR3 & scale, const wstring & _ClipName, const wstring & _AnimName, ANIMATION_TYPE type, D3DXVECTOR3 dirVec, const float& speed,const float& animSpeed, const float & time, 
const int & colSizeX, const int & colSizeY, const int & coloffsetX, const int & coloffsetY, const wstring & tagName, Layer layer)
{
	CGameObject* pEffectObj = new CGameObject;
	pEffectObj->SetObjectLayer(layer);
	pEffectObj->SetObjectTag(tagName);
	CTransform* m_pTransform = new CTransform;
	m_pTransform->Initialize(pEffectObj);
	m_pTransform->SetPosition(pos);
	m_pTransform->SetRotation(rot);
	m_pTransform->SetScaling(scale);
	pEffectObj->AddComponent(m_pTransform);

	CTextureRenderer* m_pTexture = new CTextureRenderer;
	m_pTexture->Initialize(pEffectObj);
	pEffectObj->AddComponent(m_pTexture);

	if (colSizeX != 0 && colSizeY != 0)
	{
		CBoxCollider* m_pBoxCollider = new CBoxCollider;
		m_pBoxCollider->Initialize(pEffectObj);
		m_pBoxCollider->SetBoxSize(colSizeX, colSizeY);
		m_pBoxCollider->SetBoxOffset(coloffsetX, coloffsetY);
		m_pBoxCollider->SetBoxCollider();
		pEffectObj->AddComponent(m_pBoxCollider);
	}
	CAnimator* m_pAnimator = new CAnimator;
	m_pAnimator->Initialize(pEffectObj);
	m_pAnimator->LoadClips(_ClipName);
	m_pAnimator->SetSpeed(animSpeed);
	pEffectObj->AddComponent(m_pAnimator);

	CEffectScript* pEffectScript = new CEffectScript;
	pEffectScript->SetGameObject(pEffectObj);
	pEffectScript->SetEffect(_AnimName, type, time);
	pEffectScript->SetDir(dirVec);
	pEffectScript->SetType(CEffectScript::EFFECT_TYPE::MOVABLE);
	pEffectScript->SetSpeed(speed);
	pEffectObj->AddScripts(pEffectScript);

	CObjectMgr::GetInstance()->AddObject(pEffectObj);

	return pEffectObj;
}

CGameObject* CEffect::Create(D3DXVECTOR3  pos, XMFLOAT3 & rot, D3DXVECTOR3 & scale, const wstring & _ClipName, const wstring& _AnimName, ANIMATION_TYPE type, const float & time
	,const int& colSizeX, const int& colSizeY
	, const int& coloffsetX, const int& coloffsetY
	,const wstring& tagName,Layer layer)
{

	CGameObject* pEffectObj = new CGameObject;
	pEffectObj->SetObjectLayer(layer);
	pEffectObj->SetObjectTag(tagName);
	CTransform* m_pTransform = new CTransform;
	m_pTransform->Initialize(pEffectObj);
	m_pTransform->SetPosition(pos);
	m_pTransform->SetRotation(rot);
	m_pTransform->SetScaling(scale);
	pEffectObj->AddComponent(m_pTransform);

	CTextureRenderer* m_pTexture = new CTextureRenderer;
	m_pTexture->Initialize(pEffectObj);
	pEffectObj->AddComponent(m_pTexture);

	if (colSizeX != 0 && colSizeY != 0)
	{
		CBoxCollider* m_pBoxCollider = new CBoxCollider;
		m_pBoxCollider->Initialize(pEffectObj);
		m_pBoxCollider->SetBoxSize(colSizeX, colSizeY);
		m_pBoxCollider->SetBoxOffset(coloffsetX, coloffsetY);
		m_pBoxCollider->SetBoxCollider();
		pEffectObj->AddComponent(m_pBoxCollider);
	}
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
