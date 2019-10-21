#pragma once
class CCamera;
class CGameObject;
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
	void ShakeCamera(const float& _amount, const float& _time);
	void CameraZoomIn(const D3DXVECTOR3 scale, const float& _closeTime, const float& time);
public:
	void Follow(CGameObject* object);
public:
	CCamera*	GetMainCamera();
	D3DXMATRIX*	GetViewProjMatrix();
private:
	CCamera*	m_MainCam;

};

