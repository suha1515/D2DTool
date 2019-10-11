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
	//이름으로 오브젝트 찾기
	CGameObject* FindObjectWithName(const wstring& name);
	//태그로 오브젝트 찾기
	CGameObject* FindObjectWithTag(const wstring& tag);
	
public:
	//오브젝트 스크립트 라이프 사이클
	void OnInit();
	void OnCollision();
	void OnUpdate();
	void OnDestroy();



public:
	void SetDebug(bool on);
	void Clear();
	void ClearCopy();

public:
	const map<int, vector<CGameObject*>>& GetObjects();

	const map<wstring, OBJ_COPY>&		  GetCopyObject();
public:
	HRESULT  LoadObject(const wstring& filePath);
	OBJ_COPY ReadCopy(HANDLE* hFile, DWORD* dwBytes);
public:
	HRESULT LoadCopyObject(const wstring& filePath);

public:
	CGameObject* MakeObjectFromCopy(const OBJ_COPY* copy, const wstring& name = L"", CGameObject* parent = nullptr);
public:
	CGameObject* AddCopy(const wstring& name, const wstring& newName = L"");


private:
	list<CGameObject*>				m_CollideObj;
	list<CGameObject*>				m_SciptObject;
	map<int, vector<CGameObject*>>	m_Objects;
	map<wstring, OBJ_COPY>		m_CopyObjects;
	list<CGameObject*>				m_RenderObjects[LAYER_END];

	bool							m_bIsDebug;
};

