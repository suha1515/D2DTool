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
