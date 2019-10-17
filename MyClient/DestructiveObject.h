#pragma once
#include "Scripts.h"
class CTransform;
class CTexturRenderer;
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
public:
	CTexturRenderer*	m_pTexture;
	
	bool		m_bIsActive;
	float		m_Hp;

};

