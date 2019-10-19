#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CBoxCollider;
class CBulletScript :
	public CScripts
{
	
public:
	CBulletScript();
	~CBulletScript();
	 enum BULLET_TYPE { SMALL, CHARGED,TURRET,TURRET_CHARGE };
	// CScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr) override;
	virtual void OnInput() override;
	virtual int  OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;


public:
	void Move();
public:
	void SetAngle(const float& angle);
	void SetDirection(const D3DXVECTOR3& dir);
public:
	void SetSpeed(const float& speed);
	void SetBulletType(BULLET_TYPE type);
	void SetBulletDmg(const float& dmg);
public:
	void CheckTiles();
public:
	bool CollideTiles();
	bool CollideTilesLine();
public:
	static CBulletScript* Create(const float & angle,const float& damage,const float & speed, CGameObject* pGameObject,BULLET_TYPE type);

private:
	CAnimator* pAnimator;
	CTransform* pTransform;
	CBoxCollider* pBoxCollider;

	float m_fVelocity;

	bool		bIsInit = false;
	

	BULLET_TYPE m_BulletType;

	float	m_fLifeTime;
	float	m_fAngle;
	float	m_Damage;

	list <CGameObject*> m_NearTiles;

	D3DXVECTOR3			m_DirVec;		//방향벡터
	D3DXVECTOR3*		m_BulletPos;	//총알위치
	D3DXVECTOR3			m_PrePos;		//이전위치
};

