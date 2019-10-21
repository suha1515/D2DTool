#include "stdafx.h"
#include "ObjectMgr.h"
#include "GameObject.h"

#include "TextureRenderer.h"
#include "BoxCollider.h"
#include "Scripts.h"
#include "Transform.h"
#include "Animator.h"
#include "Camera.h"
#include "Shader.h"

#include "Target.h"
#include "ScreenBuffer.h"

IMPLEMENT_SINGLETON(CObjectMgr);
CObjectMgr::CObjectMgr()
{
	m_bIsDebug = false;
	m_bIsStop = false;
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

	for (auto& i : m_Targets)
		i.second->Release();
	m_Targets.clear();

//	m_pScreenBuffer->Release();
}


void CObjectMgr::AddObject(CGameObject * object)
{
	if (object->GetObjectTag() == L"Player")
	{
		m_pPlayer = object;
	}
	if (object->GetObjectTag() == L"LightObject")
	{
		object->GetComponent<CTextureRenderer>()->SetPass(2);
	}
	//타일일경우 (인스턴싱객체) , 타일충돌체 인스턴싱은.. 안해도될듯.. 
	if (object->GetObjectTag() == L"Instance")
	{
	//	object->GetObjectTag() == L"Instance"
		CTransform* pTransform = object->GetComponent<CTransform>();
		wstring name = object->GetObjectName();
		CBoxCollider* pBoxCollider = object->GetComponent<CBoxCollider>();
		int index;
		if (pTransform != nullptr)
		{
			D3DXVECTOR3* pos = pTransform->GetWorldPos();
			int indexX = ((m_MapSizeX / 2) + pos->x)/16;
			int indexY = ((m_MapSizeY / 2) - pos->y)/16;

			m_index.push_back({ indexX,indexY });
			index = indexX + (indexY*m_MapSizeX);
			if (pBoxCollider != nullptr)
			{
				if (object->GetObjectLayer() == LAYER_GROUND)
					pBoxCollider->SetCollideColor(D3DCOLOR_XRGB(153, 217, 234));
				else if(object->GetObjectLayer()==LAYER_1)
					pBoxCollider->SetCollideColor( D3DCOLOR_XRGB(255, 255, 234));
				else if(object->GetObjectLayer() == LAYER_2)
					pBoxCollider->SetCollideColor(D3DCOLOR_XRGB(0, 128, 0));
				m_Tiles[index] = object;
			}	
		}
			m_RenderTiles.push_back(object);
	}
	//발판의 경우
	if (object->GetObjectTag() == L"step"|| object->GetObjectTag() == L"step_mid")
	{
		CBoxCollider* pBoxCollider = object->GetComponent<CBoxCollider>();
		if (pBoxCollider != nullptr)
		{
			if (object->GetObjectLayer() == LAYER_0)
				pBoxCollider->SetCollideColor(D3DCOLOR_XRGB(0, 0, 0));
			else if (object->GetObjectLayer() == LAYER_1)
				pBoxCollider->SetCollideColor(D3DCOLOR_XRGB(255, 0, 0));
			else if (object->GetObjectLayer() == LAYER_2)
				pBoxCollider->SetCollideColor(D3DCOLOR_XRGB(0, 0, 255));
			m_Stairs[object->GetObjectLayer()].push_back(object);
		}
	}
		m_Objects[object->GetLevel()].push_back(object);
}

void CObjectMgr::AddScriptObject(CScripts * script)
{
	m_Scripts.push_back(script);
}

void CObjectMgr::Initialize()
{
	/*CTarget* target = new CTarget;
	target->Initialize(WINCX, WINCY, D3DFMT_A8R8G8B8, D3DCOLOR_ARGB(255, 0, 0, 0));
	m_Targets.insert({ DIFFUSE,target });
	target = new CTarget;
	target->Initialize(WINCX, WINCY, D3DFMT_A8R8G8B8, D3DCOLOR_ARGB(255, 0, 0, 0));
	m_Targets.insert({ LIGHT_MAP,target });
	m_pScreenBuffer = new CScreenBuffer;
	m_pScreenBuffer->Initialize(WINCX, WINCY);

	m_pBlendShader = CShaderMgr::GetInstance()->GetEffect(L"BlendShader");*/
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
							if ((*iter_begin)->GetObjectLayer() == LAYER_GROUND)
							{
								CInstanceMgr::GetInstance()->AddObject((*iter_begin));
							}
							else if ((*iter_begin)->GetObjectTag() == L"LightObject")
								m_LightObject.push_back((*iter_begin));
							else
								m_RenderObjects[(*iter_begin)->GetObjectLayer()].push_back((*iter_begin));
						}
						//오브젝트가 스크립트 를가지고있으면 따로 스크립트 처리하기위해 컨테이너에 넣는다.
						if ((*iter_begin)->GetScripts().size() > 0)
						{

							m_SciptObject.push_back((*iter_begin));
						}

						//타일일경우 건너뛴다 (콜라이더를 넣지않음)
						if ((*iter_begin)->GetObjectTag() != L"Instance")
						{
							//오브젝트가 콜라이더를 가지고있으면 콜라이더 계산을 위해 리스트에 넣는다.
							if ((*iter_begin)->GetComponent<CBoxCollider>() != nullptr)
							{
								if ((*iter_begin)->GetComponent<CBoxCollider>()->GetOn())
									m_CollideObj.push_back((*iter_begin));
							}

						}
						else
						{
							if ((*iter_begin)->GetComponent<CBoxCollider>() != nullptr)
								m_CollideTile.push_back((*iter_begin));
							//if((*iter_begin)->GetObjectLayer()==LAYER_GROUND)

						}
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
	//렌더타겟 초기화.
	for (auto&i : m_Targets)
	{
		i.second->ClearColor();
	}
	//라이트 그리기.
	//m_Targets[LIGHT_MAP]->ChangeNewDevice(0);
	//for (auto&i : m_LightObject)
	//	i->Render();
	//m_Targets[LIGHT_MAP]->ChangeToPreDevice(0);

	//m_Targets[DIFFUSE]->ChangeNewDevice(0);
	//인스턴스 오브젝트는 가장 먼저그린다.(가장 밑바닥의 경우만가능할것같은데..
	CInstanceMgr::GetInstance()->InstanceRender();
	for (int i = 0; i < LAYER_END; ++i)
	{
		//Y축 소팅. 
		if (Layer(i) != LAYER_GROUND)//타일일경우 하지않는다. 
			m_RenderObjects[i].sort(CLess<CGameObject*, CTransform>());
		for (auto& object : m_RenderObjects[i])
		{
			D3DXVECTOR3 pos = *object->GetComponent<CTransform>()->GetWorldPos();
			CCamera* m_Cam = CCameraMgr::GetInstance()->GetMainCamera();
			if (m_Cam != nullptr)
			{
				float winX, winY;
				D3DXVECTOR3 scaleFactor = m_Cam->GetScaleFactor();
				if (m_Cam->GetLocalPosition().x>0.f)
					winX = (WINCX / scaleFactor.x) / 2.0f + m_Cam->GetLocalPosition().x;
				else
					winX = (WINCX / scaleFactor.x) / 2.0f - m_Cam->GetLocalPosition().x;

				if (m_Cam->GetLocalPosition().y>0.f)
					winY = (WINCX / scaleFactor.x) / 2.0f + m_Cam->GetLocalPosition().y;
				else
					winY = (WINCY / scaleFactor.y) / 2.0f - m_Cam->GetLocalPosition().y;
				if ((pos.x > (-1 * winX) && pos.x < winX) && (pos.y >(-1 * winY) && pos.y < winY))
				{
					//활성화 상태만 렌더함.
					if (object->GetComponent<CTextureRenderer>()->GetOn())
					{
						object->Render();
					}
				}
			}
		}
		m_RenderObjects[i].clear();
	}
	
	//일단 스크립트 오브젝트 렌더를 앞에다 두겠음
	for (auto&i : m_SciptObject)
	{
		for (auto&j : i->GetScripts())
			j.second->OnRender();
	}

	if (m_bIsDebug||m_bIsLayerDebug)
	{
		DebugRender();
	}
		
	m_SciptObject.clear();
	m_CollideObj.clear();
	m_CollideTile.clear();
	m_Barricade.clear();
	m_LightObject.clear();

	//m_Targets[DIFFUSE]->ChangeToPreDevice(0);

	//m_pBlendShader->GetEffect()->SetTexture("tex0", m_Targets[DIFFUSE]->GetTexture());
	//D3DXVECTOR3 color = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//m_pBlendShader->GetEffect()->SetFloatArray("color",color,3);
	////m_pBlendShader->GetEffect()->SetTexture("tex1", m_Targets[LIGHT_MAP]->GetTexture());
	//m_pBlendShader->GetEffect()->Begin(nullptr,0);
	//m_pBlendShader->GetEffect()->BeginPass(0);

	//m_pScreenBuffer->Render();
	//if (GetKeyState(VK_RETURN) < 0)
	//{
	//	D3DXSaveTextureToFile(L"../Texture/nORMAL.jpg", D3DXIFF_JPG, m_Targets[LIGHT_MAP]->GetTexture(), nullptr);
	//}
	//m_pBlendShader->GetEffect()->EndPass();
	//m_pBlendShader->GetEffect()->End();
}

void CObjectMgr::DebugRender()
{
	if (m_bIsDebug)
	{
		
		for (auto&i : m_CollideTile)
		{
			i->DebugRender(true,false);
		}
	}
	else if(m_bIsLayerDebug)
	{
		for (auto&i : m_CollideObj)
		{
				i->GetComponent<CBoxCollider>()->SetCollideColor(D3DCOLOR_XRGB(255, 0, 0));
				i->DebugRender(true, false);
		}
	}
	//for (auto&i : m_CollideObj)
	//{
	//	i->GetComponent<CBoxCollider>()->SetCollideColor(D3DCOLOR_XRGB(255, 0, 0));
	//	i->DebugRender(true, false);
	//}
	//
	//for (auto&i : m_RenderTiles)
	//{
	//	//i->DebugRender();
	//}
	
}

int CObjectMgr::GetObjectCount()
{
	return m_Objects[0].size();
}

vector<CGameObject*> CObjectMgr::FindObjectWithName(const wstring & name)
{
	vector<CGameObject*> temp;
	for (auto &i : m_Objects)
	{
		for (auto& j : i.second)
		{
			wstring objName =j->GetObjectName();
			if (objName == name)
				temp.push_back(j);
		}
	}
	return temp;
}

vector<CGameObject*> CObjectMgr::FindObjectWithTag(const wstring & tag)
{
	vector<CGameObject*> temp;
	for (auto &i : m_Objects)
	{
		for (auto& j : i.second)
		{
			wstring objTag = j->GetObjectTag();
			if (objTag == tag)
				temp.push_back(j);
		}
	}
	return temp;
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

void CObjectMgr::OnLateUpdate()
{
	//모든 객체 스크립트 OnUpdate
	for (auto& i : m_SciptObject)
	{
		for (auto& j : i->GetScripts())
			j.second->OnLateUpdate();
	}
}


void CObjectMgr::OnCollision()
{
	list<CGameObject*>::iterator iter_begin = m_CollideObj.begin();

	for (; iter_begin != m_CollideObj.end(); iter_begin++)
	{
		CBoxCollider* pSource = (*iter_begin)->GetComponent<CBoxCollider>();
		if (pSource->GetOn()&& !(*iter_begin)->GetScripts().empty())
		{
			for (list<CGameObject*>::iterator iter_begin2 = m_CollideObj.begin(); iter_begin2 != m_CollideObj.end(); iter_begin2++)
			{
				//비교대상이 자기 자신이라면 컨티뉴
				if (iter_begin2 == iter_begin)
					continue;
				CBoxCollider* pDest = (*iter_begin2)->GetComponent<CBoxCollider>();
				if (pDest->GetOn())
				{
					float fMoveX = 0.0f, fMoveY = 0.0f;
					XMFLOAT2 move;
					COLLIDE_TYPE colType = pDest->GetCollideType();
					if (colType == NORMAL)
					{
						if (CCollisionMgr::GetInstance()->CheckAABB(pSource, pDest))
						{
							//스크립트의 OnCollision 실행 충돌대상으로 포인터를 전달한다.
							for (auto& i : (*iter_begin)->GetScripts())
								i.second->OnCollision((*iter_begin2), &move);
						}
					}
					else
					{
						if (CCollisionMgr::GetInstance()->CheckLineBox(pSource, pDest))
						{
							//스크립트의 OnCollision 실행 충돌대상으로 포인터를 전달한다.
							for (auto& i : (*iter_begin)->GetScripts())
								i.second->OnCollision((*iter_begin2), &move);
						}
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
				{ 
					
					j.second->OnUpdate();
				}
				
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

void CObjectMgr::SetTileSize(int x, int y)
{
	m_MapSizeX = x;
	m_MapSizeY = y;
	m_Tiles.reserve(x*y);
	m_Tiles.assign(x*y,nullptr);
}

void CObjectMgr::SetDebug(bool on)
{
	m_bIsDebug = on;
}

void CObjectMgr::SetLayerDebug(bool on)
{
	m_bIsLayerDebug = on;
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
	m_Scripts.clear();
	m_LightObject.clear();
	m_Tiles.clear();
	m_RenderTiles.clear();
	m_CollideTile.clear();
	m_Barricade.clear();
}
void CObjectMgr::ClearCopy()
{
	m_CopyObjects.clear();
}

bool CObjectMgr::GetGameStop()
{
	return m_bIsStop;
}

void CObjectMgr::SetGameStop(bool stop)
{
	m_bIsStop = stop;
}


const list<CGameObject*>& CObjectMgr::GetBarricades()
{
	return m_Barricade;
}

const map<int, vector<CGameObject*>>& CObjectMgr::GetObjects()
{
	return m_Objects;
}

const vector<CGameObject*>& CObjectMgr::GetTiles()
{
	return m_Tiles;
}

const map<Layer, vector<CGameObject*>>& CObjectMgr::GetStairs()
{
	return m_Stairs;
}

HRESULT CObjectMgr::LoadObject(const wstring & filePath)
{
	HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	//string 함수를 이용해 파일이름과 확장자를 추출해야한다.
	wstring path = filePath;
	wstring name;
	size_t sep = path.find_last_of(L"\\/");								//파일경로에서 마지막 / or \\을찾는다

	if (sep != std::wstring::npos)										//찾는 문자열이 없을경우 npos를 반환하게된다.
		path = path.substr(sep + 1, path.size() - sep - 1);				//substr을 이용하여 해당 인덱스부터 끝까지 추출

	size_t dot = path.find_last_of(L".");								//추출된 내용에서 .까지의 인덱스를 찾는다.
	if (dot != std::wstring::npos)
	{
		name = path.substr(0, dot);										//처음부터 .까지의 문자를 추출하면 파일이름
	}
	else
		name = path;

	//불러오기 이므로 오브젝트 모두 없애기
	Clear();

	TCHAR	tmp[MAX_STR] = L"";
	OBJ_INFO		objInfo;						//오브젝트정보
	TRANSFORM_INFO  transInfo;						//트랜스폼정보
	TEXTURE_INFO	textureInfo;					//텍스처정보
	BOXCOL_INFO		boxcolInfo;						//콜라이더정보
	ANIM_INFO		animInfo;						//애니메이터정보
	SCRIPT_INFO		scriptInfo;						//스크립트정보.
	COMP_INFO compInfo;

	int sizeScript = 0;
	DWORD dwBytes = 0;
	while (true)
	{
		ReadFile(hFile, &compInfo, sizeof(COMP_INFO), &dwBytes, nullptr);
		if (0 == dwBytes)
			break;
		//오브젝트 생성
		CGameObject* pGameObject = new CGameObject;
		pGameObject->Initialize();
		//========================================오브젝트 정보 불러오기===========================================
		ReadFile(hFile, &objInfo, sizeof(OBJ_INFO), &dwBytes, nullptr);
		wstring strInfo;
		pGameObject->SetObjectName(objInfo._ObjectName);		//오브젝트 이름설정
		pGameObject->SetObjectTag(objInfo._ObjectTag);			//오브젝트 태그설정
		pGameObject->SetObjectLayer(objInfo._ObjectLayer);		//오브젝트 레이어설정

		if (lstrcmp(objInfo._ParentObject, L""))				//불러온 자료중 부모 오브젝트 이름이 있다면
		{
			CGameObject* pObject = CObjectMgr::GetInstance()->FindObjectWithName(objInfo._ParentObject).front(); //부모 오브젝트를 찾아서 넣는다. 0계층인 부모는 무조건 있으므로.. 순서만 바뀌지 않으면 여기서 문제가 없을것이다.
			pGameObject->SetParentObject(pObject);					//찾아서 해당 오브젝트의 부모 오브젝트로 등록한다
			pObject->GetChildernVector().push_back(pGameObject);	//더욱이 부모 벡터에는 자식을 넣는다.
			pGameObject->SetObjectLevel(pObject->GetLevel() + 1);	//부모의 계층보다 1높을것이다.
		}															//아니라면 그냥 넘어간다.
																	//==========================================================================================================	

																	//========================================트랜스폼 정보 불러오기===========================================
		if (compInfo._Transform == 1)
		{
			ReadFile(hFile, &transInfo, sizeof(TRANSFORM_INFO), &dwBytes, nullptr);
			CTransform*	pTransform = new CTransform;
			pTransform->Initialize(pGameObject);					//컴포넌트 주체 오브젝트 지정.
			pTransform->SetPosition(transInfo._ObjectPos);			//트랜스폼 위치 지정.
			pTransform->SetRotation(transInfo._ObjectRotation);		//트랜스폼 회전 지정.
			pTransform->SetScaling(transInfo._ObjectScale);			//트랜스폼 크기 지정.

			pGameObject->AddComponent(pTransform);					//오브젝트 컴포넌트 지정.
		}
		//==========================================================================================================	

		//========================================텍스쳐 정보 불러오기==============================================
		if (compInfo._Texture == 1)
		{
			ReadFile(hFile, &textureInfo, sizeof(TEXTURE_INFO), &dwBytes, nullptr);
			CTextureRenderer*	pTexture = new CTextureRenderer;
			pTexture->Initialize(pGameObject);						//텍스쳐 컴포넌트 주체 오브젝트 지정.
			pTexture->SetTexture(textureInfo._TextrueName);			//텍스쳐 이름 지정.

			XMFLOAT2 tex[4];										//텍스쳐 위치
			tex[0] = textureInfo._TexturPos[0];
			tex[1] = textureInfo._TexturPos[1];
			tex[2] = textureInfo._TexturPos[2];
			tex[3] = textureInfo._TexturPos[3];

			pTexture->SetVertex(textureInfo._TextureSize, tex);		//텍스처 크기와 위치를 지정한다.
			pGameObject->AddComponent(pTexture);					//오브젝트에 텍스쳐 컴포넌트 지정.
		}
		//==========================================================================================================	


		//========================================콜라이더 정보 불러오기==============================================
		if (compInfo._BoxCol == 1)
		{
			ReadFile(hFile, &boxcolInfo, sizeof(BOXCOL_INFO), &dwBytes, nullptr);
			CBoxCollider* pBoxCollider = new CBoxCollider;
			pBoxCollider->Initialize(pGameObject);					//박스 콜라이더 컴포넌트 오브젝트 지정.
			pBoxCollider->SetBoxSize(boxcolInfo._BoxWidth, boxcolInfo._BoxHeight);	//박스콜라이더 너비,높이지정.
			pBoxCollider->SetBoxOffset(boxcolInfo._BoxOffsetX, boxcolInfo._BoxOffsetY);	//박스콜라이더 오프셋지정.
			pBoxCollider->SetCollideType(boxcolInfo._colType);							//콜라이더 타입지정

			pGameObject->AddComponent(pBoxCollider);				//박스 콜라이더 지정.
		}
		//==========================================================================================================	

		//========================================애니메이터 정보 불러오기==============================================
		if (compInfo._Animator == 1)
		{
			ReadFile(hFile, &animInfo, sizeof(ANIM_INFO), &dwBytes, nullptr);
			CAnimator*	pAnimator = new CAnimator;
			pAnimator->Initialize(pGameObject);					//애니메이터 오브젝트 지정.
			pAnimator->LoadClips(animInfo._AnimationName);		//애니메이션 지정.

			pGameObject->AddComponent(pAnimator);
		}
		//==========================================================================================================	


		//========================================스크립트 정보 불러오기==============================================

		ReadFile(hFile, &sizeScript, sizeof(int), &dwBytes, nullptr);
		for (int i = 0; i < sizeScript; ++i)
		{
			ReadFile(hFile, &tmp, sizeof(tmp), &dwBytes, nullptr);
			CScriptMgr::GetInstance()->LoadScripts(tmp, pGameObject);
		}
		//==========================================================================================================	

		//완성된 오브젝트를 추가
		AddObject(pGameObject);
	}
	CloseHandle(hFile);

}
OBJ_COPY CObjectMgr::ReadCopyObject(HANDLE * hFile, DWORD * dwBytes)
{
	TCHAR	tmp[MAX_STR] = L"";
	OBJ_INFO		objInfo;						//오브젝트정보
	TRANSFORM_INFO  transInfo;						//트랜스폼정보
	TEXTURE_INFO	textureInfo;					//텍스처정보
	BOXCOL_INFO		boxcolInfo;						//콜라이더정보
	ANIM_INFO		animInfo;						//애니메이터정보
	SCRIPT_INFO		scriptInfo;						//스크립트정보.
	COMP_INFO compInfo;

	int sizeScript = 0;

	ReadFile(*hFile, &compInfo, sizeof(COMP_INFO), dwBytes, nullptr);

	OBJ_COPY copyObject;
	copyObject.compInfo = compInfo;
	//========================================오브젝트 정보 불러오기===========================================
	ReadFile(*hFile, &objInfo, sizeof(OBJ_INFO), dwBytes, nullptr);
	copyObject.objInfo = objInfo;
	wstring name = objInfo._ObjectName;

	//==========================================================================================================	

	//========================================트랜스폼 정보 불러오기===========================================
	if (compInfo._Transform == 1)
	{
		ReadFile(*hFile, &transInfo, sizeof(TRANSFORM_INFO), dwBytes, nullptr);
		copyObject.transformInfo = transInfo;
	}
	//==========================================================================================================	

	//========================================텍스쳐 정보 불러오기==============================================
	if (compInfo._Texture == 1)
	{
		ReadFile(*hFile, &textureInfo, sizeof(TEXTURE_INFO), dwBytes, nullptr);
		copyObject.textureInfo = textureInfo;
	}
	//==========================================================================================================	


	//========================================콜라이더 정보 불러오기==============================================
	if (compInfo._BoxCol == 1)
	{
		ReadFile(*hFile, &boxcolInfo, sizeof(BOXCOL_INFO), dwBytes, nullptr);
		copyObject.boxcolInfo = boxcolInfo;
	}
	//==========================================================================================================	

	//========================================애니메이터 정보 불러오기==============================================
	if (compInfo._Animator == 1)
	{
		ReadFile(*hFile, &animInfo, sizeof(ANIM_INFO), dwBytes, nullptr);
		copyObject.animInfo = animInfo;
	}
	//==========================================================================================================	


	//========================================스크립트 정보 불러오기==============================================

	ReadFile(*hFile, &sizeScript, sizeof(int), dwBytes, nullptr);
	for (int i = 0; i < sizeScript; ++i)
	{
		ScriptInfo info;
		ReadFile(hFile, &info, sizeof(ScriptInfo), dwBytes, nullptr);
		copyObject.scriptInfo.push_back(info);
	}
	//==========================================================================================================
	//자식이 있을경우
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

	//string 함수를 이용해 파일이름과 확장자를 추출해야한다.
	wstring path = filePath;
	wstring name;
	size_t sep = path.find_last_of(L"\\/");								//파일경로에서 마지막 / or \\을찾는다

	if (sep != std::wstring::npos)										//찾는 문자열이 없을경우 npos를 반환하게된다.
		path = path.substr(sep + 1, path.size() - sep - 1);				//substr을 이용하여 해당 인덱스부터 끝까지 추출

	size_t dot = path.find_last_of(L".");								//추출된 내용에서 .까지의 인덱스를 찾는다.
	if (dot != std::wstring::npos)
	{
		name = path.substr(0, dot);										//처음부터 .까지의 문자를 추출하면 파일이름
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
	//오브젝트 정보넣기
	if (name == L"")
		pGameObject->SetObjectName(copy->objInfo._ObjectName);
	else
		pGameObject->SetObjectName(name);		//새로운 이름을 넣어도된다.
	pGameObject->SetObjectLayer(copy->objInfo._ObjectLayer);
	pGameObject->SetObjectLevel(copy->objInfo._ObjectLevel);
	pGameObject->SetObjectTag(copy->objInfo._ObjectTag);

	if (parent != nullptr)
	{
		pGameObject->SetParentObject(parent);
		parent->GetChildernVector().push_back(pGameObject);
	}
	//Comp정보참조
	if (copy->compInfo._Transform == 1)
	{
		//부모의 기본위치는 따로 정해준다. 자식은 부모를 따라온다.
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
		pBoxCollider->SetCollideType(copy->boxcolInfo._colType);

		pGameObject->AddComponent(pBoxCollider);
	}
	if (copy->compInfo._Animator == 1)
	{
		CAnimator* pAnimator = new CAnimator;
		pAnimator->Initialize(pGameObject);
		pAnimator->LoadClips(copy->animInfo._AnimationName);

		pGameObject->AddComponent(pAnimator);
	}

	//스크립트
	if (copy->scriptInfo.size() > 0)
	{
		for (auto&i : copy->scriptInfo)
		{
			CScriptMgr::GetInstance()->LoadScripts(i._ScriptName, pGameObject);
		}
	}

	//자식이 있을경우
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
