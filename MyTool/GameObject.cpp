#include "stdafx.h"
#include "GameObject.h"


//���� ������Ʈ
#include "TextureRenderer.h"

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
	//�ʱ� ��ġ ���
	Translate(m_Pos);
	Rotate(m_Rotaion);
	Scale(m_Scale);

	//���� ��� ����
	SetWorld();

	//������Ʈ ��� ����
	for (auto& i : m_Components)
		i->Action(this);

	DrawBox();

	return 0;
}

void CGameObject::LateUpdate()
{
}

HRESULT CGameObject::Initialize(CGameObject* pParent)
{
	//��� �ʱ�ȭ
	D3DXMatrixIdentity(&m_TransMat);
	D3DXMatrixIdentity(&m_RotMat);
	D3DXMatrixIdentity(&m_ScaleMat);
	D3DXMatrixIdentity(&m_WorldMat);

	//��ġ,ũ��,ȸ���� �ʱ�ȭ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotaion = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//�ʱ� ��ġ ���
	Translate(m_Pos);
	Rotate(m_Rotaion);
	Scale(m_Scale);

	//���� ��� ����
	SetWorld();

	m_bIsClicked = false;

	//�̸�,�±�,���̾� ����
	SetObjectName(L"GameObject");
	SetObjectTag(L"none");
	SetObjectLayer(L"Layer0");

	

	//�ӽ� ���� ������Ʈ �ֱ�.
	CTextureRenderer* pRender = new CTextureRenderer;
	pRender->Initialize();
	m_Components.push_back(pRender);

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

void CGameObject::SetPosition(const D3DXVECTOR3& pos)
{
	m_Pos = pos;
}

void CGameObject::SetRotation(const XMFLOAT3 & rot)
{
	m_Rotaion = rot;
}

void CGameObject::SetScaling(const D3DXVECTOR3 & size)
{
	m_Scale = size;
}

void CGameObject::Translate(const D3DXVECTOR3 & vec)
{
	D3DXMatrixTranslation(&m_TransMat, vec.x, vec.y, vec.z);
}

void CGameObject::Rotate(const XMFLOAT3 & rot)
{
	D3DXMatrixRotationX(&m_RotMat, D3DXToRadian(rot.x));
	D3DXMatrixRotationY(&m_RotMat, D3DXToRadian(rot.y));
	D3DXMatrixRotationZ(&m_RotMat, D3DXToRadian(rot.z));
}

void CGameObject::Scale(const D3DXVECTOR3 & vec)
{
	D3DXMatrixScaling(&m_ScaleMat, vec.x, vec.y, vec.z);
}

void CGameObject::SetWorld()
{
	m_WorldMat = m_ScaleMat* m_RotMat * m_TransMat;
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

void CGameObject::DrawBox()
{
	//�⺻ ������Ʈ �ڽ� ������.
	D3DXMATRIX* mat = CCameraMgr::GetInstance()->GetViewProjMatrix();
	m_pDeviceMgr->GetLine()->SetWidth(3.f);
	m_pDeviceMgr->GetLine()->Begin();
	{
		D3DXVECTOR3 m_Line[2];
		m_Line[0] = { m_Pos.x - 8.0f,m_Pos.y + 8.0f,0.0f };
		m_Line[1] = { m_Pos.x + 8.0f,m_Pos.y + 8.0f,0.0f };
		m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(255, 0, 0));

		m_Line[0] = { m_Pos.x + 8.0f,m_Pos.y + 8.0f,0.0f };
		m_Line[1] = { m_Pos.x + 8.0f,m_Pos.y - 8.0f,0.0f };
		m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(255, 0, 0));

		m_Line[0] = { m_Pos.x + 8.0f,m_Pos.y - 8.0f,0.0f };
		m_Line[1] = { m_Pos.x - 8.0f,m_Pos.y - 8.0f,0.0f };
		m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(255, 0, 0));

		m_Line[0] = { m_Pos.x - 8.0f,m_Pos.y - 8.0f,0.0f };
		m_Line[1] = { m_Pos.x - 8.0f,m_Pos.y + 8.0f,0.0f };
		m_pDeviceMgr->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(255, 0, 0));
	}
	m_pDeviceMgr->GetLine()->End();
}

const D3DXVECTOR3 & CGameObject::GetPosition() const
{
	return m_Pos;
}
const D3DXMATRIX & CGameObject::GetWorldMat() const
{
	return m_WorldMat;
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

CGameObject * CGameObject::GetParentObject()
{
	return m_ParentObj;
}

CGameObject * CGameObject::GetChildrenObject(int index)
{
	return m_ChildrenObj[index];
}

vector<CGameObject*>& CGameObject::GetChildernVector()
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_ChildrenObj;
}


