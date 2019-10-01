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
	//오브젝트 렌더. (렌더 컴포넌트가 있을경우만.)
	void Render();

public:
	//오브젝트 갯수 반환.
	int GetObjectCount();

private:
	//vector<CGameObject*> m_Objects;

	map<int, vector<CGameObject*>>	m_Objects;
	list<CGameObject*>				m_RenderObjects[LAYER_END];
};

