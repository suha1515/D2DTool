#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CPuzzleScripts :
	public CScripts
{
public:
	static enum PUZZLE_TYPE{POINT,STACK,SPAWN};
	CPuzzleScripts();
	~CPuzzleScripts();

	// CScripts을(를) 통해 상속됨
	virtual void OnInit()=0;
	virtual void OnEnable()=0;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr)=0;
	virtual void OnInput()=0;
	virtual int OnUpdate()=0;
	virtual void OnLateUpdate() =0;
	virtual void OnRender() =0;
	virtual void OnDisable() =0;
	virtual void OnDestroy() =0;
public:
	virtual void Action()=0;
	void		SetClearWay(CGameObject* pClearWay);
	bool		GetPuzzleOn();
public:

	void		SetPuzzleActive(bool on);

protected:
	CAnimator*			m_pAnimator;
	CTransform*			m_pTransform;

	list<CGameObject*> m_pPuzzleWay;
	float				 m_fTime;

	bool				m_PuzzleOn;
	PUZZLE_TYPE			m_PuzzleType;

	bool				m_bPuzzleActive;
public:
	static CPuzzleScripts* Create(CGameObject* pGameObject,PUZZLE_TYPE type);
};

