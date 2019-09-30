#pragma once
class CGameObject;
class CComponent
{
public:
	explicit CComponent();
	virtual ~CComponent();
public:
	virtual	void		Initialize()=0;
			void		SetOn(bool on);
	virtual void		Action(CGameObject* pObject) = 0;
public:
	template<typename T>
	T* GetComponent();
private:
	bool		m_ComponentOn;
};

template<typename T>
inline T * CComponent::GetComponent()
{
	T* pComponent = dynamic_cast<T*>(this);
	return pComponent;
}
