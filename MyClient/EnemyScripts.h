#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CBoxCollider;
class CEnemyScripts :
	public CScripts
{
public:
	CEnemyScripts();
	~CEnemyScripts();
public:
	virtual void				DirState()=0;
	virtual void				AnimState()=0;
	virtual void				Move() = 0;
public:
	virtual void				GetHit(D3DXVECTOR3 dirVec,float power,float dmg) = 0;
	void						CheckTiles();		//타일확인
	bool						CollideTiles();		//타일과 충돌
protected:
	CTransform*			m_pTransform;		//트랜스폼
	CAnimator*			m_pAnimator;		//애니메이션
	CBoxCollider*		m_pBoxCollider;

	ENEMY_TYPE			m_Type;

	float				m_fAcc;
	float				m_fVelocity;
	float				m_Hp;
	float				m_Atk;

	D3DXVECTOR3			m_PrePos;

	list <CGameObject*> m_NearTiles;		//타일 충돌을 위한것.
	bool				m_IsActive;
public:
	void				SetOn(bool on);
	static CEnemyScripts* Create(CGameObject *pGameObject, ENEMY_TYPE type);
};

