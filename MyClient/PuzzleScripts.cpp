#include "stdafx.h"
#include "PuzzleScripts.h"
#include "PuzzlePoint.h"
#include "PuzzleStack.h"
#include "PuzzleSpawn.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"

#include "Effect.h"
CPuzzleScripts::CPuzzleScripts()
{
	m_bPuzzleActive = false;
}


CPuzzleScripts::~CPuzzleScripts()
{
}

void CPuzzleScripts::SetClearWay(CGameObject * pClearWay)
{
	m_pPuzzleWay.push_back(pClearWay);
}

bool CPuzzleScripts::GetPuzzleOn()
{
	return m_PuzzleOn;
}

void CPuzzleScripts::SetPuzzleActive(bool on)
{
	m_bPuzzleActive = on;
}

CPuzzleScripts * CPuzzleScripts::Create(CGameObject * pGameObject ,PUZZLE_TYPE type)
{
	CPuzzleScripts* pScript=nullptr;
	switch (type)
	{
	case POINT:
		 pScript = new CPuzzlePoint;
		break;
	case STACK:
		pScript = new CPuzzleStack;
		break;
	case SPAWN:
		pScript = new CPuzzleSpawn;
		break;
	}
	
	pScript->SetGameObject(pGameObject);

	return pScript;
}
