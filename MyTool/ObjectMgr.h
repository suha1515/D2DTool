#pragma once
//class CGameObject;
class CGameObject;
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)
public:
	CObjectMgr();
	~CObjectMgr();

	//������Ʈ �߰�
public:
	void AddObject(CGameObject* object);
public:
	//������Ʈ ������Ʈ
	void Update();
	void UpdateChildren(vector<CGameObject*>::iterator& object);

public:
	//������Ʈ ���� ��ȯ.
	int GetRootObjectCount();

private:
	vector<CGameObject*> m_Objects;
};

