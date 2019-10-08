#include "stdafx.h"
#include "ScriptMgr.h"

#include "Scripts.h"
#include "GameObject.h"

//스크립트 목록
#include "PlayerScript.h"

IMPLEMENT_SINGLETON(CScriptMgr)

CScriptMgr::CScriptMgr()
{
}


CScriptMgr::~CScriptMgr()
{
}

HRESULT CScriptMgr::LoadScripts(const wstring & wstrScriptName, CGameObject* pObject)
{
	if (pObject == nullptr)
	{
		MessageBox(0,L"오브젝트가 nullptr 입니다", L"스크립트 에러", 0);
		return E_FAIL;
	}
	string scriptName;
	scriptName.assign(wstrScriptName.begin(), wstrScriptName.end());
	if (pObject->GetScript(scriptName) != nullptr)
	{
		wstring alert = pObject->GetObjectName() + L"번 오브젝트는"+ wstrScriptName+
			 L" 스크립트 컴포넌트가 있습니다.";
		MessageBox(0, alert.c_str(), L"스크립트 중복", 0);
		return E_FAIL;
	}
	//플레이어 스크립트 초기화
		if (wstrScriptName == L"CPlayerScript")				
		{
			CScripts* pScript = new CPlayerScript;
			pObject->AddScripts(pScript);
			pScript->SetGameObject(pObject);
			return S_OK;
		}
		else
		{

		}

}
