#pragma once
#include "PuzzleScripts.h"

class CPuzzlePoint :
	public CPuzzleScripts
{
	enum STATE{IDLE,HIT,CLEAR};
public:
	CPuzzlePoint();
	~CPuzzlePoint();

	// CPuzzleScripts을(를) 통해 상속됨
	virtual void Action() override;

	// CPuzzleScripts을(를) 통해 상속됨
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
	void		AnimState();

private:

	STATE		m_CurState;
	STATE		m_PreState;

};

