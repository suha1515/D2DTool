#pragma once

class CTransform;
class CGameObject;
class CCamera
{
public:
	CCamera();
	~CCamera();

public:
	void					Initialize(int width, int height, float angle, D3DXVECTOR3 scaleFactor);

public:
	void					Update();
	void					LateUpdate();
	void					Follow(CGameObject* object);
	void					UnFollow();
	bool					IsFollowing() const;
	void					SetTransform() const;

public:
	void					SetPosisiton(const D3DXVECTOR3& pos);
	void					SetScale(const float& _x, const float& _y);
	void					SetScale(const D3DXVECTOR3& _scale);
	const	D3DXVECTOR3&	GetLocalPosition()	const;
public:
	D3DXMATRIX*				GetViewMatrix();
	D3DXMATRIX*				GetProjectMatrix();
	D3DXMATRIX*				GetViewProjMatrix();
	D3DXVECTOR3				GetScaleFactor();
public:
	void					Scaling(const float&_x = 0.0f, const float& _y = 0.0f);
	void					MoveCamera(const D3DXVECTOR3& pos);
	void					MoveCamera(const float& _x, const float& _y);

public:
	void					CameraShaking(const float& amount,const float& time);
	void					CameraCloseUp(const D3DXVECTOR3& scale,const float& closeUpTime,const float& time);
	bool					CameraZoomOut();

private:
	float m_fAngle;
	D3DXVECTOR3				m_ScaleFactors;

	D3DXMATRIX				m_OrthographicMat;
	D3DXMATRIX				m_IdentityMat;
	D3DXMATRIX				m_ViewMat;
	D3DXMATRIX				m_ViewProjMat;

	CGameObject*			m_Following;
	CTransform*			    m_FollowTransform;
	D3DXVECTOR3*			m_FollowPos;

	int width;
	int height;
	bool					m_bIsShaking=false;
	float					m_fAmountShake=0.0f;
	float					m_fShakeTime=0.0f;
	D3DXVECTOR3				m_InitialPos;
	D3DXVECTOR3				m_PreCamPos;

	bool					m_bIsCloseUp = false;
	float					m_fCloseUpTime=0.0f;
	float					m_fCloseTime = 0.0f;
	float					m_fRemainTime = 0.0f;
	float					m_fZoomOutTime=0.0f;
	D3DXVECTOR3				m_CloseUpScale;
	D3DXVECTOR3				m_InitialScale;

	D3DXVECTOR3				m_CamPos;
	CDeviceMgr*				m_pDevice;

};

