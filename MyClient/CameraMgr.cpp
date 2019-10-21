#include "stdafx.h"
#include "CameraMgr.h"
#include "Camera.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CCameraMgr);
CCameraMgr::CCameraMgr()
{

}


CCameraMgr::~CCameraMgr()
{
}

void CCameraMgr::SetMainCamera(CCamera * cam)
{
	m_MainCam = cam;
}

void CCameraMgr::SetCameraPosition(const D3DXVECTOR3 & pos)
{
	m_MainCam->SetPosisiton(pos);
}

void CCameraMgr::ShakeCamera(const float & _amount, const float& _time)
{
	m_MainCam->CameraShaking(_amount, _time);
}

void CCameraMgr::CameraZoomIn(const D3DXVECTOR3 scale, const float & _closeTime, const float & time)
{
	m_MainCam->CameraCloseUp(scale, _closeTime, time);
}

void CCameraMgr::Follow(CGameObject * object)
{
	m_MainCam->Follow(object);
}


CCamera * CCameraMgr::GetMainCamera()
{
	return m_MainCam;
}

D3DXMATRIX* CCameraMgr::GetViewProjMatrix()
{
	return m_MainCam->GetViewProjMatrix();
}
