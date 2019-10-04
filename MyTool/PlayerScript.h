#pragma once
#include "Scripts.h"
class CPlayerScript :
	public CScripts
{
public:
	CPlayerScript();
	~CPlayerScript();

	// CScripts��(��) ���� ��ӵ�
	virtual void ScriptInit() override;
	virtual void ScriptLateInit() override;
	virtual void Update() override;
	virtual void Release() override;

	// CScripts��(��) ���� ��ӵ�
	virtual void Initialize(CGameObject * pObject) override;
	virtual void Action() override;
};

