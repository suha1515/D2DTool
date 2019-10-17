#pragma once
#include "Scripts.h"
class CTransform;
class CTextureRenderer;
class CBoxCollider;
class CDestructiveObject :
	public CScripts
{
public:
	CDestructiveObject();
	~CDestructiveObject();

	// CScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr) override;
	virtual void OnInput() override;
	virtual int OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;

public:
	void Hit();
	void Destruct();
	void Throw();
public:
	CTextureRenderer*	m_pTexture;
	CTransform*			m_pTransform;
	CBoxCollider*		m_pBoxCollider;
	bool		m_bIsActive;

	bool		m_bIsHit;

	bool		m_bIsGone;

	float		m_Hp;

	float		m_fWhiteValue;

	int			m_iHitCount;

	float		m_Radius;

	float		m_fThrowTime;
	vector<tuple<CGameObject*,D3DXVECTOR3, D3DXVECTOR3>> m_fragment;

};


