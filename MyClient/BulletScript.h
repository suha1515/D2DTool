#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CBulletScript :
	public CScripts
{
public:
	CBulletScript();
	~CBulletScript();

	// CScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr) override;
	virtual void OnInput() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;

public:
	void SetAngle(const float& angle);
public:
	void SetSpeed(const float& speed);
public:
	static CBulletScript* Create(const float & angle, const float & speed, CGameObject* pGameObject);

private:
	CAnimator* pAnimator;
	CTransform* pTransform;

	float m_fVelocity;

	bool		bIsInit = false;

	float	m_fLifeTime;
	float	m_fAngle;
};

