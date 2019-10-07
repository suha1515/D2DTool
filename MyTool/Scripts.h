#pragma once
#include "Component.h"
class CGameObject;
class CScripts
{
public:
	explicit CScripts();
	virtual ~CScripts();
public:
	//������ ����Ŭ.
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

//��ũ��Ʈ Ŭ������ ���ӿ�����Ʈ���� �߰��� �� �ִ� ������Ʈ�ν�
//�ش� ������Ʈ�� �ൿ�� ���� �� �� �ִ�.
//������Ʈ �Ŵ������� 

