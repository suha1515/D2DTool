#pragma once
#include "EnemyScripts.h"
class CTurretScript :
	public CEnemyScripts
{
	enum STATE { IDLE, AIM,SHOOT,CHARGE,CHARGE_SHOOT };
	enum DIR16 { RIGHT, RIGHT_22, RIGHT_45, RIGHT_67, UP, LEFT_112, LEFT_135,LEFT_157,LEFT, LEFT_202, LEFT_225,LEFT_247,DOWN, RIGHT_292,RIGHT_315,RIGHT_337};
public:
	CTurretScript();
	~CTurretScript();

	// CEnemyScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr) override;
	virtual void OnInput() override;
	virtual int OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;
	virtual void DirState() override;
	virtual void AnimState() override;
	virtual void Move() override;
	virtual void GetHit(D3DXVECTOR3 dirVec, float power, float dmg) override;

private:
	void				GetDirPlayer();
	void				TrackPlayer();
private:
	D3DXVECTOR3			m_DirVec;			//방향벡터

	DIR16					m_CurDir;
	DIR16					m_PreDir;

	STATE				m_CurState;
	STATE				m_PreState;
private:
	D3DXVECTOR3*		m_Pos;
	CGameObject*		m_pPlayer;
	float				m_fAngle;

};

