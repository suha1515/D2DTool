#include "stdafx.h"
#include "STage1Event.h"

#include "PuzzleScripts.h"
#include "Animator.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "Camera.h"
#include "BoxCollider.h"

#include "GameObject.h"
#include "Scripts.h"

CSTage1Event::CSTage1Event()
{
	m_Puzzle1 = false;
	m_Puzzle1ObjFade = false;
	m_fAlphaValue = 1.0f;

	m_CurState = NONE;
	m_PreState = m_CurState;
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
			m_Puzzle1ObjFade = true;
			m_Puzzle1 = true;
		}
	}
	if (m_Puzzle1ObjFade)
	{
		if (m_fAlphaValue >= 0.0f)
		{
			m_PuzzlesObject["����1"]["��_1"]->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue);
			for (auto&i : m_PuzzlesObject["����1"]["��_1"]->GetChildernVector())
			{
				i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue);
			}
			m_fAlphaValue -= CTimeMgr::GetInstance()->GetDeltaTime()*0.5f;
			cout << m_fAlphaValue << endl;
		}
		else
		{
			m_PuzzlesObject["����1"]["��_1"]->SetObjectDestroy(true);
			m_Puzzle1ObjFade = false;
			m_fAlphaValue = 1.f;
		}	
	}
	
}

void CSTage1Event::EventState()
{
	if (m_CurState != m_PreState)
	{
		switch (m_CurState)
		{
		case PUZZLE1:
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
					m_Puzzle1ObjFade = true;
					m_Puzzle1 = true;
				}
			}
			break;
		case PUZZLE2:
			break;

		default:
			break;
		}
	}
}
