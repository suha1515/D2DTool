#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"

#include "TextureRenderer.h"
#include "BoxCollider.h"
#include "Scripts.h"
#include "Transform.h"
IMPLEMENT_SINGLETON(CObjectMgr);
CObjectMgr::CObjectMgr()
{
	m_bIsDebug = false;
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

void CObjectMgr::Initialize()
{
	for (size_t i = 0; i < m_Objects.size(); ++i)
	{
		for (auto& j : m_Objects[i])
		{
			//렌더 컴포넌트가 nullptr이 아닐경우. 레이어에따라 렌더할 오브젝트를 넣는다.
			if (j->GetComponent<CTextureRenderer>() != nullptr)
				m_RenderObjects[j->GetObjectLayer()].push_back(j);
			//오브젝트가 스크립트 를가지고있으면 따로 스크립트 처리하기위해 컨테이너에 넣는다.
			if (j->GetScripts().size() > 0)
				m_SciptObject.push_back(j);
		}
	}
	//스크립트 객체 OnInit
	OnInit();
}

//모든 오브젝트 컴포넌트 업데이트
void CObjectMgr::Update()
{
	//트리 계층별 업데이트를한다.
	// 0 계층은 최상위 부모 객체들
	// 1,2,3.. 계층은 0계층 부모객체들의 자식객체들..
	for (size_t i = 0; i < m_Objects.size(); ++i)
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
				{
					//인스턴스 태그를 가지고 있을경우
					if ((*iter_begin)->GetObjectTag() == L"Instance")
					{
						CInstanceMgr::GetInstance()->AddObject((*iter_begin));
					}
					//아닐경우
					else
						m_RenderObjects[(*iter_begin)->GetObjectLayer()].push_back((*iter_begin));
				}
				//오브젝트가 스크립트 를가지고있으면 따로 스크립트 처리하기위해 컨테이너에 넣는다.
				if ((*iter_begin)->GetScripts().size() > 0)
					m_SciptObject.push_back((*iter_begin));

				//오브젝트가 콜라이더를 가지고있으면 콜라이더 계산을 위해 리스트에 넣는다.
				if ((*iter_begin)->GetComponent<CBoxCollider>() != nullptr)
					m_CollideObj.push_back((*iter_begin));

				
				iter_begin++;
			}
		}
	}

	//모든 컴포넌트 업데이트가 끝나면 스크립트 라이프 사이클을 진행한다.
	//위에서 너비조사를 진행했으므로 부모부터 스크립트 오브젝트를 실행할것이다.
	OnCollision();			//충돌검사
	OnUpdate();				//스크립트 업데이트
	OnDestroy();			//스크립트 제거

}

//레이어에 따라 렌더를 진행한다. 
void CObjectMgr::Render()
{
	//인스턴스 오브젝트는 가장 먼저그린다.
	CInstanceMgr::GetInstance()->InstanceRender();
	for (int i = 0; i < LAYER_END; ++i)
	{
		//Y축 소팅.
		m_RenderObjects[i].sort(CLess<CGameObject*,CTransform>());
		for (auto& object : m_RenderObjects[i])
		{
			//활성화 상태만 렌더함.
			if(object->GetComponent<CTextureRenderer>()->GetOn())
				object->Render();
		}
		m_RenderObjects[i].clear();
	}

	if (m_bIsDebug)
		DebugRender();

	m_SciptObject.clear();
	m_CollideObj.clear();
}

void CObjectMgr::DebugRender()
{
	for (auto&i : m_CollideObj)
	{
		i->DebugRender();
	}
		
}

int CObjectMgr::GetObjectCount()
{
	return m_Objects[0].size();
}

void CObjectMgr::OnInit()
{
	//모든 객체 스크립트 OnInit
		for (auto& i : m_SciptObject)
		{
				for (auto& j : i->GetScripts())
					j.second->OnInit();
		}
}

void CObjectMgr::OnCollision()
{
	list<CGameObject*>::iterator iter_begin = m_CollideObj.begin();

	for (; iter_begin != m_CollideObj.end(); iter_begin++)
	{
		CBoxCollider* pSource = (*iter_begin)->GetComponent<CBoxCollider>();
		if (pSource->GetOn())
		{
			for (list<CGameObject*>::iterator iter_begin2 = m_CollideObj.begin(); iter_begin2 != m_CollideObj.end(); iter_begin2++)
			{
				//비교대상이 자기 자신이라면 컨티뉴
				if (iter_begin2 == iter_begin)
					continue;
				CBoxCollider* pDest = (*iter_begin2)->GetComponent<CBoxCollider>();
				if (pDest->GetOn())
				{
					if (CCollisionMgr::GetInstance()->CheckAABB(pSource, pDest))
					{
						//스크립트의 OnCollision 실행 충돌대상으로 포인터를 전달한다.
						for (auto& i : (*iter_begin)->GetScripts())
							i.second->OnCollision((*iter_begin2));
					}
				}
			}
		}
		
	}
}

void CObjectMgr::OnUpdate()
{
	//모든 객체 스크립트 OnUpdate
		for (auto& i : m_SciptObject)
		{
				for (auto& j : i->GetScripts())
					j.second->OnUpdate();
		}
}
void CObjectMgr::OnDestroy()
{
	//모든 객체 스크립트 OnDestroy
		for (auto& i : m_SciptObject)
		{
				for (auto& j : i->GetScripts())
					j.second->OnDestroy();
		}
}

void CObjectMgr::SetDebug(bool on)
{
	m_bIsDebug = on;
}
