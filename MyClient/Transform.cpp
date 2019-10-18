#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"

CTransform::CTransform()
{
}


CTransform::~CTransform()
{
}

void CTransform::Initialize(CGameObject *pObject)
{
	//행렬 초기화
	D3DXMatrixIdentity(&m_TransMat);
	D3DXMatrixIdentity(&m_RotMatX);
	D3DXMatrixIdentity(&m_RotMatY);
	D3DXMatrixIdentity(&m_RotMatZ);
	D3DXMatrixIdentity(&m_ScaleMat);
	D3DXMatrixIdentity(&m_WorldMat);
	D3DXMatrixIdentity(&m_ParentMat);

	m_GameObject = pObject;

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
}

void CTransform::Action()
{
	Update(m_GameObject);
}

void CTransform::Update(CGameObject *pObject)
{

		//초기 위치 계산
		Translate(m_Pos);
		Rotate(m_Rotaion);
		Scale(m_Scale);

		//월드 행렬 구성
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
	m_RotateMat = m_RotMatX*m_RotMatY*m_RotMatZ;
}

void CTransform::Scale(const D3DXVECTOR3 & vec)
{
	D3DXMatrixScaling(&m_ScaleMat, vec.x, vec.y, vec.z);
}

void CTransform::SetWorld()
{
	//부모 객체가 있으면
	if (m_GameObject->GetParentObject() != nullptr)
	{
		CGameObject*  m_ParentObj = m_GameObject->GetParentObject();
		m_ParentMat = m_ParentObj->GetComponent<CTransform>()->GetWorldMat();
	}
	else
		D3DXMatrixIdentity(&m_ParentMat);

	m_WorldMat = m_ScaleMat*m_RotateMat* m_TransMat*m_ParentMat;
}

 D3DXVECTOR3 & CTransform::GetLocalPosition()
{
	return m_Pos;
}

 D3DXVECTOR3* CTransform::GetWorldPos()
 {
	 if (m_GameObject->GetParentObject() != nullptr)
	 {
		 D3DXVECTOR3 pos = *m_GameObject->GetParentObject()->GetComponent<CTransform>()->GetWorldPos();
		 m_WorldPos = m_Pos + pos;
	 }
	 else
		 m_WorldPos = m_Pos;
	 return &m_WorldPos;
 }

const XMFLOAT3 & CTransform::GetRotation() const
{
	return m_Rotaion;
}

const D3DXVECTOR3 & CTransform::GetScale() const
{
	return m_Scale;
}

const D3DXMATRIX & CTransform::GetTransMat() const
{
	return m_TransMat;
}

const D3DXMATRIX & CTransform::GetRotateMat() const
{
	return m_RotateMat;
}

const D3DXMATRIX & CTransform::GetScaleMat() const
{
	return m_ScaleMat;
}

const D3DXMATRIX & CTransform::GetWorldMat() const
{
	return m_WorldMat;
}
