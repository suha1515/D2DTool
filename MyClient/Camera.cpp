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

void CCamera::Initialize(int width, int height, float angle, D3DXVECTOR3 scaleFactor)
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
	m_ViewMat = D3DXMATRIX(
		m_ScaleFactors.x * cosf(m_fAngle), m_ScaleFactors.x * sinf(m_fAngle), 0, 0,
		-m_ScaleFactors.y * sinf(m_fAngle), m_ScaleFactors.y * cosf(m_fAngle), 0, 0,
		0, 0, m_ScaleFactors.z, 0,
		-m_CamPos.x*m_ScaleFactors.x*cosf(m_fAngle) + m_CamPos.y * m_ScaleFactors.y * sinf(m_fAngle), -m_CamPos.x * m_ScaleFactors.y * sinf(m_fAngle) - m_CamPos.y * m_ScaleFactors.y * cosf(m_fAngle), 0, 1);
	
	//m_CamPos.x = this->width / 2.0f, m_CamPos.y = this->height / 2.0f;
	if (m_Following != nullptr)
	{
		float time = CTimeMgr::GetInstance()->GetDeltaTime();
		m_CamPos = Lerp(m_CamPos, *m_FollowPos, CTimeMgr::GetInstance()->GetDeltaTime()*2.f);
	}
	
}

void CCamera::LateUpdate()
{
	m_PreCamPos = *m_FollowPos;
	if (m_bIsCloseUp)
	{
		if (m_fRemainTime > 0.0f)
		{
			if (m_fCloseTime<m_fCloseUpTime)
			{
				m_ScaleFactors = Lerp(m_InitialScale, m_CloseUpScale, m_fCloseTime / m_fCloseUpTime);
				m_fCloseTime += CTimeMgr::GetInstance()->GetDeltaTime();
			}
			m_fRemainTime -= CTimeMgr::GetInstance()->GetDeltaTime();
		}
		else
		{
			if (CameraZoomOut())
			{
				m_fRemainTime = 0.0f;
				m_fCloseTime -= m_fCloseTime;
				m_bIsCloseUp = false;
			}
		}
	}

	if (m_bIsShaking)
	{
		if (m_fShakeTime > 0)
		{
			random_device	rn;
			mt19937_64 rnd(rn());
			uniform_real_distribution<float> nDir(-1.0f, 1.0f);

			D3DXVECTOR3 dir(nDir(rnd), nDir(rnd), 0.f);

			D3DXVec3Normalize(&dir, &dir);
			D3DXVECTOR3 pos;
			if (m_Following != nullptr)
				pos = dir*m_fAmountShake + m_PreCamPos;
			else
				pos = dir*m_fAmountShake + m_InitialPos;

			m_CamPos = pos;
			m_fShakeTime -= CTimeMgr::GetInstance()->GetDeltaTime();
		}
		else
		{
			m_fShakeTime = 0.0f;
			m_bIsShaking = false;
		}
	}
	
}

void CCamera::Follow(CGameObject * object)
{
	m_Following = object;
	m_FollowTransform = object->GetComponent<CTransform>();
	m_FollowPos = m_FollowTransform->GetWorldPos();
}

void CCamera::UnFollow()
{
	m_Following = nullptr;
	m_FollowPos = nullptr;
	m_FollowTransform = nullptr;
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

void CCamera::SetScale(const D3DXVECTOR3 & _scale)
{
	m_ScaleFactors = _scale;
}

const D3DXVECTOR3 & CCamera::GetLocalPosition() const
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

D3DXVECTOR3 CCamera::GetScaleFactor()
{
	return m_ScaleFactors;
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

void CCamera::CameraShaking(const float& amount, const float& time)
{
	if (!m_bIsShaking)
	{
		m_bIsShaking = true;
		m_InitialPos = m_CamPos;
		m_fAmountShake = amount;
		m_fShakeTime = time;
	}
}

void CCamera::CameraCloseUp(const D3DXVECTOR3& scale,const float& closeUpTime, const float& time)
{
	m_CloseUpScale = scale;
	m_fCloseUpTime = closeUpTime;
	m_fRemainTime = time;
	m_InitialScale = m_ScaleFactors;
	m_bIsCloseUp = true;
}

bool CCamera::CameraZoomOut()
{
	if (m_fZoomOutTime > 1.0f)
	{
		m_fZoomOutTime -= m_fZoomOutTime;
		m_InitialScale = m_ScaleFactors;
		return true;
	}
	else
	{
		m_ScaleFactors = Lerp(m_CloseUpScale,m_InitialScale, m_fZoomOutTime);
		m_fZoomOutTime += CTimeMgr::GetInstance()->GetDeltaTime();
		return false;
	}
}
