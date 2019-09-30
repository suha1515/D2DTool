#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"

CTransform::CTransform()
{
}


CTransform::~CTransform()
{
}

void CTransform::Initialize()
{
	//��� �ʱ�ȭ
	D3DXMatrixIdentity(&m_TransMat);
	D3DXMatrixIdentity(&m_RotMatX);
	D3DXMatrixIdentity(&m_RotMatY);
	D3DXMatrixIdentity(&m_RotMatZ);
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
}

void CTransform::Action(CGameObject *pObject)
{
	Update(pObject);
}

void CTransform::Update(CGameObject *pObject)
{
	//�ʱ� ��ġ ���
	Translate(m_Pos);
	Rotate(m_Rotaion);
	Scale(m_Scale);

	//���� ��� ����
	SetWorld();

}

void CTransform::SetPosition(const D3DXVECTOR3 & pos)
{
	m_Pos = pos;
}

void CTransform::SetRotation(const XMFLOAT3 & rot)
{
	m_Rotaion = rot;
}

void CTransform::SetScaling(const D3DXVECTOR3 & size)
{
	m_Scale = size;
}

void CTransform::Translate(const D3DXVECTOR3 & vec)
{
	D3DXMatrixTranslation(&m_TransMat, vec.x, vec.y, vec.z);
}

void CTransform::Rotate(const XMFLOAT3 & rot)
{
	D3DXMatrixRotationX(&m_RotMatX, D3DXToRadian(rot.x));
	D3DXMatrixRotationY(&m_RotMatY, D3DXToRadian(rot.y));
	D3DXMatrixRotationZ(&m_RotMatZ, D3DXToRadian(rot.z));
}

void CTransform::Scale(const D3DXVECTOR3 & vec)
{
	D3DXMatrixScaling(&m_ScaleMat, vec.x, vec.y, vec.z);
}

void CTransform::SetWorld()
{
	m_WorldMat = m_ScaleMat*( m_RotMatX*m_RotMatY*m_RotMatZ)* m_TransMat;
}

const D3DXVECTOR3 & CTransform::GetPosition() const
{
	return m_Pos;
}

const XMFLOAT3 & CTransform::GetRotation() const
{
	return m_Rotaion;
}

const D3DXVECTOR3 & CTransform::GetScale() const
{
	return m_Scale;
}

const D3DXMATRIX & CTransform::GetWorldMat() const
{
	return m_WorldMat;
}
