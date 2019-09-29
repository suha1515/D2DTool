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

	//오브젝트 이동,회전,크기 지정 함수
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotation(const XMFLOAT3& rot);
	void SetScaling(const D3DXVECTOR3& size);

	//오브젝트 이동,회전,크기 조절함수
	void Translate(const D3DXVECTOR3& vec);
	void Rotate(const XMFLOAT3& rot);
	void Scale(const D3DXVECTOR3& vec);
	void SetWorld();

	//오브젝트 태그,이름,레이어 지정
	void SetObjectName(const wstring& name);
	void SetObjectTag(const wstring& tag);
	void SetObjectLayer(const wstring& layer);

public:
	// 오브젝트 크기 그리기.
	void DrawBox();

public:
	//위치값 리턴하는 함수.
	const D3DXVECTOR3&  GetPosition()	 const;

	//월드행렬을 리턴하는 함수.
	const D3DXMATRIX&	GetWorldMat()	 const;

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
	//부모 오브젝트 반환 함수
	CGameObject*		GetParentObject();
	//자식 오브젝트 반환 함수
	CGameObject*		GetChildrenObject(int index);
	//자식 오브젝트 벡터 반환
	vector<CGameObject*>& GetChildernVector();

	
protected:
	// 디바이스 매니저
	CDeviceMgr*				m_pDeviceMgr;

	// 오브젝트 이름,태그,레이어
	wstring					m_ObjectName;
	wstring					m_ObjectTag;
	wstring					m_ObjectLayer;
	
	//오브젝트 크기,회전,이동,월드
	D3DXMATRIX				m_ScaleMat;
	D3DXMATRIX				m_RotMat;
	D3DXMATRIX				m_TransMat;

	D3DXMATRIX				m_WorldMat;

	//오브젝트 위치,회전,크기
	D3DXVECTOR3				m_Pos;
	XMFLOAT3				m_Rotaion;
	D3DXVECTOR3				m_Scale;

	//컴포넌트
	vector<CComponent*>		m_Components;


	bool					m_bIsInit;
	bool					m_bIsClicked;

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
