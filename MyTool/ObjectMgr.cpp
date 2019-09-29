#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObjectMgr);
CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	for (auto &i : m_Objects)
		SafeDelete(i);
	m_Objects.clear();
}


void CObjectMgr::AddObject(CGameObject * object)
{
	m_Objects.push_back(object);
}

//모든 오브젝트 업데이트
void CObjectMgr::Update()
{
	vector<CGameObject*>::iterator iter_begin = m_Objects.begin();
	vector<CGameObject*>::iterator iter_end   = m_Objects.end();

	for (; iter_begin != iter_end;)
	{
		UpdateChildren(iter_begin);
	}
}
//자식 객체까지 업데이트하기위한 재귀 함수.
void CObjectMgr::UpdateChildren(vector<CGameObject*>::iterator& object)
{
	//업데이트후 오브젝트가 죽지않은 오브젝트라면
	if ((*object)->Update() != DEAD_OBJ)
	{
		//자식들도 업데이트한다.
		if ((*object)->GetChildernVector().size() > 0)
		{
			vector<CGameObject*>& m_Children = (*object)->GetChildernVector();
			vector<CGameObject*>::iterator iter_begin = m_Children.begin();
			vector<CGameObject*>::iterator iter_end = m_Children.end();
			for (; iter_begin != iter_end;)
			{
				UpdateChildren(iter_begin);
			}	
		}
		object++;
	}
	//죽었을 경우 삭제
	else
	{
		SafeDelete((*object));
		object = m_Objects.erase(object);
	}
	
}

int CObjectMgr::GetRootObjectCount()
{
	return m_Objects.size();
}
