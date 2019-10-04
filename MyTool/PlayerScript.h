#pragma once
#include "Scripts.h"
class CPlayerScript :
	public CScripts
{
public:
	CPlayerScript();
	~CPlayerScript();

	// CScripts을(를) 통해 상속됨
	virtual void ScriptInit() override;
	virtual void ScriptLateInit() override;
	virtual void Update() override;
	virtual void Release() override;

	// CScripts을(를) 통해 상속됨
	virtual void Initialize(CGameObject * pObject) override;
	virtual void Action() override;
};

