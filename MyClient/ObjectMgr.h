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

	//��ũ��Ʈ ������Ʈ �߰�
	void AddScriptObject(CScripts* script);
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

	void SetTileSize(int x, int y);

public:
	void SetDebug(bool on);
	void SetLayerDebug(bool on);
	void Clear();
	void ClearCopy();

public:
	const map<int, vector<CGameObject*>>& GetObjects();			//������Ʈ�� ����

	const map<wstring, OBJ_COPY>&		  GetCopyObject();		//ī�Ǹ� ����

	const vector<CGameObject*>&			  GetTiles();			//Ÿ�ϵ� ����

	const map<Layer, vector<CGameObject*>>& GetStairs();			//��ܵ� ����
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
	list<CGameObject*>				m_CollideObj;	//�浹ü��
	list<CGameObject*>				m_SciptObject;	//��ũ��Ʈ ������Ʈ�� 
	list<CScripts*>					m_Scripts;		//��ũ��Ʈ��
	map<int, vector<CGameObject*>>	m_Objects;		//��������Ʈ��
	map<wstring, OBJ_COPY>			m_CopyObjects;	//���纻
	list<CGameObject*>				m_RenderObjects[LAYER_END];		//�����ҳ༮��

	vector<CGameObject*>			m_Tiles;		//Ÿ�ϵ�
	vector<CGameObject*>			m_RenderTiles;	//������ Ÿ�ϵ�
	vector<CGameObject*>			m_CollideTile;  //�浹 Ÿ�ϵ�

	map<Layer,vector<CGameObject*>>	m_Stairs;		//��ܵ�;
	vector<pair<int, int>>			m_index;		//�ε���Ȯ�ο�;


	bool							m_bIsDebug;
	bool							m_bIsLayerDebug;
public:
	int								m_MapSizeX;
	int								m_MapSizeY;
};

