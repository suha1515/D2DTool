#include "stdafx.h"
#include "BoxCollider.h"

#include "Transform.h"
#include "GameObject.h"

CBoxCollider::CBoxCollider()
{
}


CBoxCollider::~CBoxCollider()
{
}

void CBoxCollider::Initialize(CGameObject * pObject)
{
	m_GameObject = pObject;
	m_pTransform = pObject->GetComponent<CTransform>();
	wstring Alert = m_GameObject->GetObjectName() + L" 오브젝트는 트랜스폼 컴포넌트가 없습니다";
	NULL_CHECK_MSG_RETURN(m_pTransform, Alert.c_str());

	m_GameObject = pObject;
	m_ObjectPos = m_pTransform->GetLocalPosition();

	m_width = 16.0f, m_height = 16.0f;
	m_PosX = 0.0f, m_PosY = 0.0f;
}

void CBoxCollider::Action()
{
	SetBoxCollider();
}

void CBoxCollider::DrawCollide()
{
	if (m_CollideType == NORMAL)
		DrawBox();
	else
		DrawTriangle();
}

void CBoxCollider::DrawBox()
{
	D3DXMATRIX*	mat = CCameraMgr::GetInstance()->GetViewProjMatrix();
	
	
	CDeviceMgr::GetInstance()->GetLine()->SetWidth(2.f);
	CDeviceMgr::GetInstance()->GetLine()->Begin();
	{
		D3DXVECTOR3 m_Line[2];
		m_Line[0] = m_BoxDrawPos[0];
		m_Line[1] = m_BoxDrawPos[2];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[2];
		m_Line[1] = m_BoxDrawPos[3];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[3];
		m_Line[1] = m_BoxDrawPos[1];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[1];
		m_Line[1] = m_BoxDrawPos[0];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);
	}
	CDeviceMgr::GetInstance()->GetLine()->End();
}

void CBoxCollider::DrawTriangle()
{
	D3DXMATRIX*	mat = CCameraMgr::GetInstance()->GetViewProjMatrix();
	CDeviceMgr::GetInstance()->GetLine()->SetWidth(2.f);
	CDeviceMgr::GetInstance()->GetLine()->Begin();
	//콜라이더 타입이 좌측상단 삼각형일경우.
	{
		D3DXVECTOR3 m_Line[2];
	switch(m_CollideType)
	{
	case LEFT_TOP:
		m_Line[0] = m_BoxDrawPos[0];
		m_Line[1] = m_BoxDrawPos[2];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[2];
		m_Line[1] = m_BoxDrawPos[1];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[1];
		m_Line[1] = m_BoxDrawPos[0];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);
		break;
	case LEFT_BOTTOM:
		m_Line[0] = m_BoxDrawPos[0];
		m_Line[1] = m_BoxDrawPos[1];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[1];
		m_Line[1] = m_BoxDrawPos[3];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[3];
		m_Line[1] = m_BoxDrawPos[0];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);
		break;
	case RIGHT_TOP:
		m_Line[0] = m_BoxDrawPos[0];
		m_Line[1] = m_BoxDrawPos[2];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[2];
		m_Line[1] = m_BoxDrawPos[3];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[3];
		m_Line[1] = m_BoxDrawPos[0];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);
		break;
	case RIGHT_BOTTOM:
		m_Line[0] = m_BoxDrawPos[2];
		m_Line[1] = m_BoxDrawPos[3];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[3];
		m_Line[1] = m_BoxDrawPos[1];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);

		m_Line[0] = m_BoxDrawPos[1];
		m_Line[1] = m_BoxDrawPos[2];
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, m_BoxColor);
		break;
	}	
	}
	CDeviceMgr::GetInstance()->GetLine()->End();
}

void CBoxCollider::SetBoxSize(float width, float height)
{
	m_width = width;
	m_height = height;
}

void CBoxCollider::SetBoxOffset(float x, float y)
{
	m_PosX = x;
	m_PosY = y;
}

void CBoxCollider::SetBoxCollider()
{
	wstring Alert = m_GameObject->GetObjectName() + L" 오브젝트는 트랜스폼 컴포넌트가 없습니다";
	NULL_CHECK_MSG_RETURN(m_pTransform, Alert.c_str());

	m_ObjectPos = m_pTransform->GetLocalPosition();
	if (m_GameObject->GetParentObject() != nullptr)
	{
		D3DXVECTOR3 parentPos = *m_GameObject->GetParentObject()->GetComponent<CTransform>()->GetWorldPos();
		m_ObjectPos += parentPos;
	}

	m_BoxPos[0] = D3DXVECTOR3(-1*((m_width*0.5f)) + m_PosX+ m_ObjectPos.x,(m_height*0.5f) + m_PosY + m_ObjectPos.y, 0.0f);
	m_BoxPos[1] = D3DXVECTOR3(-1*((m_width*0.5f)) + m_PosX + m_ObjectPos.x,-1*((m_height*0.5f)) + m_PosY + m_ObjectPos.y, 0.0f);
	m_BoxPos[2] = D3DXVECTOR3((m_width*0.5f) + m_PosX + m_ObjectPos.x,(m_height*0.5f) + m_PosY + m_ObjectPos.y, 0.0f);
	m_BoxPos[3] = D3DXVECTOR3((m_width*0.5f) + m_PosX + m_ObjectPos.x,-1*((m_height*0.5f)) + m_PosY + m_ObjectPos.y, 0.0f);

	m_BoxDrawPos[0] = D3DXVECTOR3(-1 * ((m_width*0.5f)) + m_PosX , (m_height*0.5f) + m_PosY , 0.0f);
	m_BoxDrawPos[1] = D3DXVECTOR3(-1 * ((m_width*0.5f)) + m_PosX, -1 * ((m_height*0.5f)) + m_PosY , 0.0f);
	m_BoxDrawPos[2] = D3DXVECTOR3((m_width*0.5f) + m_PosX  , (m_height*0.5f) + m_PosY, 0.0f);
	m_BoxDrawPos[3] = D3DXVECTOR3((m_width*0.5f) + m_PosX  , -1 * ((m_height*0.5f)) + m_PosY , 0.0f);

	D3DXMATRIX	worldMat = m_GameObject->GetComponent<CTransform>()->GetWorldMat();
	D3DXVECTOR4 pos;
	D3DXVec3Transform(&pos,&m_BoxDrawPos[0], &worldMat);
	m_BoxDrawPos[0] = D3DXVECTOR3(pos.x, pos.y, pos.z);
	D3DXVec3Transform(&pos, &m_BoxDrawPos[1], &worldMat);
	m_BoxDrawPos[1] = D3DXVECTOR3(pos.x, pos.y, pos.z);
	D3DXVec3Transform(&pos, &m_BoxDrawPos[2], &worldMat);
	m_BoxDrawPos[2] = D3DXVECTOR3(pos.x, pos.y, pos.z);
	D3DXVec3Transform(&pos, &m_BoxDrawPos[3], &worldMat);
	m_BoxDrawPos[3] = D3DXVECTOR3(pos.x, pos.y, pos.z);

	
	m_BoxPosition = D3DXVECTOR3(m_ObjectPos.x + m_PosX, m_ObjectPos.y + m_PosY, 0.0f);

}
void CBoxCollider::SetCollideType(COLLIDE_TYPE type)
{
	if (type != NORMAL || type != LEFT_TOP || type != LEFT_BOTTOM || type != RIGHT_TOP || type != RIGHT_BOTTOM)
		m_CollideType = NORMAL;

	m_CollideType = type;


}
void CBoxCollider::SetCollideColor(D3DCOLOR color)
{
	m_BoxColor = color;
}
const D3DXVECTOR3 * CBoxCollider::GetBox()
{
	return m_BoxPos;
}

const D3DXVECTOR3* CBoxCollider::GetBoxPosition()
{
	return &m_BoxPosition;
}

const float CBoxCollider::GetBoxWidth()
{
	return m_width;
}

const float CBoxCollider::GetBoxHeight()
{
	return m_height;
}

const float CBoxCollider::GetBoxOffsetX()
{
	return m_PosX;
}

const float CBoxCollider::GetBoxoffsetY()
{
	return m_PosY;
}
COLLIDE_TYPE CBoxCollider::GetCollideType()
{
	return m_CollideType;
}
