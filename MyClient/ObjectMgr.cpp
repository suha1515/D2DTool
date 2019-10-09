#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"

#include "TextureRenderer.h"
#include "BoxCollider.h"
#include "Scripts.h"
#include "Transform.h"
#include "Animator.h"
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
	//OnCollision();			//�浹�˻�
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
		if (Layer(i) != LAYER_0)//Ÿ���ϰ�� �����ʴ´�. 
			m_RenderObjects[i].sort(CLess<CGameObject*, CTransform>());
		
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

CGameObject * CObjectMgr::FindObjectWithName(const wstring & name)
{
	for (auto &i : m_Objects)
	{
		for (auto& j : i.second)
		{
			wstring objName =j->GetObjectName();
			if (objName == name)
				return j;
		}
	}
	return nullptr;
}

CGameObject * CObjectMgr::FindObjectWithTag(const wstring & tag)
{
	for (auto &i : m_Objects)
	{
		for (auto& j : i.second)
		{
			wstring objTag = j->GetObjectTag();
			if (objTag == tag)
				return j;
		}
	}
	return nullptr;
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
					float fMoveX = 0.0f, fMoveY = 0.0f;
					XMFLOAT2 move;
					if (CCollisionMgr::GetInstance()->CheckRect(pSource, pDest,&move.x,&move.y))
					{
						//��ũ��Ʈ�� OnCollision ���� �浹������� �����͸� �����Ѵ�.
						for (auto& i : (*iter_begin)->GetScripts())
							i.second->OnCollision((*iter_begin2),&move);
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

void CObjectMgr::Clear()
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
	for (auto& i : m_RenderObjects)
	{
		i.clear();
	}
	m_CollideObj.clear();
	m_SciptObject.clear();
}
void CObjectMgr::ClearCopy()
{
	m_CopyObjects.clear();
}


const map<int, vector<CGameObject*>>& CObjectMgr::GetObjects()
{
	return m_Objects;
}

HRESULT CObjectMgr::LoadObject(const wstring & filePath)
{
	HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	//string �Լ��� �̿��� �����̸��� Ȯ���ڸ� �����ؾ��Ѵ�.
	wstring path = filePath;
	wstring name;
	size_t sep = path.find_last_of(L"\\/");								//���ϰ�ο��� ������ / or \\��ã�´�

	if (sep != std::wstring::npos)										//ã�� ���ڿ��� ������� npos�� ��ȯ�ϰԵȴ�.
		path = path.substr(sep + 1, path.size() - sep - 1);				//substr�� �̿��Ͽ� �ش� �ε������� ������ ����

	size_t dot = path.find_last_of(L".");								//����� ���뿡�� .������ �ε����� ã�´�.
	if (dot != std::wstring::npos)
	{
		name = path.substr(0, dot);										//ó������ .������ ���ڸ� �����ϸ� �����̸�
	}
	else
		name = path;

	//�ҷ����� �̹Ƿ� ������Ʈ ��� ���ֱ�
	Clear();

	TCHAR	tmp[MAX_STR] = L"";
	OBJ_INFO		objInfo;						//������Ʈ����
	TRANSFORM_INFO  transInfo;						//Ʈ����������
	TEXTURE_INFO	textureInfo;					//�ؽ�ó����
	BOXCOL_INFO		boxcolInfo;						//�ݶ��̴�����
	ANIM_INFO		animInfo;						//�ִϸ���������
	SCRIPT_INFO		scriptInfo;						//��ũ��Ʈ����.
	COMP_INFO compInfo;

	int sizeScript = 0;
	DWORD dwBytes = 0;
	while (true)
	{
		ReadFile(hFile, &compInfo, sizeof(COMP_INFO), &dwBytes, nullptr);
		if (0 == dwBytes)
			break;
		//������Ʈ ����
		CGameObject* pGameObject = new CGameObject;
		pGameObject->Initialize();
		//========================================������Ʈ ���� �ҷ�����===========================================
		ReadFile(hFile, &objInfo, sizeof(OBJ_INFO), &dwBytes, nullptr);
		wstring strInfo;
		pGameObject->SetObjectName(objInfo._ObjectName);		//������Ʈ �̸�����
		pGameObject->SetObjectTag(objInfo._ObjectTag);			//������Ʈ �±׼���
		pGameObject->SetObjectLayer(objInfo._ObjectLayer);		//������Ʈ ���̾��

		if (lstrcmp(objInfo._ParentObject, L""))				//�ҷ��� �ڷ��� �θ� ������Ʈ �̸��� �ִٸ�
		{
			CGameObject* pObject = CObjectMgr::GetInstance()->FindObjectWithName(objInfo._ParentObject); //�θ� ������Ʈ�� ã�Ƽ� �ִ´�. 0������ �θ�� ������ �����Ƿ�.. ������ �ٲ��� ������ ���⼭ ������ �������̴�.
			pGameObject->SetParentObject(pObject);					//ã�Ƽ� �ش� ������Ʈ�� �θ� ������Ʈ�� ����Ѵ�
			pObject->GetChildernVector().push_back(pGameObject);	//������ �θ� ���Ϳ��� �ڽ��� �ִ´�.
			pGameObject->SetObjectLevel(pObject->GetLevel() + 1);		//�θ��� �������� 1�������̴�.
		}															//�ƴ϶�� �׳� �Ѿ��.
																	//==========================================================================================================	

																	//========================================Ʈ������ ���� �ҷ�����===========================================
		if (compInfo._Transform == 1)
		{
			ReadFile(hFile, &transInfo, sizeof(TRANSFORM_INFO), &dwBytes, nullptr);
			CTransform*	pTransform = new CTransform;
			pTransform->Initialize(pGameObject);					//������Ʈ ��ü ������Ʈ ����.
			pTransform->SetPosition(transInfo._ObjectPos);			//Ʈ������ ��ġ ����.
			pTransform->SetRotation(transInfo._ObjectRotation);		//Ʈ������ ȸ�� ����.
			pTransform->SetScaling(transInfo._ObjectScale);			//Ʈ������ ũ�� ����.

			pGameObject->AddComponent(pTransform);					//������Ʈ ������Ʈ ����.
		}
		//==========================================================================================================	

		//========================================�ؽ��� ���� �ҷ�����==============================================
		if (compInfo._Texture == 1)
		{
			ReadFile(hFile, &textureInfo, sizeof(TEXTURE_INFO), &dwBytes, nullptr);
			CTextureRenderer*	pTexture = new CTextureRenderer;
			pTexture->Initialize(pGameObject);						//�ؽ��� ������Ʈ ��ü ������Ʈ ����.
			pTexture->SetTexture(textureInfo._TextrueName);			//�ؽ��� �̸� ����.

			XMFLOAT2 tex[4];										//�ؽ��� ��ġ
			tex[0] = textureInfo._TexturPos[0];
			tex[1] = textureInfo._TexturPos[1];
			tex[2] = textureInfo._TexturPos[2];
			tex[3] = textureInfo._TexturPos[3];

			pTexture->SetVertex(textureInfo._TextureSize, tex);		//�ؽ�ó ũ��� ��ġ�� �����Ѵ�.
			pGameObject->AddComponent(pTexture);					//������Ʈ�� �ؽ��� ������Ʈ ����.
		}
		//==========================================================================================================	


		//========================================�ݶ��̴� ���� �ҷ�����==============================================
		if (compInfo._BoxCol == 1)
		{
			ReadFile(hFile, &boxcolInfo, sizeof(BOXCOL_INFO), &dwBytes, nullptr);
			CBoxCollider* pBoxCollider = new CBoxCollider;
			pBoxCollider->Initialize(pGameObject);					//�ڽ� �ݶ��̴� ������Ʈ ������Ʈ ����.
			pBoxCollider->SetBoxSize(boxcolInfo._BoxWidth, boxcolInfo._BoxHeight);	//�ڽ��ݶ��̴� �ʺ�,��������.
			pBoxCollider->SetBoxOffset(boxcolInfo._BoxOffsetX, boxcolInfo._BoxOffsetY);	//�ڽ��ݶ��̴� ����������.

			pGameObject->AddComponent(pBoxCollider);				//�ڽ� �ݶ��̴� ����.
		}
		//==========================================================================================================	

		//========================================�ִϸ����� ���� �ҷ�����==============================================
		if (compInfo._Animator == 1)
		{
			ReadFile(hFile, &animInfo, sizeof(ANIM_INFO), &dwBytes, nullptr);
			CAnimator*	pAnimator = new CAnimator;
			pAnimator->Initialize(pGameObject);					//�ִϸ����� ������Ʈ ����.
			pAnimator->LoadClips(animInfo._AnimationName);		//�ִϸ��̼� ����.

			pGameObject->AddComponent(pAnimator);
		}
		//==========================================================================================================	


		//========================================��ũ��Ʈ ���� �ҷ�����==============================================

		ReadFile(hFile, &sizeScript, sizeof(int), &dwBytes, nullptr);
		for (int i = 0; i < sizeScript; ++i)
		{
			ReadFile(hFile, &tmp, sizeof(tmp), &dwBytes, nullptr);
			CScriptMgr::GetInstance()->LoadScripts(tmp, pGameObject);
		}
		//==========================================================================================================	

		//�ϼ��� ������Ʈ�� �߰�
		AddObject(pGameObject);
	}
	CloseHandle(hFile);

}
OBJ_COPY CObjectMgr::ReadCopyObject(HANDLE * hFile, DWORD * dwBytes)
{
	TCHAR	tmp[MAX_STR] = L"";
	OBJ_INFO		objInfo;						//������Ʈ����
	TRANSFORM_INFO  transInfo;						//Ʈ����������
	TEXTURE_INFO	textureInfo;					//�ؽ�ó����
	BOXCOL_INFO		boxcolInfo;						//�ݶ��̴�����
	ANIM_INFO		animInfo;						//�ִϸ���������
	SCRIPT_INFO		scriptInfo;						//��ũ��Ʈ����.
	COMP_INFO compInfo;

	int sizeScript = 0;

	ReadFile(*hFile, &compInfo, sizeof(COMP_INFO), dwBytes, nullptr);

	OBJ_COPY copyObject;
	copyObject.compInfo = compInfo;
	//========================================������Ʈ ���� �ҷ�����===========================================
	ReadFile(*hFile, &objInfo, sizeof(OBJ_INFO), dwBytes, nullptr);
	copyObject.objInfo = objInfo;
	wstring name = objInfo._ObjectName;

	//==========================================================================================================	

	//========================================Ʈ������ ���� �ҷ�����===========================================
	if (compInfo._Transform == 1)
	{
		ReadFile(*hFile, &transInfo, sizeof(TRANSFORM_INFO), dwBytes, nullptr);
		copyObject.transformInfo = transInfo;
	}
	//==========================================================================================================	

	//========================================�ؽ��� ���� �ҷ�����==============================================
	if (compInfo._Texture == 1)
	{
		ReadFile(*hFile, &textureInfo, sizeof(TEXTURE_INFO), dwBytes, nullptr);
		copyObject.textureInfo = textureInfo;
	}
	//==========================================================================================================	


	//========================================�ݶ��̴� ���� �ҷ�����==============================================
	if (compInfo._BoxCol == 1)
	{
		ReadFile(*hFile, &boxcolInfo, sizeof(BOXCOL_INFO), dwBytes, nullptr);
		copyObject.boxcolInfo = boxcolInfo;
	}
	//==========================================================================================================	

	//========================================�ִϸ����� ���� �ҷ�����==============================================
	if (compInfo._Animator == 1)
	{
		ReadFile(*hFile, &animInfo, sizeof(ANIM_INFO), dwBytes, nullptr);
		copyObject.animInfo = animInfo;
	}
	//==========================================================================================================	


	//========================================��ũ��Ʈ ���� �ҷ�����==============================================

	ReadFile(*hFile, &sizeScript, sizeof(int), dwBytes, nullptr);
	for (int i = 0; i < sizeScript; ++i)
	{
		ScriptInfo info;
		ReadFile(hFile, &info, sizeof(ScriptInfo), dwBytes, nullptr);
		copyObject.scriptInfo.push_back(info);
	}
	//==========================================================================================================
	//�ڽ��� �������
	int childSize = 0;
	ReadFile(*hFile, &childSize, sizeof(int), dwBytes, nullptr);

	if (childSize > 0)
	{
		for (int i = 0; i < childSize; ++i)
		{
			OBJ_COPY child;
			child = ReadCopyObject(hFile, dwBytes);
			copyObject.childInfo.push_back(child);
		}
	}
	return copyObject;
}

HRESULT CObjectMgr::LoadCopyObjectFromFile(const wstring & filePath)
{
	HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	//string �Լ��� �̿��� �����̸��� Ȯ���ڸ� �����ؾ��Ѵ�.
	wstring path = filePath;
	wstring name;
	size_t sep = path.find_last_of(L"\\/");								//���ϰ�ο��� ������ / or \\��ã�´�

	if (sep != std::wstring::npos)										//ã�� ���ڿ��� ������� npos�� ��ȯ�ϰԵȴ�.
		path = path.substr(sep + 1, path.size() - sep - 1);				//substr�� �̿��Ͽ� �ش� �ε������� ������ ����

	size_t dot = path.find_last_of(L".");								//����� ���뿡�� .������ �ε����� ã�´�.
	if (dot != std::wstring::npos)
	{
		name = path.substr(0, dot);										//ó������ .������ ���ڸ� �����ϸ� �����̸�
	}
	else
		name = path;

	ClearCopy();

	int sizeScript = 0;
	DWORD dwBytes = 0;
	TCHAR copyName[MAX_STR];
	while (true)
	{
		OBJ_COPY copyObject;
		ReadFile(hFile, copyName, sizeof(copyName), &dwBytes, nullptr);
		copyObject = ReadCopyObject(&hFile, &dwBytes);
		if (dwBytes == 0)
			break;
		m_CopyObjects.insert({ copyName,copyObject });
	}
	CloseHandle(hFile);
	return S_OK;
}

CGameObject* CObjectMgr::MakeObjectFromCopy(const OBJ_COPY * copy, const wstring & name, CGameObject * parent)
{
	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	//������Ʈ �����ֱ�
	if (name == L"")
		pGameObject->SetObjectName(copy->objInfo._ObjectName);
	else
		pGameObject->SetObjectName(name);		//���ο� �̸��� �־�ȴ�.
	pGameObject->SetObjectLayer(copy->objInfo._ObjectLayer);
	pGameObject->SetObjectLevel(copy->objInfo._ObjectLevel);
	pGameObject->SetObjectTag(copy->objInfo._ObjectTag);

	if (parent != nullptr)
	{
		pGameObject->SetParentObject(parent);
		parent->GetChildernVector().push_back(pGameObject);
	}
	//Comp��������
	if (copy->compInfo._Transform == 1)
	{
		//�θ��� �⺻��ġ�� ���� �����ش�. �ڽ��� �θ� ����´�.
		CTransform* pTransform = new CTransform;
		pTransform->Initialize(pGameObject);
		pTransform->SetPosition(copy->transformInfo._ObjectPos);
		pTransform->SetRotation(copy->transformInfo._ObjectRotation);
		pTransform->SetScaling(copy->transformInfo._ObjectScale);
		pGameObject->AddComponent(pTransform);
	}
	if (copy->compInfo._Texture == 1)
	{
		CTextureRenderer* pTexture = new CTextureRenderer;
		pTexture->Initialize(pGameObject);
		pTexture->SetTexture(copy->textureInfo._TextrueName);
		pTexture->SetTexSize(copy->textureInfo._TextureSize);
		pTexture->SetTexPos(copy->textureInfo._TexturPos);

		pGameObject->AddComponent(pTexture);
	}
	if (copy->compInfo._BoxCol == 1)
	{
		CBoxCollider* pBoxCollider = new CBoxCollider;
		pBoxCollider->Initialize(pGameObject);
		pBoxCollider->SetBoxSize(copy->boxcolInfo._BoxWidth, copy->boxcolInfo._BoxHeight);
		pBoxCollider->SetBoxOffset(copy->boxcolInfo._BoxOffsetX, copy->boxcolInfo._BoxOffsetY);

		pGameObject->AddComponent(pBoxCollider);
	}
	if (copy->compInfo._Animator == 1)
	{
		CAnimator* pAnimator = new CAnimator;
		pAnimator->Initialize(pGameObject);
		pAnimator->LoadClips(copy->animInfo._AnimationName);

		pGameObject->AddComponent(pAnimator);
	}

	//��ũ��Ʈ
	if (copy->scriptInfo.size() > 0)
	{
		for (auto&i : copy->scriptInfo)
		{
			CScriptMgr::GetInstance()->LoadScripts(i._ScriptName, pGameObject);
		}
	}

	//�ڽ��� �������
	if (copy->childInfo.size() > 0)
	{
		for (auto&i : copy->childInfo)
			MakeObjectFromCopy(&i, i.objInfo._ObjectName, pGameObject);
	}

	AddObject(pGameObject);

	return pGameObject;
}

CGameObject * CObjectMgr::AddCopy(const wstring & name, const wstring & newName)
{
	auto iter_find = m_CopyObjects.find(name);
	if (m_CopyObjects.end() == iter_find)
		return nullptr;
	return MakeObjectFromCopy(&iter_find->second,newName,nullptr);
}
