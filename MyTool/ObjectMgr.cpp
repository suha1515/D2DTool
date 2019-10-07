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
			//���� ������Ʈ�� nullptr�� �ƴҰ��. ���̾���� ������ ������Ʈ�� �ִ´�.
			if (j->GetComponent<CTextureRenderer>() != nullptr)
				m_RenderObjects[j->GetObjectLayer()].push_back(j);
			//������Ʈ�� ��ũ��Ʈ �������������� ���� ��ũ��Ʈ ó���ϱ����� �����̳ʿ� �ִ´�.
			if (j->GetScripts().size() > 0)
				m_SciptObject.push_back(j);
		}
	}
	//��ũ��Ʈ ��ü OnInit
	OnInit();
}

//��� ������Ʈ ������Ʈ ������Ʈ
void CObjectMgr::Update()
{
	//Ʈ�� ������ ������Ʈ���Ѵ�.
	// 0 ������ �ֻ��� �θ� ��ü��
	// 1,2,3.. ������ 0���� �θ�ü���� �ڽİ�ü��..
	for (size_t i = 0; i < m_Objects.size(); ++i)
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
				{
					//�ν��Ͻ� �±׸� ������ �������
					if ((*iter_begin)->GetObjectTag() == L"Instance")
					{
						CInstanceMgr::GetInstance()->AddObject((*iter_begin));
					}
					//�ƴҰ��
					else
						m_RenderObjects[(*iter_begin)->GetObjectLayer()].push_back((*iter_begin));
				}
				//������Ʈ�� ��ũ��Ʈ �������������� ���� ��ũ��Ʈ ó���ϱ����� �����̳ʿ� �ִ´�.
				if ((*iter_begin)->GetScripts().size() > 0)
					m_SciptObject.push_back((*iter_begin));

				//������Ʈ�� �ݶ��̴��� ������������ �ݶ��̴� ����� ���� ����Ʈ�� �ִ´�.
				if ((*iter_begin)->GetComponent<CBoxCollider>() != nullptr)
					m_CollideObj.push_back((*iter_begin));

				
				iter_begin++;
			}
		}
	}

	//��� ������Ʈ ������Ʈ�� ������ ��ũ��Ʈ ������ ����Ŭ�� �����Ѵ�.
	//������ �ʺ����縦 ���������Ƿ� �θ���� ��ũ��Ʈ ������Ʈ�� �����Ұ��̴�.
	OnCollision();			//�浹�˻�
	OnUpdate();				//��ũ��Ʈ ������Ʈ
	OnDestroy();			//��ũ��Ʈ ����

}

//���̾ ���� ������ �����Ѵ�. 
void CObjectMgr::Render()
{
	//�ν��Ͻ� ������Ʈ�� ���� �����׸���.
	CInstanceMgr::GetInstance()->InstanceRender();
	for (int i = 0; i < LAYER_END; ++i)
	{
		//Y�� ����.
		m_RenderObjects[i].sort(CLess<CGameObject*,CTransform>());
		for (auto& object : m_RenderObjects[i])
		{
			//Ȱ��ȭ ���¸� ������.
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
	//��� ��ü ��ũ��Ʈ OnInit
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
				//�񱳴���� �ڱ� �ڽ��̶�� ��Ƽ��
				if (iter_begin2 == iter_begin)
					continue;
				CBoxCollider* pDest = (*iter_begin2)->GetComponent<CBoxCollider>();
				if (pDest->GetOn())
				{
					if (CCollisionMgr::GetInstance()->CheckAABB(pSource, pDest))
					{
						//��ũ��Ʈ�� OnCollision ���� �浹������� �����͸� �����Ѵ�.
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
	//��� ��ü ��ũ��Ʈ OnUpdate
		for (auto& i : m_SciptObject)
		{
				for (auto& j : i->GetScripts())
					j.second->OnUpdate();
		}
}
void CObjectMgr::OnDestroy()
{
	//��� ��ü ��ũ��Ʈ OnDestroy
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
