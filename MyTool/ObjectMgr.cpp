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

//모든 오브젝트 업데이트
void CObjectMgr::Update()
{
	//트리 계층별 업데이트를한다.
	// 0 계층은 최상위 부모 객체들
	// 1,2,3.. 계층은 0계층 부모객체들의 자식객체들..
	for (int i = 0; i < m_Objects.size(); ++i)
	{
		vector<CGameObject*>::iterator iter_begin = m_Objects[i].begin();
		vector<CGameObject*>::iterator iter_end = m_Objects[i].end();
		for (;iter_begin!=iter_end;)
		{
			if ((*iter_begin)->Update() == DEAD_OBJ)
			{
				//부모 오브젝트가 있을 경우.
				//부모 자식벡터에서 자신을 제거한다.
				CGameObject* parentObj = (*iter_begin)->GetParentObject();
				if (nullptr != parentObj)
				{
					auto& vecChild = parentObj->GetChildernVector();
					auto iter_find = find(vecChild.begin(), vecChild.end(), (*iter_begin));
					if (vecChild.end() != iter_find)
						vecChild.erase(iter_find);
				}
				//자식 오브젝트가 있을경우. 다른 계층에 존재함으로 DEAD_OBJ 메시지만 전달한다.
				// 0계층의 부모가 1계층의 자식 들에게 DEAD_OBJ를 전달하면 1계층의 자식이 2계층의 자식에게 DEAD_OBJ를 전달한다.
				// 그리고 자식 오브젝트들이 부모 오브젝트를 잃도록 하여 부모벡터에서 제거하려는 시도를 막는다.
				auto& vecChild = (*iter_begin)->GetChildernVector();
				if (vecChild.size() > 0)
				{
					for (auto& i : vecChild)
					{
						i->SetObjectDestroy(true);
						i->SetParentObject(nullptr);
					}
				}

				//부모,자식 처리가 끝나면 해당 객체는 할당해제한다.
				SafeDelete((*iter_begin));
				iter_begin = m_Objects[i].erase(iter_begin);
				iter_end = m_Objects[i].end();
			}
			else
			{
				//렌더 컴포넌트가 nullptr이 아닐경우. 레이어에따라 렌더할 오브젝트를 넣는다.
				if ((*iter_begin)->GetComponent<CTextureRenderer>() != nullptr)
					m_RenderObjects[(*iter_begin)->GetObjectLayer()].push_back((*iter_begin));
				iter_begin++;
			}
		}
	}

}

//레이어에 따라 렌더를 진행한다. 
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
