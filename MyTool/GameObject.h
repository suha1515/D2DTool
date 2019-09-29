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

	//�Ű����� ������ �θ� ���ȴ�.
	virtual HRESULT Initialize(CGameObject*	pParent=nullptr);
	virtual HRESULT LateInit();
	virtual void Release();

public:

	//������Ʈ �̵�,ȸ��,ũ�� ���� �Լ�
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotation(const XMFLOAT3& rot);
	void SetScaling(const D3DXVECTOR3& size);

	//������Ʈ �̵�,ȸ��,ũ�� �����Լ�
	void Translate(const D3DXVECTOR3& vec);
	void Rotate(const XMFLOAT3& rot);
	void Scale(const D3DXVECTOR3& vec);
	void SetWorld();

	//������Ʈ �±�,�̸�,���̾� ����
	void SetObjectName(const wstring& name);
	void SetObjectTag(const wstring& tag);
	void SetObjectLayer(const wstring& layer);

public:
	// ������Ʈ ũ�� �׸���.
	void DrawBox();

public:
	//��ġ�� �����ϴ� �Լ�.
	const D3DXVECTOR3&  GetPosition()	 const;

	//��������� �����ϴ� �Լ�.
	const D3DXMATRIX&	GetWorldMat()	 const;

	//������Ʈ �̸� ��ȯ �Լ�
	const wstring&		GetObjectName()  const;
	//������Ʈ �±� ��ȯ �Լ�
	const wstring&		GetObjectTag()   const;
	//������Ʈ ���̾� ��ȯ�Լ�
	const wstring&		GetObjectLayer() const;
	

	//������Ʈ �߰�
public:
	void  AddComponent(CComponent* component);
	//������Ʈ ����
public:
	template<typename T>
	T* GetComponent();

public:
	// *�θ�,�ڽ� ������Ʈ�� ���� �Լ���*
	//�θ� ������Ʈ ��ȯ �Լ�
	CGameObject*		GetParentObject();
	//�ڽ� ������Ʈ ��ȯ �Լ�
	CGameObject*		GetChildrenObject(int index);
	//�ڽ� ������Ʈ ���� ��ȯ
	vector<CGameObject*>& GetChildernVector();

	
protected:
	// ����̽� �Ŵ���
	CDeviceMgr*				m_pDeviceMgr;

	// ������Ʈ �̸�,�±�,���̾�
	wstring					m_ObjectName;
	wstring					m_ObjectTag;
	wstring					m_ObjectLayer;
	
	//������Ʈ ũ��,ȸ��,�̵�,����
	D3DXMATRIX				m_ScaleMat;
	D3DXMATRIX				m_RotMat;
	D3DXMATRIX				m_TransMat;

	D3DXMATRIX				m_WorldMat;

	//������Ʈ ��ġ,ȸ��,ũ��
	D3DXVECTOR3				m_Pos;
	XMFLOAT3				m_Rotaion;
	D3DXVECTOR3				m_Scale;

	//������Ʈ
	vector<CComponent*>		m_Components;


	bool					m_bIsInit;
	bool					m_bIsClicked;

private:
	CGameObject*			m_ParentObj;
	vector<CGameObject*>	m_ChildrenObj;
};

//������Ʈ ����
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
