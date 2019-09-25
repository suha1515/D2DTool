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
		m_pDeviceMgr->GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pDeviceMgr->GetDevice()->SetIndices(m_pIB);
		m_pDeviceMgr->GetDevice()->SetFVF(FVF_VERTEX);
		m_pDeviceMgr->GetDevice()->SetTexture(0, m_texInfo->pTexture);
		
		m_pDeviceMgr->GetDevice()->SetTransform(D3DTS_WORLD, &m_TransMat);

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
}

void CGameObject::SetVertex(const int& size, const XMFLOAT2* tex)
{
	m_Vertex[0] = Vertex(-size*0.5f, size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[0].x, tex[0].y);
	m_Vertex[0] = Vertex(-size*0.5f,-size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[1].x, tex[1].y);
	m_Vertex[0] = Vertex( size*0.5f, size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[2].x, tex[2].y);
	m_Vertex[0] = Vertex( size*0.5f,-size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[3].x, tex[3].y);

}

void CGameObject::SetTexture(const CString & tileName)
{
	m_texInfo = m_pTextureMgr->GetTexInfo(L"TILE_MAP", tileName.operator LPCWSTR());
	NULL_CHECK_RETURN(m_texInfo);
}

void CGameObject::Translate(const D3DXVECTOR3 & vec)
{
	D3DXMatrixTranslation(&m_TransMat, vec.x, vec.y, vec.z);
}

const D3DXVECTOR3 & CGameObject::GetPosition() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_Pos;
}
