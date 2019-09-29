#pragma once
#include "Component.h"
//

class CShader : public CComponent
{
public:
	CShader();
	~CShader();
public:
	LPD3DXEFFECT  GetEffect();
public:
	void		  SetEffect(const wstring& filePath);
public:

private:
	LPD3DXEFFECT m_Effect;

private:
	CDeviceMgr*			m_pDeviceMgr;

	// CComponent을(를) 통해 상속됨
	virtual void Action(CGameObject * pObject) override;
};

