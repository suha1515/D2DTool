#include "stdafx.h"
#include "STage1Event.h"

#include "PuzzleScripts.h"
#include "Animator.h"
#include "Transform.h"
#include "Camera.h"
#include "BoxCollider.h"

#include "GameObject.h"
#include "Scripts.h"

CSTage1Event::CSTage1Event()
{
	m_Puzzle1 = false;
}


CSTage1Event::~CSTage1Event()
{
}

void CSTage1Event::Initialize()
{
	CPuzzleScripts* m_Puzzles=nullptr;
	//1½ºÅ×ÀÌÁö¿¡¼­ ÇÊ¿äÇÑ ÆÛÁñ¿ä¼Ò Ã£±â.
	//ÆÛÁñ ¿ä¼Ò Ã£±â ÆÛÁñ1 ½ºÀ§Ä¡µé
	//============================================================================
	CGameObject* puzzle_1 = CObjectMgr::GetInstance()->FindObjectWithName(L"ÆÛÁñÆ÷ÀÎÆ®_1").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_1, CPuzzleScripts::PUZZLE_TYPE::POINT);
	puzzle_1->AddScripts(m_Puzzles);
	for (auto&i : puzzle_1->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["ÆÛÁñ1"].push_back(m_Puzzles);
	//============================================================================
	puzzle_1 = CObjectMgr::GetInstance()->FindObjectWithName(L"ÆÛÁñ½ºÅÃ_1").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_1, CPuzzleScripts::PUZZLE_TYPE::STACK);
	puzzle_1->AddScripts(m_Puzzles);
	for (auto&i : puzzle_1->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["ÆÛÁñ1"].push_back(m_Puzzles);
	//============================================================================

	//ÆÛÁñ1 ¿ÀºêÁ§Æ®
	//============================================================================
	CGameObject* puzzlePad = CObjectMgr::GetInstance()->FindObjectWithName(L"ÆÛÁñÆÐµå_1").front();
	m_PuzzlesObject["ÆÛÁñ1"].insert({ "ÆÛÁñÆÐµå_1",puzzlePad });
	CGameObject* puzzleWall = CObjectMgr::GetInstance()->FindObjectWithName(L"¹æ¾îº®_1").front();
	m_PuzzlesObject["ÆÛÁñ1"].insert({ "¹æ¾îº®_1",puzzleWall });
	//============================================================================
}

void CSTage1Event::Update()
{
	if (!m_Puzzle1)
	{
		int puzzle1Active = 0;
		for (auto&i : m_mapPuzzle["ÆÛÁñ1"])
		{
			bool isOn = i->GetPuzzleOn();
			if (isOn)
			{
				puzzle1Active++;		
			}
		}
		if (puzzle1Active == m_mapPuzzle["ÆÛÁñ1"].size())
		{
			m_PuzzlesObject["ÆÛÁñ1"]["ÆÛÁñÆÐµå_1"]->GetComponent<CAnimator>()->Play(L"Pad_On", ANIMATION_ONCE);
			m_PuzzlesObject["ÆÛÁñ1"]["¹æ¾îº®_1"]->SetObjectDestroy(true);
			/*for (auto&i : m_PuzzlesObject["ÆÛÁñ1"]["¹æ¾îº®_1"]->GetChildernVector())
			{
				i->SetObjectDestroy(true);
			}*/
			m_Puzzle1 = true;
			cout << "adaw" << endl;
		}
	}
	
}
