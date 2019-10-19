#pragma once
#include "Scene.h"
class CCamera;
class CMouse;
class CBossStage :
	public CScene
{
public:
	CBossStage();
	~CBossStage();

	// CScene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CBossStage* Create();

private:
	CCamera*		m_Cam;
	CMouse*			m_Mouse;

	
};

