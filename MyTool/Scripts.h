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


	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize(CGameObject * pObject)=0;

	virtual void Action()=0;

};

//��ũ��Ʈ Ŭ������ ���ӿ�����Ʈ���� �߰��� �� �ִ� ������Ʈ�ν�
//�ش� ������Ʈ�� �ൿ�� ���� �� �� �ִ�.
//������Ʈ �Ŵ������� 

