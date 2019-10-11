#pragma once
class CGameObject;
class CTransform;
class CMouse
{
public:
	CMouse();
	~CMouse();
public:
	void Initialize();
public:
	void Update();
public:
	void SetMouse(CGameObject* pMouse);
public:
	D3DXVECTOR3 GetMousePos();

private:
	CTransform*		m_pTransform;

	CGameObject* m_pMouse;
	POINT		 m_Point;

	XMFLOAT3 scaleFactor;
};

