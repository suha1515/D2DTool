#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"

#include "TextureRenderer.h"

IMPLEMENT_SINGLETON(CObjectMgr);
CObjectMgr::CObjectMgr()
{
}


CObjectMgr::~CObjectMgr()
{
	for (auto &i : m_Objects)
	{
		for (auto& j : i.second)
		{
			SafeDelete(j);
		}
		i.second.clear();
	}
	m_Objects.clear();
}


void CObjectMgr::AddObject(CGameObject * object)
{
	m_Objects[object->GetLevel()].push_back(object);
}

//��� ������Ʈ ������Ʈ
void CObjectMgr::Update()
{
	//Ʈ�� ������ ������Ʈ���Ѵ�.
	// 0 ������ �ֻ��� �θ� ��ü��
	// 1,2,3.. ������ 0���� �θ�ü���� �ڽİ�ü��..
	for (int i = 0; i < m_Objects.size(); ++i)
	{
		vector<CGameObject*>::iterator iter_begin = m_Objects[i].begin();
		vector<CGameObject*>::iterator iter_end = m_Objects[i].end();
		for (;iter_begin!=iter_end;)
		{
			if ((*iter_begin)->Update() == DEAD_OBJ)
			{
				//�θ� ������Ʈ�� ���� ���.
				//�θ� �ڽĺ��Ϳ��� �ڽ��� �����Ѵ�.
				CGameObject* parentObj = (*iter_begin)->GetParentObject();
				if (nullptr != parentObj)
				{
					auto& vecChild = parentObj->GetChildernVector();
					auto iter_find = find(vecChild.begin(), vecChild.end(), (*iter_begin));
					if (vecChild.end() != iter_find)
						vecChild.erase(iter_find);
				}
				//�ڽ� ������Ʈ�� �������. �ٸ� ������ ���������� DEAD_OBJ �޽����� �����Ѵ�.
				// 0������ �θ� 1������ �ڽ� �鿡�� DEAD_OBJ�� �����ϸ� 1������ �ڽ��� 2������ �ڽĿ��� DEAD_OBJ�� �����Ѵ�.
				// �׸��� �ڽ� ������Ʈ���� �θ� ������Ʈ�� �ҵ��� �Ͽ� �θ��Ϳ��� �����Ϸ��� �õ��� ���´�.
				auto& vecChild = (*iter_begin)->GetChildernVector();
				if (vecChild.size() > 0)
				{
					for (auto& i : vecChild)
					{
						i->SetObjectDestroy(true);
						i->SetParentObject(nullptr);
					}
				}

				//�θ�,�ڽ� ó���� ������ �ش� ��ü�� �Ҵ������Ѵ�.
				SafeDelete((*iter_begin));
				iter_begin = m_Objects[i].erase(iter_begin);
				iter_end = m_Objects[i].end();
			}
			else
			{
				//���� ������Ʈ�� nullptr�� �ƴҰ��. ���̾���� ������ ������Ʈ�� �ִ´�.
				if ((*iter_begin)->GetComponent<CTextureRenderer>() != nullptr)
					m_RenderObjects[(*iter_begin)->GetObjectLayer()].push_back((*iter_begin));
				iter_begin++;
			}
		}
	}

}

//���̾ ���� ������ �����Ѵ�. 
void CObjectMgr::Render()
{
	for (int i = 0; i < LAYER_END; ++i)
	{
		for (auto& object : m_RenderObjects[i])
		{
			object->Render();
		}
		m_RenderObjects[i].clear();
	}
}

int CObjectMgr::GetObjectCount()
{
	return m_Objects[0].size();
}
