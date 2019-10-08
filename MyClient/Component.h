#pragma once
class CGameObject;
class CComponent
{
public:
	explicit CComponent();
	virtual ~CComponent();
public:
	virtual	void		Initialize(CGameObject* pObject)=0;						//������Ʈ �ʱ�ȭ
			void		SetOn(bool on);						//������Ʈ Ȱ��ȭ
			bool		GetOn();							//������Ʈ Ȱ��ȭ üũ
	virtual void		Action() = 0;	//������Ʈ �۾� ����
public:
	template<typename T>
	T* GetComponent();
protected:
	bool				m_ComponentOn;
	CGameObject*		m_GameObject;
};

template<typename T>
inline T * CComponent::GetComponent()
{
	T* pComponent = dynamic_cast<T*>(this);
	return pComponent;
}
