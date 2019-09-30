#include "stdafx.h"
#include "GameObject.h"


//���� ������Ʈ
#include "TextureRenderer.h"
//Ʈ������ ������Ʈ
#include "Transform.h"

CGameObject::CGameObject(): m_bIsInit(false),m_pDeviceMgr(CDeviceMgr::GetInstance())
{
}


CGameObject::~CGameObject()
{
	//������Ʈ ����
	for (auto& i : m_Components)
		SafeDelete(i);
	m_Components.clear();

	//�ڽ� ������Ʈ ����� ��� ����
	for (auto& i : m_ChildrenObj)
		SafeDelete(i);
	m_ChildrenObj.clear();
}

int CGameObject::Update()
{
	if (m_bIsDead)
		return DEAD_OBJ;

	//������Ʈ ��� ����
	for (auto& i : m_Components)
		i->Action(this);
	

	//Ŭ���� �ڽ�����.
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

	//�̸�,�±�,���̾� ����
	SetObjectName(L"GameObject");
	SetObjectTag(L"none");
	SetObjectLayer(L"Layer0");

	//������Ʈ �ڽ� �÷�
	m_ColorBox = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_ParentObj = pParent;

	
	return S_OK;
}
// ��� ������Ʈ���� Initialize�� ȣ���ϰ��� ȣ��Ǵ� �Ǵٸ� �ʱ�ȭ �Լ�.
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
	//Ʈ������ ������Ʈ�� ������� ��ġ���� �����´�.
	if (GetComponent<CTransform>() != nullptr)
	{
		m_Pos = GetComponent<CTransform>()->GetPosition();
	}
	else
		m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�⺻ ������Ʈ �ڽ� ������.
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


