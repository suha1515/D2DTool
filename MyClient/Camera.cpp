#include "stdafx.h"
#include "Camera.h"
#include "GameObject.h"

#include "Transform.h"

CCamera::CCamera()
{

}


CCamera::~CCamera()
{
}

void CCamera::Initialize(int width, int height, float angle, XMFLOAT3 scaleFactor)
{
	this->width = width;
	this->height = height;
	this->m_fAngle = angle;
	this->m_ScaleFactors = scaleFactor;

	m_CamPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMatrixOrthoLH(&m_OrthographicMat, width, height, 0.0f, 1.0f);
	D3DXMatrixIdentity(&m_IdentityMat);

	m_pDevice = CDeviceMgr::GetInstance();
	m_Following = nullptr;


}

void CCamera::Update()
{
	//m_CamPos.x = this->width / 2.0f, m_CamPos.y = this->height / 2.0f;
	if (m_Following != nullptr)
	{
		CTransform* pTransform = m_Following->GetComponent<CTransform>();
		NULL_CHECK_MSG_RETURN(pTransform, L"GameObject Transform is null");

		m_CamPos = pTransform->GetPosition();
	}
	m_ViewMat = D3DXMATRIX(
		m_ScaleFactors.x * cosf(m_fAngle), m_ScaleFactors.x * sinf(m_fAngle), 0, 0,
		-m_ScaleFactors.y * sinf(m_fAngle), m_ScaleFactors.y * cosf(m_fAngle), 0, 0,
		0, 0, m_ScaleFactors.z, 0,
		-m_CamPos.x*m_ScaleFactors.x*cosf(m_fAngle) + m_CamPos.y * m_ScaleFactors.y * sinf(m_fAngle), -m_CamPos.x * m_ScaleFactors.y * sinf(m_fAngle) - m_CamPos.y * m_ScaleFactors.y * cosf(m_fAngle), 0, 1);
}

void CCamera::Follow(CGameObject * object)
{
	m_Following = object;
}

void CCamera::UnFollow()
{
	m_Following = nullptr;
}

bool CCamera::IsFollowing() const
{
	return this->m_Following != nullptr;
}

void CCamera::SetTransform() const
{
	m_pDevice->GetDevice()->SetTransform(D3DTS_PROJECTION, &m_OrthographicMat);
	m_pDevice->GetDevice()->SetTransform(D3DTS_WORLD, &m_IdentityMat);
	m_pDevice->GetDevice()->SetTransform(D3DTS_VIEW, &m_ViewMat);
}

void CCamera::SetPosisiton(const D3DXVECTOR3 & pos)
{
	m_CamPos = pos;
}

void CCamera::SetScale(const float & _x, const float & _y)
{
	m_ScaleFactors.x = _x;
	m_ScaleFactors.y = _y;
}

void CCamera::SetScale(const XMFLOAT3 & _scale)
{
	m_ScaleFactors = _scale;
}

const D3DXVECTOR3 & CCamera::GetPosition() const
{
	return m_CamPos;
}

D3DXMATRIX * CCamera::GetViewMatrix()
{
	return &m_ViewMat;
}

D3DXMATRIX * CCamera::GetProjectMatrix()
{
	return &m_OrthographicMat;
}

D3DXMATRIX * CCamera::GetViewProjMatrix()
{
	m_ViewProjMat = m_ViewMat*m_OrthographicMat;
	return  &m_ViewProjMat;
}

void CCamera::Scaling(const float & _x, const float & _y)
{
	m_ScaleFactors.x += _x;
	m_ScaleFactors.y += _y;
}

void CCamera::MoveCamera(const D3DXVECTOR3 & pos)
{
	m_CamPos += pos;
}

void CCamera::MoveCamera(const float& _x, const float& _y)
{
	m_CamPos.x += _x;
	m_CamPos.y += _y;
}
