#pragma once

class CGameObject;
class CCamera
{
public:
	CCamera();
	~CCamera();

public:
	void					Initialize(int width, int height, float angle, XMFLOAT3 scaleFactor);

public:
	void					Update();
	void					Follow(CGameObject* object);
	void					UnFollow();
	bool					IsFollowing() const;
	void					SetTransform() const;

public:
	void					SetPosisiton(const D3DXVECTOR3& pos);
	const	D3DXVECTOR3&	GetPosition()	const;
public:
	D3DXMATRIX*				GetViewMatrix();
	D3DXMATRIX*				GetProjectMatrix();
	D3DXMATRIX*				GetViewProjMatrix();

public:
	void					MoveCamera(const D3DXVECTOR3& pos);
	void					MoveCamera(const float& _x, const float& _y);

private:
	float m_fAngle;
	XMFLOAT3				m_ScaleFactors;

	D3DXMATRIX				m_OrthographicMat;
	D3DXMATRIX				m_IdentityMat;
	D3DXMATRIX				m_ViewMat;
	D3DXMATRIX				m_ViewProjMat;

	CGameObject*			m_Following;

	int width;
	int height;

	D3DXVECTOR3				m_CamPos;
	CDeviceMgr*				m_pDevice;

};

