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
	//오브젝트 초기화
	void Initialize();
	//오브젝트 업데이트
	void Update();
	//오브젝트 렌더. (렌더 컴포넌트가 있을경우만.)
	void Render();

	//디버그 렌더 (콜라이더나 확인등등..)
	void DebugRender();

public:
	//오브젝트 갯수 반환.
	int GetObjectCount();
	
public:
	//오브젝트 스크립트 라이프 사이클
	void OnInit();
	void OnCollision();
	void OnUpdate();
	void OnDestroy();

public:
	void SetDebug(bool on);


private:
	list<CGameObject*>				m_CollideObj;
	list<CGameObject*>				m_SciptObject;
	map<int, vector<CGameObject*>>	m_Objects;
	list<CGameObject*>				m_RenderObjects[LAYER_END];

	bool							m_bIsDebug;
};

