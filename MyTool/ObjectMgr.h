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
	//������Ʈ ����. (���� ������Ʈ�� ������츸.)
	void Render();

public:
	//������Ʈ ���� ��ȯ.
	int GetObjectCount();

private:
	//vector<CGameObject*> m_Objects;

	map<int, vector<CGameObject*>>	m_Objects;
	list<CGameObject*>				m_RenderObjects[LAYER_END];
};

