#include "stdafx.h"
#include "STage1Event.h"

#include "PuzzleScripts.h"
#include "Animator.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "Camera.h"
#include "BoxCollider.h"

#include "PuzzleSpawn.h"
#include "GameObject.h"
#include "Scripts.h"
#include "EnemyScripts.h"

CSTage1Event::CSTage1Event()
{
	m_Puzzle1 = false;
	m_Puzzle1ObjFade = false;
	m_fAlphaValue = 1.0f;

	m_CurState = NONE;
	m_PreState = m_CurState;
	m_fAlpahValue2 = 0.0f;

	m_fAlphaValue3 = 1.0f;
	
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
	m_Puzzles->SetPuzzleActive(true);
	puzzle_1->AddScripts(m_Puzzles);
	for (auto&i : puzzle_1->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["����1"].insert({ "��������Ʈ_1",m_Puzzles });
	//============================================================================
	puzzle_1 = CObjectMgr::GetInstance()->FindObjectWithName(L"������_1").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_1, CPuzzleScripts::PUZZLE_TYPE::STACK);
	puzzle_1->AddScripts(m_Puzzles);
	m_Puzzles->SetPuzzleActive(true);
	for (auto&i : puzzle_1->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["����1"].insert({ "������_1",m_Puzzles });
	//============================================================================

	//����1 ������Ʈ
	//============================================================================
	CGameObject* puzzlePad = CObjectMgr::GetInstance()->FindObjectWithName(L"�����е�_1").front();
	m_PuzzlesObject["����1"].insert({ "�����е�_1",puzzlePad });
	CGameObject* puzzleWall = CObjectMgr::GetInstance()->FindObjectWithName(L"��_1").front();
	m_PuzzlesObject["����1"].insert({ "��_1",puzzleWall });
	//============================================================================


	//���� 2 ����ġ��
	//============================================================================
	CGameObject* puzzle_2 = CObjectMgr::GetInstance()->FindObjectWithName(L"��������Ʈ_2").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_2, CPuzzleScripts::PUZZLE_TYPE::POINT);
	m_Puzzles->SetPuzzleActive(true);
	puzzle_2->AddScripts(m_Puzzles);
	for (auto&i : puzzle_2->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["����2"].insert({ "��������Ʈ_2",m_Puzzles });

	//============================================================================
	 puzzle_2 = CObjectMgr::GetInstance()->FindObjectWithName(L"��������_2").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_2, CPuzzleScripts::PUZZLE_TYPE::SPAWN);
	m_Puzzles->SetPuzzleActive(false);
	puzzle_2->AddScripts(m_Puzzles);
	m_mapPuzzle["����2"].insert({ "��������_2",m_Puzzles });
	//============================================================================
	//����2 ������Ʈ��
	CGameObject* Spawn= CObjectMgr::GetInstance()->FindObjectWithName(L"����2_�������").front();
	m_PuzzlesObject["����2"].insert({ "����2_�������",Spawn });
	CPuzzleSpawn* pSpawner = dynamic_cast<CPuzzleSpawn*>(m_mapPuzzle["����2"]["��������_2"]);

	pSpawner->SetSpawnLocation(Spawn);
	pSpawner->SetPuzzleName(L"Puzzle2");
	vector<CGameObject*>& child = Spawn->GetChildernVector();
	for (auto&i : child)
	{
		pSpawner->SetSpawnLocation(i);
	}
	CGameObject* puzzleWall2 = CObjectMgr::GetInstance()->FindObjectWithName(L"����2_���ع�").front();
	m_PuzzlesObject["����2"].insert({ "����2_���ع�",puzzleWall2 });
	////============================================================================
	///����2 ���ع� �۾�.
	puzzleWall2->GetComponent<CTextureRenderer>()->SetAlpha(0.0f);
	for (auto&i : puzzleWall2->GetChildernVector())
	{
		i->GetComponent<CTextureRenderer>()->SetAlpha(0.0f);
	}
	//����3 ����ġ �۾�==================================================================

	CGameObject* puzzle_3 = CObjectMgr::GetInstance()->FindObjectWithName(L"����3_����Ʈ").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_3, CPuzzleScripts::PUZZLE_TYPE::POINT);
	m_Puzzles->SetPuzzleActive(true);
	puzzle_3->AddScripts(m_Puzzles);
	for (auto&i : puzzle_3->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["����3"].insert({ "����3_����Ʈ",m_Puzzles });

	//============================================================================
	puzzle_3 = CObjectMgr::GetInstance()->FindObjectWithName(L"����3_����Ʈ2").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_3, CPuzzleScripts::PUZZLE_TYPE::POINT);
	m_Puzzles->SetPuzzleActive(true);
	puzzle_3->AddScripts(m_Puzzles);
	for (auto&i : puzzle_3->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["����3"].insert({ "����3_����Ʈ2",m_Puzzles });

	//=================================================================================

	//����3 ������Ʈ �۾�=================================================================

	CGameObject* puzzle_Barrier = CObjectMgr::GetInstance()->FindObjectWithName(L"����_3�踮��_����").front();
	m_PuzzlesObject["����3"].insert({ "����3_���ع�",puzzle_Barrier });
	CGameObject* pTurret = CObjectMgr::GetInstance()->FindObjectWithName(L"����3_�ͷ�").front();
	pTurret->SetObjectTag(L"Enemy");
	pTurret->AddScripts(CEnemyScripts::Create(pTurret, ENEMY_TYPE::TURRET));

	m_PuzzlesObject["����3"].insert({ "����3_�ͷ�",pTurret });
	pTurret = CObjectMgr::GetInstance()->FindObjectWithName(L"����3_�ͷ�2").front();
	pTurret->SetObjectTag(L"Enemy");
	pTurret->AddScripts(CEnemyScripts::Create(pTurret, ENEMY_TYPE::TURRET));

	m_PuzzlesObject["����3"].insert({ "����3_�ͷ�2",pTurret });
	pTurret = CObjectMgr::GetInstance()->FindObjectWithName(L"����3_�ͷ�3").front();
	pTurret->SetObjectTag(L"Enemy");
	pTurret->AddScripts(CEnemyScripts::Create(pTurret, ENEMY_TYPE::TURRET));

	m_PuzzlesObject["����3"].insert({ "����3_�ͷ�3",pTurret });
	pTurret = CObjectMgr::GetInstance()->FindObjectWithName(L"����3_�ͷ�4").front();
	pTurret->SetObjectTag(L"Enemy");
	pTurret->AddScripts(CEnemyScripts::Create(pTurret, ENEMY_TYPE::TURRET));

	m_PuzzlesObject["����3"].insert({ "����3_�ͷ�4",pTurret });

	CGameObject* Stone = CObjectMgr::GetInstance()->FindObjectWithName(L"��������").front();
	m_PuzzlesObject["����3"].insert({ "��������",Stone });



	//=================================================================================
}

void CSTage1Event::Update()
{
	//if (!m_Puzzle1)
	//{
	//	int puzzle1Active = 0;
	//	for (auto&i : m_mapPuzzle["����1"])
	//	{
	//		bool isOn = i.second->GetPuzzleOn();
	//		if (isOn)
	//		{
	//			puzzle1Active++;
	//		}
	//	}
	//	if (puzzle1Active == m_mapPuzzle["����1"].size())
	//	{
	//		m_PuzzlesObject["����1"]["�����е�_1"]->GetComponent<CAnimator>()->Play(L"Pad_On", ANIMATION_ONCE);
	//		m_Puzzle1ObjFade = true;
	//		m_Puzzle1 = true;
	//	}
	//}
	//if (m_Puzzle1ObjFade)
	//{
	//	if (m_fAlphaValue >= 0.0f)
	//	{
	//		m_PuzzlesObject["����1"]["��_1"]->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue);
	//		for (auto&i : m_PuzzlesObject["����1"]["��_1"]->GetChildernVector())
	//		{
	//			i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue);
	//		}
	//		m_fAlphaValue -= CTimeMgr::GetInstance()->GetDeltaTime()*0.5f;
	//	}
	//	else
	//	{
	//		m_PuzzlesObject["����1"]["��_1"]->SetObjectDestroy(true);
	//		m_Puzzle1ObjFade = false;
	//		m_fAlphaValue = 1.f;
	//	}	
	//}

	////����2=====================================================================
	//if (!m_Puzzle2Clear)
	//{
	//	if (m_mapPuzzle["����2"]["��������Ʈ_2"]->GetPuzzleOn())
	//	{
	//		
	//		if (!m_Puzzle2WallFade)
	//		{
	//			CGameObject* wall = m_PuzzlesObject["����2"]["����2_���ع�"];
	//			if (m_fAlpahValue2 <= 1.0f)
	//			{
	//				wall->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlpahValue2);
	//				for (auto&i : wall->GetChildernVector())
	//				{
	//					i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlpahValue2);
	//				}
	//				m_fAlpahValue2 += CTimeMgr::GetInstance()->GetDeltaTime()*0.7f;
	//			}
	//			else
	//			{
	//				m_Puzzle2WallFade = true;
	//				m_fAlpahValue2 = 1.0f;
	//			}
	//		}
	//		else
	//			m_mapPuzzle["����2"]["��������_2"]->SetPuzzleActive(true);
	//	}
	//	if (m_mapPuzzle["����2"]["��������_2"]->GetPuzzleOn())
	//	{
	//		cout << "����2 ������ Ŭ����" << endl;
	//		CGameObject* wall = m_PuzzlesObject["����2"]["����2_���ع�"];
	//		if (m_fAlpahValue2 >= 0.0f)
	//		{
	//			wall->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlpahValue2);
	//			for (auto&i : wall->GetChildernVector())
	//			{
	//				i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlpahValue2);
	//			}
	//			m_fAlpahValue2 -= CTimeMgr::GetInstance()->GetDeltaTime()*0.7f;
	//		}
	//		else
	//		{
	//			wall->SetObjectDestroy(true);
	//			m_Puzzle2Clear = true;
	//			m_fAlpahValue2 =0.0f;
	//		}
	//	}
	//}	

	////����3=====================================================================================================

	//if (!m_Puzzle3Clear)
	//{
	//	if (m_mapPuzzle["����3"]["����3_����Ʈ2"]->GetPuzzleOn())
	//	{
	//		if (!m_Puzzle3WallFde)
	//		{
	//			if (m_fAlphaValue3 >= 0.0f)
	//			{
	//				m_PuzzlesObject["����3"]["����3_���ع�"]->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue3);
	//				for (auto&i : m_PuzzlesObject["����3"]["����3_���ع�"]->GetChildernVector())
	//				{
	//					i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue3);
	//				}
	//				m_fAlphaValue3 -= CTimeMgr::GetInstance()->GetDeltaTime()*0.7f;
	//			}
	//			else
	//			{
	//				m_PuzzlesObject["����3"]["����3_���ع�"]->SetObjectDestroy(true);
	//				m_Puzzle3WallFde = true;
	//				m_fAlphaValue3 = 1.0f;
	//			}
	//		}
	//	}
	//}
	//

	//=========================================================================================================
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
					bool isOn = i.second->GetPuzzleOn();
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
