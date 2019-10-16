#pragma once
#include "Scripts.h"
class CTransform;
class CAnimator;
class CPuzzleScripts :
	public CScripts
{
	enum STATE{IDLE,HIT,WAY,LIGHT,OPEN};
public:
	CPuzzleScripts();
	~CPuzzleScripts();

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
	void AnimState();
	void SetOnObject(CGameObject* pGameObject);
	void SetWallObject(CGameObject* pGameObject);
	void SetPuzzleWay(CGameObject* pGameObject);

	bool GetPuzzleOn();
private:
	CAnimator*			m_pAnimator;
	CTransform*			m_pTransform;

	CGameObject*		m_pPuzzlePad;
	vector<CGameObject*> m_pPuzzleWall;
	list<CGameObject*> m_pPuzzleWay;

	STATE				m_CurState;
	STATE				m_PreState;

	float			  m_fTime;

	bool				m_PuzzleOn;
	bool				m_PuzzleLight;
	bool				m_PuzzleOpen;

public:
	static CPuzzleScripts* Create(CGameObject* pGameObject);
};

