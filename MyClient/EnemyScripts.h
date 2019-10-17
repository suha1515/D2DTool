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
	void						CheckTiles();		//Ÿ��Ȯ��
	bool						CollideTiles();		//Ÿ�ϰ� �浹
protected:
	CTransform*			m_pTransform;		//Ʈ������
	CAnimator*			m_pAnimator;		//�ִϸ��̼�
	CBoxCollider*		m_pBoxCollider;

	ENEMY_TYPE			m_Type;

	float				m_fAcc;
	float				m_fVelocity;
	float				m_Hp;
	float				m_Atk;

	D3DXVECTOR3			m_PrePos;

	list <CGameObject*> m_NearTiles;		//Ÿ�� �浹�� ���Ѱ�.
	bool				m_IsActive;
public:
	void				SetOn(bool on);
	static CEnemyScripts* Create(CGameObject *pGameObject, ENEMY_TYPE type);
};

