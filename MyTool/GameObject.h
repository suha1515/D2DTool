#pragma once
class CComponent;
class CGameObject
{
public:
	CGameObject();

public:
	virtual ~CGameObject();

public:
	virtual int Update();
	virtual void LateUpdate();

	//매개변수 있을시 부모 가된다.
	virtual HRESULT Initialize(CGameObject*	pParent=nullptr);
	virtual HRESULT LateInit();
	virtual void Release();

public:
	//오브젝트 태그,이름,레이어 지정
	void SetObjectName(const wstring& name);
	void SetObjectTag(const wstring& tag);
	void SetObjectLayer(const wstring& layer);

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
	const wstring&		GetObjectLayer() const;
	

	//컴포넌트 추가
public:
	void  AddComponent(CComponent* component);
	//컴포넌트 접근
public:
	template<typename T>
	T* GetComponent();

public:
	// *부모,자식 오브젝트에 대한 함수들*
	//부모 오브젝트 설정
	void SetParentObject(CGameObject* parent);

	//부모 오브젝트 반환 함수
	const CGameObject*		GetParentObject();
	//자식 오브젝트 반환 함수
	const CGameObject*		GetChildrenObject(int index);
	//자식 오브젝트 벡터 반환
	vector<CGameObject*>& GetChildernVector();

	
protected:
	// 디바이스 매니저
	CDeviceMgr*				m_pDeviceMgr;

	// 오브젝트 이름,태그,레이어
	wstring					m_ObjectName;
	wstring					m_ObjectTag;
	wstring					m_ObjectLayer;
	//오브젝트 박스 컬러.
	D3DXCOLOR				m_ColorBox;

	//컴포넌트
	vector<CComponent*>		m_Components;


	bool					m_bIsInit;
	bool					m_bIsClicked;
	bool					m_bIsDead;

private:
	CGameObject*			m_ParentObj;
	vector<CGameObject*>	m_ChildrenObj;
};

//컴포넌트 접근
template<typename T>
inline T * CGameObject::GetComponent()
{
	for (auto& i : m_Components)
	{
		if (typeid(*i).name() == typeid(T).name())
		{
			return dynamic_cast<T*>(i);
		}
			
	}
	return nullptr;
}
