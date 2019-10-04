#pragma once
#include "Subject.h"
class CPlayerSubject :
	public CSubject
{
	DECLARE_SINGLETON(CPlayerSubject)
public:
	CPlayerSubject();
	virtual ~CPlayerSubject();
public:
	void*GetData(int iMessage);
public:
	void AddData(int iMessage, void* pData);		//관찰 대상을 추가하는 함수.

private:
	map<int, void*>m_mapData;
};

