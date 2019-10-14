#pragma once
#include "Scene.h"
class CCamera;
class CMouse;
class CTestScene :
	public CScene
{
public:
	CTestScene();
	~CTestScene();

	// CScene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
public:
	static CTestScene* Create();

private:
	CCamera*		m_Cam;
	CMouse*			m_Mouse;

};

