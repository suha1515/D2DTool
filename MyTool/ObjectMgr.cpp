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

//��� ������Ʈ ������Ʈ
void CObjectMgr::Update()
{
	vector<CGameObject*>::iterator iter_begin = m_Objects.begin();
	vector<CGameObject*>::iterator iter_end   = m_Objects.end();

	for (; iter_begin != iter_end;)
	{
		UpdateChildren(iter_begin);
	}
}
//�ڽ� ��ü���� ������Ʈ�ϱ����� ��� �Լ�.
void CObjectMgr::UpdateChildren(vector<CGameObject*>::iterator& object)
{
	//������Ʈ�� ������Ʈ�� �������� ������Ʈ���
	if ((*object)->Update() != DEAD_OBJ)
	{
		//�ڽĵ鵵 ������Ʈ�Ѵ�.
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
	//�׾��� ��� ����
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
