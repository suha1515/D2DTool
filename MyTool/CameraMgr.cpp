#include "stdafx.h"
#include "CameraMgr.h"
#include "Camera.h"

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

CCamera * CCameraMgr::GetMainCamera()
{
	return m_MainCam;
}

D3DXMATRIX* CCameraMgr::GetViewProjMatrix()
{
	return m_MainCam->GetViewProjMatrix();
}
