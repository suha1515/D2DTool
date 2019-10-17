#include "stdafx.h"
#include "PuzzleSpawn.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "Effect.h"
#include "EnemyScripts.h"
#include "Transform.h"

CPuzzleSpawn::CPuzzleSpawn()
{
}


CPuzzleSpawn::~CPuzzleSpawn()
{
}

void CPuzzleSpawn::OnInit()
{
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Puzzle_Spawner");
		m_pAnimator->Play(L"Puzzle_Idle", ANIMATION_ONCE);
	}

	m_CurState = IDLE;
	m_PreState = m_CurState;

	m_PuzzleOn = false;
	m_iSpawnMonster = 0;
}

void CPuzzleSpawn::OnEnable()
{
}

void CPuzzleSpawn::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move)
{
}

void CPuzzleSpawn::OnInput()
{
}

int CPuzzleSpawn::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}
	if(m_bPuzzleActive)
	{
		AnimState();
		if (!m_bActivate)
			Spawning();
		if (m_CurState != CLEAR&&m_CurState == ACTIVE)
			CheckMonster();
	}
	
	return 0;
}

void CPuzzleSpawn::OnLateUpdate()
{
}

void CPuzzleSpawn::OnRender()
{
}

void CPuzzleSpawn::OnDisable()
{
}

void CPuzzleSpawn::OnDestroy()
{
}

void CPuzzleSpawn::Action()
{
}

void CPuzzleSpawn::Spawning()
{
	if (m_iSpawnMonster < m_SpawnLocation.size())
	{
		if (m_fSpawnCoolTime > 1.0f)
		{
			SpawnMonster(ENEMY_TYPE::BOT, m_iSpawnMonster);
			m_iSpawnMonster++;
			m_fSpawnCoolTime -= m_fSpawnCoolTime;
		}
		m_fSpawnCoolTime += CTimeMgr::GetInstance()->GetDeltaTime();
	}
	else
	{
		for (auto&i : m_Monsters)
			dynamic_cast<CEnemyScripts*>(i->GetScript("CEnemyScripts"))->SetOn(true);
		m_bActivate = true;
		m_CurState = ACTIVE;
	}
}

void CPuzzleSpawn::SpawnMonster(ENEMY_TYPE type,int index)
{
	CGameObject* pMonster = CObjectMgr::GetInstance()->AddCopy(L"Bot_Normal", m_PuzzleName);
	CEnemyScripts* enemyScripts = CEnemyScripts::Create(pMonster, BOT);
	enemyScripts->SetOn(false);
	pMonster->AddScripts(enemyScripts);
	
	m_Monsters.push_back(pMonster);

	D3DXVECTOR3 spawnPos = *m_SpawnLocation[index]->GetComponent<CTransform>()->GetWorldPos();

	pMonster->GetComponent<CTransform>()->SetPosition(spawnPos);
}

void CPuzzleSpawn::AnimState()
{
	if (m_PreState != m_CurState)
	{
		switch (m_CurState)
		{
		case IDLE:
			break;
		case ACTIVE:
			m_pAnimator->Play(L"Puzzle_Active", ANIMATION_ONCE);
			break;
		case CLEAR:
			m_PuzzleOn = true;

			break;
		}
	}
}

void CPuzzleSpawn::SetSpawnLocation(CGameObject * pGameObject)
{
	m_SpawnLocation.push_back(pGameObject);
}

void CPuzzleSpawn::CheckMonster()
{
	vector<CGameObject*>& temp = CObjectMgr::GetInstance()->FindObjectWithName(m_PuzzleName);

	if (temp.size() <= 0)
	{
		m_CurState = CLEAR;
	}
}

void CPuzzleSpawn::SetPuzzleName(const wstring & name)
{
	m_PuzzleName = name;
}
