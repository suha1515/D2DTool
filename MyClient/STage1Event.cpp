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
	//1Ω∫≈◊¿Ã¡ˆø°º≠ « ø‰«— ∆€¡Òø‰º“ √£±‚.
	//∆€¡Ò ø‰º“ √£±‚ ∆€¡Ò1 Ω∫¿ßƒ°µÈ
	//============================================================================
	CGameObject* puzzle_1 = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò∆˜¿Œ∆Æ_1").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_1, CPuzzleScripts::PUZZLE_TYPE::POINT);
	m_Puzzles->SetPuzzleActive(true);
	puzzle_1->AddScripts(m_Puzzles);
	for (auto&i : puzzle_1->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["∆€¡Ò1"].insert({ "∆€¡Ò∆˜¿Œ∆Æ_1",m_Puzzles });
	//============================================================================
	puzzle_1 = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡ÒΩ∫≈√_1").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_1, CPuzzleScripts::PUZZLE_TYPE::STACK);
	puzzle_1->AddScripts(m_Puzzles);
	m_Puzzles->SetPuzzleActive(true);
	for (auto&i : puzzle_1->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["∆€¡Ò1"].insert({ "∆€¡ÒΩ∫≈√_1",m_Puzzles });
	//============================================================================

	//∆€¡Ò1 ø¿∫Í¡ß∆Æ
	//============================================================================
	CGameObject* puzzlePad = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò∆–µÂ_1").front();
	m_PuzzlesObject["∆€¡Ò1"].insert({ "∆€¡Ò∆–µÂ_1",puzzlePad });
	CGameObject* puzzleWall = CObjectMgr::GetInstance()->FindObjectWithName(L"πÊæÓ∫Æ_1").front();
	m_PuzzlesObject["∆€¡Ò1"].insert({ "πÊæÓ∫Æ_1",puzzleWall });
	//============================================================================


	//∆€¡Ò 2 Ω∫¿ßƒ°µÈ
	//============================================================================
	CGameObject* puzzle_2 = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò∆˜¿Œ∆Æ_2").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_2, CPuzzleScripts::PUZZLE_TYPE::POINT);
	m_Puzzles->SetPuzzleActive(true);
	puzzle_2->AddScripts(m_Puzzles);
	for (auto&i : puzzle_2->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["∆€¡Ò2"].insert({ "∆€¡Ò∆˜¿Œ∆Æ_2",m_Puzzles });

	//============================================================================
	 puzzle_2 = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡ÒΩ∫∆˜≥ _2").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_2, CPuzzleScripts::PUZZLE_TYPE::SPAWN);
	m_Puzzles->SetPuzzleActive(false);
	puzzle_2->AddScripts(m_Puzzles);
	m_mapPuzzle["∆€¡Ò2"].insert({ "∆€¡ÒΩ∫∆˜≥ _2",m_Puzzles });
	//============================================================================
	//∆€¡Ò2 ø¿∫Í¡ß∆ÆµÈ
	CGameObject* Spawn= CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò2_Ω∫∆˘¿Âº“").front();
	m_PuzzlesObject["∆€¡Ò2"].insert({ "∆€¡Ò2_Ω∫∆˘¿Âº“",Spawn });
	CPuzzleSpawn* pSpawner = dynamic_cast<CPuzzleSpawn*>(m_mapPuzzle["∆€¡Ò2"]["∆€¡ÒΩ∫∆˜≥ _2"]);

	pSpawner->SetSpawnLocation(Spawn);
	pSpawner->SetPuzzleName(L"Puzzle2");
	vector<CGameObject*>& child = Spawn->GetChildernVector();
	for (auto&i : child)
	{
		pSpawner->SetSpawnLocation(i);
	}
	CGameObject* puzzleWall2 = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò2_πÊ«ÿπ∞").front();
	m_PuzzlesObject["∆€¡Ò2"].insert({ "∆€¡Ò2_πÊ«ÿπ∞",puzzleWall2 });
	////============================================================================
	///∆€¡Ò2 πÊ«ÿπ∞ ¿€æ˜.
	puzzleWall2->GetComponent<CTextureRenderer>()->SetAlpha(0.0f);
	for (auto&i : puzzleWall2->GetChildernVector())
	{
		i->GetComponent<CTextureRenderer>()->SetAlpha(0.0f);
	}
	//∆€¡Ò3 Ω∫¿ßƒ° ¿€æ˜==================================================================

	CGameObject* puzzle_3 = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò3_∆˜¿Œ∆Æ").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_3, CPuzzleScripts::PUZZLE_TYPE::POINT);
	m_Puzzles->SetPuzzleActive(true);
	puzzle_3->AddScripts(m_Puzzles);
	for (auto&i : puzzle_3->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["∆€¡Ò3"].insert({ "∆€¡Ò3_∆˜¿Œ∆Æ",m_Puzzles });

	//============================================================================
	puzzle_3 = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò3_∆˜¿Œ∆Æ2").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_3, CPuzzleScripts::PUZZLE_TYPE::POINT);
	m_Puzzles->SetPuzzleActive(true);
	puzzle_3->AddScripts(m_Puzzles);
	for (auto&i : puzzle_3->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["∆€¡Ò3"].insert({ "∆€¡Ò3_∆˜¿Œ∆Æ2",m_Puzzles });

	//=================================================================================

	//∆€¡Ò3 ø¿∫Í¡ß∆Æ ¿€æ˜=================================================================

	CGameObject* puzzle_Barrier = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò_3πË∏ÆæÓ_ª°∞≠").front();
	m_PuzzlesObject["∆€¡Ò3"].insert({ "∆€¡Ò3_πÊ«ÿπ∞",puzzle_Barrier });
	CGameObject* pTurret = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò3_≈Õ∑ø").front();
	pTurret->SetObjectTag(L"Enemy");
	pTurret->AddScripts(CEnemyScripts::Create(pTurret, ENEMY_TYPE::TURRET));

	m_PuzzlesObject["∆€¡Ò3"].insert({ "∆€¡Ò3_≈Õ∑ø",pTurret });
	pTurret = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò3_≈Õ∑ø2").front();
	pTurret->SetObjectTag(L"Enemy");
	pTurret->AddScripts(CEnemyScripts::Create(pTurret, ENEMY_TYPE::TURRET));

	m_PuzzlesObject["∆€¡Ò3"].insert({ "∆€¡Ò3_≈Õ∑ø2",pTurret });
	pTurret = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò3_≈Õ∑ø3").front();
	pTurret->SetObjectTag(L"Enemy");
	pTurret->AddScripts(CEnemyScripts::Create(pTurret, ENEMY_TYPE::TURRET));

	m_PuzzlesObject["∆€¡Ò3"].insert({ "∆€¡Ò3_≈Õ∑ø3",pTurret });
	pTurret = CObjectMgr::GetInstance()->FindObjectWithName(L"∆€¡Ò3_≈Õ∑ø4").front();
	pTurret->SetObjectTag(L"Enemy");
	pTurret->AddScripts(CEnemyScripts::Create(pTurret, ENEMY_TYPE::TURRET));

	m_PuzzlesObject["∆€¡Ò3"].insert({ "∆€¡Ò3_≈Õ∑ø4",pTurret });

	CGameObject* Stone = CObjectMgr::GetInstance()->FindObjectWithName(L"µππ´¥ı±‚").front();
	m_PuzzlesObject["∆€¡Ò3"].insert({ "µππ´¥ı±‚",Stone });



	//=================================================================================
}

void CSTage1Event::Update()
{
	//if (!m_Puzzle1)
	//{
	//	int puzzle1Active = 0;
	//	for (auto&i : m_mapPuzzle["∆€¡Ò1"])
	//	{
	//		bool isOn = i.second->GetPuzzleOn();
	//		if (isOn)
	//		{
	//			puzzle1Active++;
	//		}
	//	}
	//	if (puzzle1Active == m_mapPuzzle["∆€¡Ò1"].size())
	//	{
	//		m_PuzzlesObject["∆€¡Ò1"]["∆€¡Ò∆–µÂ_1"]->GetComponent<CAnimator>()->Play(L"Pad_On", ANIMATION_ONCE);
	//		m_Puzzle1ObjFade = true;
	//		m_Puzzle1 = true;
	//	}
	//}
	//if (m_Puzzle1ObjFade)
	//{
	//	if (m_fAlphaValue >= 0.0f)
	//	{
	//		m_PuzzlesObject["∆€¡Ò1"]["πÊæÓ∫Æ_1"]->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue);
	//		for (auto&i : m_PuzzlesObject["∆€¡Ò1"]["πÊæÓ∫Æ_1"]->GetChildernVector())
	//		{
	//			i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue);
	//		}
	//		m_fAlphaValue -= CTimeMgr::GetInstance()->GetDeltaTime()*0.5f;
	//	}
	//	else
	//	{
	//		m_PuzzlesObject["∆€¡Ò1"]["πÊæÓ∫Æ_1"]->SetObjectDestroy(true);
	//		m_Puzzle1ObjFade = false;
	//		m_fAlphaValue = 1.f;
	//	}	
	//}

	////∆€¡Ò2=====================================================================
	//if (!m_Puzzle2Clear)
	//{
	//	if (m_mapPuzzle["∆€¡Ò2"]["∆€¡Ò∆˜¿Œ∆Æ_2"]->GetPuzzleOn())
	//	{
	//		
	//		if (!m_Puzzle2WallFade)
	//		{
	//			CGameObject* wall = m_PuzzlesObject["∆€¡Ò2"]["∆€¡Ò2_πÊ«ÿπ∞"];
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
	//			m_mapPuzzle["∆€¡Ò2"]["∆€¡ÒΩ∫∆˜≥ _2"]->SetPuzzleActive(true);
	//	}
	//	if (m_mapPuzzle["∆€¡Ò2"]["∆€¡ÒΩ∫∆˜≥ _2"]->GetPuzzleOn())
	//	{
	//		cout << "∆€¡Ò2 Ω∫∆˜≥  ≈¨∏ÆæÓ" << endl;
	//		CGameObject* wall = m_PuzzlesObject["∆€¡Ò2"]["∆€¡Ò2_πÊ«ÿπ∞"];
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

	////∆€¡Ò3=====================================================================================================

	//if (!m_Puzzle3Clear)
	//{
	//	if (m_mapPuzzle["∆€¡Ò3"]["∆€¡Ò3_∆˜¿Œ∆Æ2"]->GetPuzzleOn())
	//	{
	//		if (!m_Puzzle3WallFde)
	//		{
	//			if (m_fAlphaValue3 >= 0.0f)
	//			{
	//				m_PuzzlesObject["∆€¡Ò3"]["∆€¡Ò3_πÊ«ÿπ∞"]->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue3);
	//				for (auto&i : m_PuzzlesObject["∆€¡Ò3"]["∆€¡Ò3_πÊ«ÿπ∞"]->GetChildernVector())
	//				{
	//					i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue3);
	//				}
	//				m_fAlphaValue3 -= CTimeMgr::GetInstance()->GetDeltaTime()*0.7f;
	//			}
	//			else
	//			{
	//				m_PuzzlesObject["∆€¡Ò3"]["∆€¡Ò3_πÊ«ÿπ∞"]->SetObjectDestroy(true);
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
				for (auto&i : m_mapPuzzle["∆€¡Ò1"])
				{
					bool isOn = i.second->GetPuzzleOn();
					if (isOn)
					{
						puzzle1Active++;
					}
				}
				if (puzzle1Active == m_mapPuzzle["∆€¡Ò1"].size())
				{
					m_PuzzlesObject["∆€¡Ò1"]["∆€¡Ò∆–µÂ_1"]->GetComponent<CAnimator>()->Play(L"Pad_On", ANIMATION_ONCE);
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
