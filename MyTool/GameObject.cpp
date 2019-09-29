#include "stdafx.h"
#include "GameObject.h"


//렌더 컴포넌트
#include "TextureRenderer.h"

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
	//초기 위치 계산
	Translate(m_Pos);
	Rotate(m_Rotaion);
	Scale(m_Scale);

	//월드 행렬 구성
	SetWorld();

	//컴포넌트 기능 수행
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
	//행렬 초기화
	D3DXMatrixIdentity(&m_TransMat);
	D3DXMatrixIdentity(&m_RotMat);
	D3DXMatrixIdentity(&m_ScaleMat);
	D3DXMatrixIdentity(&m_WorldMat);

	//위치,크기,회전값 초기화
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotaion = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//초기 위치 계산
	Translate(m_Pos);
	Rotate(m_Rotaion);
	Scale(m_Scale);

	//월드 행렬 구성
	SetWorld();

	m_bIsClicked = false;

	//이름,태그,레이어 지정
	SetObjectName(L"GameObject");
	SetObjectTag(L"none");
	SetObjectLayer(L"Layer0");

	

	//임시 렌더 컴포넌트 넣기.
	CTextureRenderer* pRender = new CTextureRenderer;
	pRender->Initialize();
	m_Components.push_back(pRender);

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
	//기본 오브젝트 박스 렌더링.
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
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_ChildrenObj;
}


