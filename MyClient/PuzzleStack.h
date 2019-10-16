#pragma once
#include "PuzzleScripts.h"
class CPuzzleStack :
	public CPuzzleScripts
{
	enum STATE { STACK_0, STACK_1, STACK_2, STACK_3, STACK_4, FULL_STACK ,CLEAR};
public:
	CPuzzleStack();
	~CPuzzleStack();

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
	virtual void Action() override;

public:
	void AnimState();

private:
	STATE		m_CurState;
	STATE		m_PreState;

	float		m_fStackDownTime;

};

