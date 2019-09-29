#pragma once
//class CGameObject;
class CGameObject;
class CObjectMgr
{
	DECLARE_SINGLETON(CObjectMgr)
public:
	CObjectMgr();
	~CObjectMgr();

	//오브젝트 추가
public:
	void AddObject(CGameObject* object);
public:
	//오브젝트 업데이트
	void Update();
	void UpdateChildren(vector<CGameObject*>::iterator& object);

public:
	//오브젝트 갯수 반환.
	int GetRootObjectCount();

private:
	vector<CGameObject*> m_Objects;
};

