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

private:
	CCamera*	m_MainCam;

};

