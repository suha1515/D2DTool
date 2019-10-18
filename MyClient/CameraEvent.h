#pragma once
#include "Scripts.h"

class CCameraEvent :
	public CScripts
{
public:
	CCameraEvent();
	~CCameraEvent();

	// CScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr) override;
	virtual void OnInput() override;
	virtual int OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;

public:
	void GetCameraRoute(CGameObject* pos,float _restTIme);
	void GetCameraRoute(vector<pair<CGameObject*, float>> camRoute);
	void MoveCamera();

public:
	bool GetOn();
	bool GetEventEnd();
	void SetObjectDead();
	void CutCamera();
	void SetCameraOn();
private:
	bool	m_bIsEventOn;
	bool	m_bIsEventEnd;

	vector<pair<CGameObject*,float>> m_CamRoute;
	int					m_iIndexRoute;
	CCameraMgr*			m_pCameraMgr;

	float				m_RestTime;

public:
	static CCameraEvent* Create(vector<pair<CGameObject*,float>> camRoute);
};

