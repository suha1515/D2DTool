#pragma once
#include "Component.h"

class CScripts : public CComponent
{
public:
	explicit CScripts();
	virtual ~CScripts();
public:
	virtual void	ScriptInit() = 0;
	virtual void	ScriptLateInit() = 0;
	virtual void	Update() = 0;
	virtual void	Release() = 0;


	// CComponent을(를) 통해 상속됨
	virtual void Initialize(CGameObject * pObject)=0;

	virtual void Action()=0;

};

//스크립트 클래스는 게임오브젝트마다 추가할 수 있는 컴포넌트로써
//해당 오브젝트의 행동을 정의 할 수 있다.
//오브젝트 매니저에서 

