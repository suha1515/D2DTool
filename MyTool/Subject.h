#pragma once
class CObserver;
class CSubject
{
public:
	CSubject();
	virtual ~CSubject();
public:
	enum DATA_TYPE{PLAYER_HP,PLAYER_MP,PLAYER_MATRIX};
public:
	void Subscribe(CObserver* pObserver);
	void UnSubscribe(CObserver* pObserver);
	void Notifiy(int iMessage);


protected:
	list<CObserver*>			m_ObserverLst;
};

