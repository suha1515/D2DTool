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
	CCamera*	GetMainCamera();
	D3DXMATRIX*	GetViewProjMatrix();
private:
	CCamera*	m_MainCam;

};

