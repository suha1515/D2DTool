#pragma once

#include "TextureRenderer.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"

class CGameObject;
class CTextureRenderer;
class CTransform;
class CAnimator;
class CEffect
{
public:
	CEffect();
	~CEffect();
public:
	void Initialize();

public:
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotiation(const XMFLOAT3& rot);
	void SetScale(const D3DXVECTOR3& scale);

public:

	void SetEffectClip(const wstring& _ClipName, ANIMATION_TYPE type);
	void SetDeadTime(const float& time);
	static CGameObject* CreateMovable(D3DXVECTOR3  pos, XMFLOAT3 & rot, D3DXVECTOR3 & scale, const wstring & _ClipName, 
	const wstring& _AnimName, ANIMATION_TYPE type,
		D3DXVECTOR3 dirVec, const float& speed, const float& animSpeed=1.0f,
		const float & time = 0, const int& colSizeX = 0, const int& colSizeY = 0,
		const int& coloffsetX = 0, const int& coloffsetY = 0,
		const wstring& tagName = L"Effect", Layer layer = LAYER_5);
	static CGameObject* Create(D3DXVECTOR3  pos, XMFLOAT3 & rot, D3DXVECTOR3 & scale, const wstring & _ClipName, const wstring& _AnimName, ANIMATION_TYPE type, const float & time=0, const int& colSizeX = 0, const int& colSizeY = 0, const int& coloffsetX = 0, const int& coloffsetY = 0, const wstring& tagName = L"Effect", Layer layer =LAYER_5);
	template<typename T>
	static CGameObject* CreateEffect(D3DXVECTOR3 & pos, XMFLOAT3 & rot, D3DXVECTOR3 & scale, const wstring& tagName = L"Effect", Layer layer = LAYER_5);


private:

};

template<typename T>
inline CGameObject * CEffect::CreateEffect(D3DXVECTOR3 & pos, XMFLOAT3 & rot, D3DXVECTOR3 & scale, const wstring& tagName,Layer layer)
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

	CBoxCollider* m_pBoxCollider = new CBoxCollider;
	m_pBoxCollider->Initialize(pEffectObj);
	pEffectObj->AddComponent(m_pBoxCollider);

	CAnimator* m_pAnimator = new CAnimator;
	m_pAnimator->Initialize(pEffectObj);
	pEffectObj->AddComponent(m_pAnimator);

	T* pEffectScript = new T;
	pEffectScript->SetGameObject(pEffectObj);
	pEffectScript->OnInit();
	pEffectObj->AddScripts(pEffectScript);

	CObjectMgr::GetInstance()->AddObject(pEffectObj);

	return pEffectObj;
}
