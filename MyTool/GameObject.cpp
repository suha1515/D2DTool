#include "stdafx.h"
#include "GameObject.h"


//렌더 컴포넌트
#include "TextureRenderer.h"
//트랜스폼 컴포넌트
#include "Transform.h"

CGameObject::CGameObject(): m_bIsInit(false),m_pDeviceMgr(CDeviceMgr::GetInstance())
{
}


CGameObject::~CGameObject()
{
	//컴포넌트 제거
	for (auto& i : m_Components)
		SafeDelete(i);
	m_Components.clear();

	//자식 오브젝트 존재시 모두 제거
	for (auto& i : m_ChildrenObj)
		SafeDelete(i);
	m_ChildrenObj.clear();
}

int CGameObject::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;

	//컴포넌트 기능 수행
	for (auto& i : m_Components)
		i->Action(this);
	

	//클릭시 박스렌더.
	if(m_bIsClicked)
	 DrawBox();

	return NO_EVENT;
}

void CGameObject::LateUpdate()
{
}

HRESULT CGameObject::Initialize(CGameObject* pParent)
{
	m_bIsClicked = false;
	m_bIsDead = false;

	//이름,태그,레이어 지정
	SetObjectName(L"GameObject");
	SetObjectTag(L"none");
	SetObjectLayer(L"Layer0");

	//오브젝트 박스 컬러
	m_ColorBox = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_ParentObj = pParent;

	
	return S_OK;
}
// 모든 오브젝트들의 Initialize를 호출하고나서 호출되는 또다른 초기화 함수.
HRESULT CGameObject::LateInit()
{
	if (!m_bIsInit)
	{
		this->LateInit();
		m_bIsInit = true;
	}
	return S_OK;
}

void CGameObject::Release()
{
}
void CGameObject::SetObjectName(const wstring & name)
{
	m_ObjectName = name;
}

void CGameObject::SetObjectTag(const wstring & tag)
{
	m_ObjectTag = tag;
}

void CGameObject::SetObjectLayer(const wstring & layer)
{
	m_ObjectLayer = layer;
}

void CGameObject::SetObjectCliked(const bool & clicked,const D3DXCOLOR& color)
{
	m_bIsClicked = clicked;
	m_ColorBox = color;
}

void CGameObject::SetObjectDestroy(const bool & dead)
{
}

void CGameObject::DrawBox()
{
	D3DXVECTOR3 m_Pos;
	//트랜스폼 컴포넌트가 있을경우 위치값을 가져온다.
	if (GetComponent<CTransform>() != nullptr)
	{
		m_Pos = GetComponent<CTransform>()->GetPosition();
	}
	else
		m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//기본 오브젝트 박스 렌더링.
	D3DXMATRIX* mat = CCameraMgr::GetInstance()->GetViewProjMatrix();
	m_pDeviceMgr->GetLine()->SetWidth(3.f);
	m_pDeviceMgr->GetLine()->Begin();
	{
		D3DXVECTOR3 m_Line[2];
		m_Line[0] = { m_Pos.x - 8.0f,m_Pos.y + 8.0f,0.0f };
		m_Line[1] = { m_Pos.x + 8.0f,m_Pos.y + 8.0f,0.0f };
		m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, m_ColorBox);

		m_Line[0] = { m_Pos.x + 8.0f,m_Pos.y + 8.0f,0.0f };
		m_Line[1] = { m_Pos.x + 8.0f,m_Pos.y - 8.0f,0.0f };
		m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, m_ColorBox);

		m_Line[0] = { m_Pos.x + 8.0f,m_Pos.y - 8.0f,0.0f };
		m_Line[1] = { m_Pos.x - 8.0f,m_Pos.y - 8.0f,0.0f };
		m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, m_ColorBox);

		m_Line[0] = { m_Pos.x - 8.0f,m_Pos.y - 8.0f,0.0f };
		m_Line[1] = { m_Pos.x - 8.0f,m_Pos.y + 8.0f,0.0f };
		m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, m_ColorBox);
	}
	m_pDeviceMgr->GetLine()->End();
}

const wstring & CGameObject::GetObjectName() const
{
	return m_ObjectName;
}

const wstring & CGameObject::GetObjectTag() const
{
	return m_ObjectTag;
}

const wstring & CGameObject::GetObjectLayer() const
{
	return m_ObjectLayer;
}
void CGameObject::AddComponent(CComponent * component)
{
	m_Components.push_back(component);
}

void CGameObject::SetParentObject(CGameObject * parent)
{
	m_ParentObj = parent;
}

const CGameObject * CGameObject::GetParentObject()
{
	return m_ParentObj;
}

const CGameObject * CGameObject::GetChildrenObject(int index)
{
	return m_ChildrenObj[index];
}

vector<CGameObject*>& CGameObject::GetChildernVector()
{
	return m_ChildrenObj;
}


