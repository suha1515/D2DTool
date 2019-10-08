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
	//������Ʈ �ʱ�ȭ
	void Initialize();
	//������Ʈ ������Ʈ
	void Update();
	//������Ʈ ����. (���� ������Ʈ�� ������츸.)
	void Render();

	//����� ���� (�ݶ��̴��� Ȯ�ε��..)
	void DebugRender();

public:
	//������Ʈ ���� ��ȯ.
	int GetObjectCount();
	//�̸����� ������Ʈ ã��
	CGameObject* FindObjectWithName(const wstring& name);
	//�±׷� ������Ʈ ã��
	CGameObject* FindObjectWithTag(const wstring& tag);
	
public:
	//������Ʈ ��ũ��Ʈ ������ ����Ŭ
	void OnInit();
	void OnCollision();
	void OnUpdate();
	void OnDestroy();



public:
	void SetDebug(bool on);
	void Clear();

public:
	const map<int, vector<CGameObject*>>& GetObjects();
public:
	HRESULT LoadObject(const wstring& filePath);


private:
	list<CGameObject*>				m_CollideObj;
	list<CGameObject*>				m_SciptObject;
	map<int, vector<CGameObject*>>	m_Objects;
	list<CGameObject*>				m_RenderObjects[LAYER_END];

	bool							m_bIsDebug;
};

