#include "stdafx.h"
#include "Scripts.h"


CScripts::CScripts()
{
	m_ScriptName = "";
}


CScripts::~CScripts()
{
}

string CScripts::GetScriptName()
{
	return m_ScriptName;
}

void CScripts::SetGameObject(CGameObject * pGameObject)
{
	m_pGameObject = pGameObject;
}
