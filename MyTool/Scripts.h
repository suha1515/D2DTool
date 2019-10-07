#pragma once
#include "Component.h"
class CGameObject;
class CScripts
{
public:
	explicit CScripts();
	virtual ~CScripts();
public:
	//라이프 사이클.
	virtual void OnInit()=0;
	virtual void OnEnable()=0;
	virtual void OnCollision(CGameObject* pGameObject=nullptr) = 0;
	virtual void OnInput() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnLateUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDisable() = 0;
	virtual void OnDestroy() = 0;
public:
	string GetScriptName();
	void	SetGameObject(CGameObject* pGameObject);

protected:
		string				m_ScriptName;
		CGameObject*		m_pGameObject;
};

//스크립트 클래스는 게임오브젝트마다 추가할 수 있는 컴포넌트로써
//해당 오브젝트의 행동을 정의 할 수 있다.
//오브젝트 매니저에서 

