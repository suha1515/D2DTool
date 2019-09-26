#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_bIsInit(false), m_pVB(nullptr), m_pIB(nullptr), m_texInfo(nullptr)
{
}


CGameObject::~CGameObject()
{
	if (m_pVB != nullptr)
		m_pVB->Release();
	if(m_pIB != nullptr)
		m_pIB->Release();
}

int CGameObject::Update()
{
	return 0;
}

void CGameObject::LateUpdate()
{
}

void CGameObject::Render()
{
	if (m_texInfo != nullptr)
	{
		m_pDeviceMgr->GetDevice()->SetTransform(D3DTS_WORLD, &m_TransMat);

		m_pDeviceMgr->GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pDeviceMgr->GetDevice()->SetIndices(m_pIB);
		m_pDeviceMgr->GetDevice()->SetFVF(FVF_VERTEX);
		m_pDeviceMgr->GetDevice()->SetTexture(0, m_texInfo->pTexture);
		
		m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDeviceMgr->GetDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			6,
			0,
			2
		);
		m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
}

HRESULT CGameObject::Initialize()
{

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
	Translate(m_Pos);
}

void CGameObject::SetRotation(const XMFLOAT3 & rot)
{
	m_Rotaion = rot;
	Rotate(m_Rotaion);
}

void CGameObject::SetScaling(const D3DXVECTOR3 & size)
{
	m_Scale = size;
	Scale(m_Scale);
}

void CGameObject::Translate(const D3DXVECTOR3 & vec)
{
	D3DXMatrixTranslation(&m_TransMat, vec.x, vec.y, vec.z);
}

void CGameObject::Rotate(const XMFLOAT3 & rot)
{
	D3DXMatrixRotationX(&m_RotMat, D3DXToRadian(rot.x));
}

void CGameObject::Scale(const D3DXVECTOR3 & vec)
{
}


void CGameObject::SetVertex(const int& size, const XMFLOAT2* tex)
{
	if (m_pVB != nullptr)
		m_pVB->Release();
	if (m_pIB != nullptr)
		m_pIB->Release();

	m_Vertex[0] = Vertex(-size*0.5f, size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[0].x, tex[0].y);
	m_Vertex[1] = Vertex(-size*0.5f,-size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[1].x, tex[1].y);
	m_Vertex[2] = Vertex( size*0.5f, size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[2].x, tex[2].y);
	m_Vertex[3] = Vertex( size*0.5f,-size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[3].x, tex[3].y);

	m_pDeviceMgr->GetDevice()->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, FVF_VERTEX, D3DPOOL_MANAGED, &m_pVB, 0);
	Vertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	v[0] = m_Vertex[0];
	v[1] = m_Vertex[1];
	v[2] = m_Vertex[2];
	v[3] = m_Vertex[3];

	m_pVB->Unlock();

	m_pDeviceMgr->GetDevice()->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);

	WORD* i = 0;
	m_pIB->Lock(0, 0, (void**)&i, 0);
	i[0] = 0, i[1] = 2, i[2] = 1;
	i[3] = 1, i[4] = 2, i[5] = 3;


	m_pIB->Unlock();
}

void CGameObject::SetTexture(const CString & tileName)
{
	m_texInfo = m_pTextureMgr->GetTexInfo(L"TILE_MAP", tileName.operator LPCWSTR());
	NULL_CHECK_RETURN(m_texInfo);
}


const D3DXVECTOR3 & CGameObject::GetPosition() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_Pos;
}

const XMFLOAT2 & CGameObject::GetTexPos(const int & index)
{
	if (index > 4 || index < 0)
	{
		MessageBox(0, L"GetTexPos out of Range!", L"ERROR", ERROR);
		return XMFLOAT2(0.0f,0.0f);
	}
	// TODO: 여기에 반환 구문을 삽입합니다.
	return XMFLOAT2(m_Vertex[index]._u, m_Vertex[index]._v);
}

const CString & CGameObject::GetTexName()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	if (m_texInfo == nullptr)
	{
		MessageBox(0, L"GetTexName m_texInfo nullptr!", L"ERROR", ERROR);
		return nullptr;
	}
	return m_texInfo->textureName;
}
