#pragma once
class CComponent;
class CScripts;
class CGameObject
{
public:
	CGameObject();

public:
	virtual ~CGameObject();

public:
	//업데이트 ,렌더링, 늦은 업데이트
	virtual int Update();
	virtual void Render();
	virtual void DebugRender(bool collide, bool other);
	virtual void LateUpdate();

	//매개변수 있을시 부모 가된다.
	virtual HRESULT Initialize(CGameObject*	pParent=nullptr);
	virtual HRESULT LateInit();
	virtual void Release();

public:
	//오브젝트 태그,이름,레이어 지정
	void SetObjectName(const wstring& name);
	void SetObjectTag(const wstring& tag);
	void SetObjectLayer(const Layer& layer);

	//오브젝트 계층 설정 
	void SetObjectLevel(const int& level);

	//오브젝트 클릭 여부
	void SetObjectCliked(const bool & clicked, const D3DXCOLOR& color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	//오브젝트 삭제
	void SetObjectDestroy(const bool& dead);

public:
	// 오브젝트 크기 그리기.
	void DrawBox();
	//오브젝트 이름 반환 함수
	const wstring&		GetObjectName()  const;
	//오브젝트 태그 반환 함수
	const wstring&		GetObjectTag()   const;
	//오브젝트 레이어 반환함수
	const Layer&		GetObjectLayer() const;
	//오브젝트 계층 반환
	const int&		    GetObjectLevel() const;
	

	//컴포넌트 추가
public:
	void  AddComponent(CComponent* component);
	//컴포넌트 제거
public:
	HRESULT RemoveComponent(const string& strType);
	//컴포넌트 접근
public:
	template<typename T>
	T* GetComponent();

	//스크립트 추가
public:
	void AddScripts(CScripts* pScript);
	//스크립트 제거.
public:
	HRESULT RemoveScript(const string& strScript);
public:
	const map<string, CScripts*>&	GetScripts();
	//스크립트 접근
public:
	CScripts*	GetScript(const string& scriptName);

public:
	// *부모,자식 오브젝트에 대한 함수들*
	//부모 오브젝트 설정
	void SetParentObject(CGameObject* parent);

	//부모 오브젝트 반환 함수
	CGameObject*		GetParentObject();
	//자식 오브젝트 반환 함수
	CGameObject*		GetChildrenObject(int index);
	//자식 오브젝트 벡터 반환
	vector<CGameObject*>& GetChildernVector();
	//자신의 계층 반환
	int					GetLevel();
	
protected:
	// 디바이스 매니저
	CDeviceMgr*				m_pDeviceMgr;

	// 오브젝트 이름,태그,레이어
	wstring					m_ObjectName;
	wstring					m_ObjectTag;
	Layer					m_ObjectLayer;
	//오브젝트 박스 컬러.
	D3DXCOLOR				m_ColorBox;

	//컴포넌트
	map<string,CComponent*>m_Components;
	//스크립트
	map<string, CScripts*> m_Scripts;

	bool					m_bIsInit;
	bool					m_bIsClicked;
	bool					m_bIsDead;
	bool					m_bIsDebug;

private:
	//부모 객체, 자식 벡터, 자신이 속한 계층.
	CGameObject*			m_ParentObj;
	vector<CGameObject*>	m_ChildrenObj;
	int						m_Level;
};

//컴포넌트 접근
template<typename T>
inline T * CGameObject::GetComponent()
{
	auto&i = m_Components.find(typeid(T).name());
	if (m_Components.end() == i)
		return nullptr;

	return dynamic_cast<T*>(i->second);
}