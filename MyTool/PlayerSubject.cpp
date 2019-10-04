#include "stdafx.h"
#include "PlayerSubject.h"


IMPLEMENT_SINGLETON(CPlayerSubject)
CPlayerSubject::CPlayerSubject()
{
}


CPlayerSubject::~CPlayerSubject()
{
	m_mapData.clear();
}

void * CPlayerSubject::GetData(int iMessage)
{
	return nullptr;
}

void CPlayerSubject::AddData(int iMessage, void * pData)
{
	NULL_CHECK(pData);

	auto iter_find = m_mapData.find(iMessage);

	if (m_mapData.end() != iter_find)
		return;

	m_mapData.insert({ iMessage,pData });
	CSubject::Notifiy(iMessage);
}
