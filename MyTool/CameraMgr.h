#pragma once
class CCamera;
class CCameraMgr
{
	DECLARE_SINGLETON(CCameraMgr)
public:
	CCameraMgr();
	~CCameraMgr();
public:
	void SetMainCamera(CCamera* cam);
public:
	void SetCameraPosition(const D3DXVECTOR3& pos);
	
public:
	CCamera*	GetMainCamera();
	D3DXMATRIX*	GetViewProjMatrix();
private:
	CCamera*	m_MainCam;

};

