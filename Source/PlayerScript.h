#pragma once
#include "Scripts.h"
class CPlayerScript :
	public CScripts
{
public:
	CPlayerScript();
	~CPlayerScript();
	// CScripts��(��) ���� ��ӵ�
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject=nullptr, XMFLOAT2* move=nullptr) override;
	virtual void OnInput() override;
	virtual void OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;
	
};

