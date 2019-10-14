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

	//스크립트 오브젝트 추가
	void AddScriptObject(CScripts* script);
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

	void SetTileSize(int x, int y);

public:
	void SetDebug(bool on);
	void SetLayerDebug(bool on);
	void Clear();
	void ClearCopy();

public:
	const map<int, vector<CGameObject*>>& GetObjects();			//오브젝트맵 접근

	const map<wstring, OBJ_COPY>&		  GetCopyObject();		//카피맵 접근

	const vector<CGameObject*>&			  GetTiles();			//타일들 접근

	const map<Layer, vector<CGameObject*>>& GetStairs();			//계단들 접근
public:
	HRESULT  LoadObject(const wstring& filePath);
	OBJ_COPY ReadCopyObject(HANDLE* hFile, DWORD* dwBytes);
public:
	HRESULT LoadCopyObjectFromFile(const wstring& filePath);

public:
	CGameObject* MakeObjectFromCopy(const OBJ_COPY* copy, const wstring& name = L"", CGameObject* parent = nullptr);

public:
	CGameObject* AddCopy(const wstring& name, const wstring& newName=L"");


private:
	list<CGameObject*>				m_CollideObj;	//충돌체들
	list<CGameObject*>				m_SciptObject;	//스크립트 오브젝트들 
	list<CScripts*>					m_Scripts;		//스크립트들
	map<int, vector<CGameObject*>>	m_Objects;		//모든오브젝트들
	map<wstring, OBJ_COPY>			m_CopyObjects;	//복사본
	list<CGameObject*>				m_RenderObjects[LAYER_END];		//렌더할녀석들

	vector<CGameObject*>			m_Tiles;		//타일들
	vector<CGameObject*>			m_RenderTiles;	//렌더할 타일들
	vector<CGameObject*>			m_CollideTile;  //충돌 타일들

	map<Layer,vector<CGameObject*>>	m_Stairs;		//계단들;
	vector<pair<int, int>>			m_index;		//인덱스확인용;


	bool							m_bIsDebug;
	bool							m_bIsLayerDebug;
public:
	int								m_MapSizeX;
	int								m_MapSizeY;
};

