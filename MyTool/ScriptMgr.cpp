#include "stdafx.h"
#include "ScriptMgr.h"

#include "Scripts.h"
#include "GameObject.h"

//��ũ��Ʈ ���
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
		MessageBox(0,L"������Ʈ�� nullptr �Դϴ�", L"��ũ��Ʈ ����", 0);
		return E_FAIL;
	}
	string scriptName;
	scriptName.assign(wstrScriptName.begin(), wstrScriptName.end());
	if (pObject->GetScript(scriptName) != nullptr)
	{
		wstring alert = pObject->GetObjectName() + L"�� ������Ʈ��"+ wstrScriptName+
			 L" ��ũ��Ʈ ������Ʈ�� �ֽ��ϴ�.";
		MessageBox(0, alert.c_str(), L"��ũ��Ʈ �ߺ�", 0);
		return E_FAIL;
	}
	//�÷��̾� ��ũ��Ʈ �ʱ�ȭ
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
