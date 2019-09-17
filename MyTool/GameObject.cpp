#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
	: m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_bIsInit(false)
{
}


CGameObject::~CGameObject()
{
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
