#pragma once
class CScripts;
class CGameObject;
class CScriptMgr
{
	DECLARE_SINGLETON(CScriptMgr)
public:
	CScriptMgr();
	~CScriptMgr();
public:
	HRESULT	 LoadScripts(const wstring& wstrFilePath,CGameObject* pObject);
public:

private:

};

