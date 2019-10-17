#pragma once
#include "PuzzleScripts.h"
class CPuzzleSpawn :
	public CPuzzleScripts
{
	enum STATE {IDLE,ACTIVE,CLEAR};
public:
	CPuzzleSpawn();
	~CPuzzleSpawn();

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
	void Spawning();
	void SpawnMonster(ENEMY_TYPE type,int index);
	void AnimState();
	void SetSpawnLocation(CGameObject* pGameObject);
	void CheckMonster();
	void SetPuzzleName(const wstring& name);
private:
	int			m_iSpawnMonster;

	vector<CGameObject*> m_SpawnLocation;
	list <CGameObject*>	m_Monsters;

private:
	STATE		m_CurState;
	STATE		m_PreState;

	float	    m_fSpawnCoolTime;
	wstring		m_PuzzleName;

	bool		m_bActivate=false;





};

