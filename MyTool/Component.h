#pragma once
class CGameObject;
class CComponent
{
public:
	explicit CComponent();
	virtual ~CComponent();
public:
	virtual	void		Initialize(CGameObject* pObject)=0;						//컴포넌트 초기화
			void		SetOn(bool on);						//컴포넌트 활성화
			bool		GetOn();							//컴포넌트 활성화 체크
	virtual void		Action() = 0;	//컴포넌트 작업 수행
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
