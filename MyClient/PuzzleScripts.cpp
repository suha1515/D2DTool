#include "stdafx.h"
#include "PuzzleScripts.h"
#include "PuzzlePoint.h"
#include "PuzzleStack.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"

#include "Effect.h"
CPuzzleScripts::CPuzzleScripts()
{
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
	}
	
	pScript->SetGameObject(pGameObject);

	return pScript;
}
