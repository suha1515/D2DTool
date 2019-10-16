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
	//1������������ �ʿ��� ������ ã��.
	//���� ��� ã�� ����1 ����ġ��
	//============================================================================
	CGameObject* puzzle_1 = CObjectMgr::GetInstance()->FindObjectWithName(L"��������Ʈ_1").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_1, CPuzzleScripts::PUZZLE_TYPE::POINT);
	puzzle_1->AddScripts(m_Puzzles);
	for (auto&i : puzzle_1->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["����1"].push_back(m_Puzzles);
	//============================================================================
	puzzle_1 = CObjectMgr::GetInstance()->FindObjectWithName(L"������_1").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_1, CPuzzleScripts::PUZZLE_TYPE::STACK);
	puzzle_1->AddScripts(m_Puzzles);
	for (auto&i : puzzle_1->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["����1"].push_back(m_Puzzles);
	//============================================================================

	//����1 ������Ʈ
	//============================================================================
	CGameObject* puzzlePad = CObjectMgr::GetInstance()->FindObjectWithName(L"�����е�_1").front();
	m_PuzzlesObject["����1"].insert({ "�����е�_1",puzzlePad });
	CGameObject* puzzleWall = CObjectMgr::GetInstance()->FindObjectWithName(L"��_1").front();
	m_PuzzlesObject["����1"].insert({ "��_1",puzzleWall });
	//============================================================================
}

void CSTage1Event::Update()
{
	if (!m_Puzzle1)
	{
		int puzzle1Active = 0;
		for (auto&i : m_mapPuzzle["����1"])
		{
			bool isOn = i->GetPuzzleOn();
			if (isOn)
			{
				puzzle1Active++;		
			}
		}
		if (puzzle1Active == m_mapPuzzle["����1"].size())
		{
			m_PuzzlesObject["����1"]["�����е�_1"]->GetComponent<CAnimator>()->Play(L"Pad_On", ANIMATION_ONCE);
			m_PuzzlesObject["����1"]["��_1"]->SetObjectDestroy(true);
			/*for (auto&i : m_PuzzlesObject["����1"]["��_1"]->GetChildernVector())
			{
				i->SetObjectDestroy(true);
			}*/
			m_Puzzle1 = true;
			cout << "adaw" << endl;
		}
	}
	
}
