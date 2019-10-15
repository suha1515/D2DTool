#include "stdafx.h"
#include "Mouse.h"

#include "Camera.h"
#include"Transform.h"
#include "GameObject.h"

CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	
}

void CMouse::Initialize()
{
	m_pMouse = CObjectMgr::GetInstance()->AddCopy(L"CrossHair");
	NULL_CHECK_MSG_RETURN(m_pMouse, L"마우스가 널포인트입니다");
	m_pTransform = m_pMouse->GetComponent<CTransform>();

	GetCursorPos(&m_Point);
	ScreenToClient(g_hWnd, &m_Point);

	scaleFactor = CCameraMgr::GetInstance()->GetMainCamera()->GetScaleFactor();
	m_Point.x = m_Point.x - WINCX*0.5f;
	m_Point.y = -1 * ((m_Point.y) - WINCY*0.5f);

	m_Point.x -= CCameraMgr::GetInstance()->GetMainCamera()->GetViewMatrix()->_41;
	m_Point.y -= CCameraMgr::GetInstance()->GetMainCamera()->GetViewMatrix()->_42;
	
	m_Point.x /= scaleFactor.x;
	m_Point.y /= scaleFactor.y;
	m_pTransform->SetPosition(D3DXVECTOR3(m_Point.x, -m_Point.y,0.0f));

}

void CMouse::Update()
{
	GetCursorPos(&m_Point);
	ScreenToClient(g_hWnd, &m_Point);
	m_Point.x = m_Point.x - WINCX*0.5f;
	m_Point.y = -1 * ((m_Point.y) - WINCY*0.5f);

	m_Point.x -= CCameraMgr::GetInstance()->GetMainCamera()->GetViewMatrix()->_41;
	m_Point.y -= CCameraMgr::GetInstance()->GetMainCamera()->GetViewMatrix()->_42;

	m_Point.x /= scaleFactor.x;
	m_Point.y /= scaleFactor.y;

	D3DXVECTOR3& m_MousePos = m_pTransform->GetLocalPosition();
	m_MousePos.x = m_Point.x;
	m_MousePos.y = m_Point.y;
}

void CMouse::SetMouse(CGameObject * pMouse)
{
	m_pMouse = pMouse;
}

D3DXVECTOR3 CMouse::GetMousePos()
{
	return m_pTransform->GetLocalPosition();
}

