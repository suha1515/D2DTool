#pragma once
class CComponent;
class CGameObject
{
public:
	CGameObject();

public:
	virtual ~CGameObject();

public:
	//������Ʈ ,������, ���� ������Ʈ
	virtual int Update();
	virtual void Render();
	virtual void LateUpdate();

	//�Ű����� ������ �θ� ���ȴ�.
	virtual HRESULT Initialize(CGameObject*	pParent=nullptr);
	virtual HRESULT LateInit();
	virtual void Release();

public:
	//������Ʈ �±�,�̸�,���̾� ����
	void SetObjectName(const wstring& name);
	void SetObjectTag(const wstring& tag);
	void SetObjectLayer(const Layer& layer);

	//������Ʈ ���� ���� 
	void SetObjectLevel(const int& level);

	//������Ʈ Ŭ�� ����
	void SetObjectCliked(const bool & clicked, const D3DXCOLOR& color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	//������Ʈ ����
	void SetObjectDestroy(const bool& dead);

public:
	// ������Ʈ ũ�� �׸���.
	void DrawBox();
	//������Ʈ �̸� ��ȯ �Լ�
	const wstring&		GetObjectName()  const;
	//������Ʈ �±� ��ȯ �Լ�
	const wstring&		GetObjectTag()   const;
	//������Ʈ ���̾� ��ȯ�Լ�
	const Layer&		GetObjectLayer() const;
	

	//������Ʈ �߰�
public:
	void  AddComponent(CComponent* component);
	//������Ʈ ����
public:
	HRESULT RemoveComponent(const string& strType);
	//������Ʈ ����
public:
	template<typename T>
	T* GetComponent();

public:
	// *�θ�,�ڽ� ������Ʈ�� ���� �Լ���*
	//�θ� ������Ʈ ����
	void SetParentObject(CGameObject* parent);

	//�θ� ������Ʈ ��ȯ �Լ�
	CGameObject*		GetParentObject();
	//�ڽ� ������Ʈ ��ȯ �Լ�
	CGameObject*		GetChildrenObject(int index);
	//�ڽ� ������Ʈ ���� ��ȯ
	vector<CGameObject*>& GetChildernVector();
	//�ڽ��� ���� ��ȯ
	int					GetLevel();

	
protected:
	// ����̽� �Ŵ���
	CDeviceMgr*				m_pDeviceMgr;

	// ������Ʈ �̸�,�±�,���̾�
	wstring					m_ObjectName;
	wstring					m_ObjectTag;
	Layer					m_ObjectLayer;
	//������Ʈ �ڽ� �÷�.
	D3DXCOLOR				m_ColorBox;

	//������Ʈ
	map<string,CComponent*>m_Components;


	bool					m_bIsInit;
	bool					m_bIsClicked;
	bool					m_bIsDead;

private:
	//�θ� ��ü, �ڽ� ����, �ڽ��� ���� ����.
	CGameObject*			m_ParentObj;
	vector<CGameObject*>	m_ChildrenObj;
	int						m_Level;
};

//������Ʈ ����
template<typename T>
inline T * CGameObject::GetComponent()
{
	auto&i = m_Components.find(typeid(T).name());
	if (m_Components.end() == i)
		return nullptr;

	return dynamic_cast<T*>(i->second);
	/*for (auto& i : m_Components)
	{
		if (typeid(*i).name() == typeid(T).name())
		{
			return dynamic_cast<T*>(i);
		}
			
	}
	return nullptr;*/
}
