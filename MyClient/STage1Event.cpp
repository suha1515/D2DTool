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
#include "DestructiveObject.h"
#include "CameraEvent.h"

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
	m_CameraEvents.clear();
	m_mapPuzzle.clear();
	m_PuzzlesObject.clear();
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
	
	CDestructiveObject* pScript = new CDestructiveObject;
	Stone->AddScripts(pScript);
	pScript->SetGameObject(Stone);
	for (auto&i : Stone->GetChildernVector())
	{
		CDestructiveObject* pScript = new CDestructiveObject;
		i->AddScripts(pScript);
		pScript->SetGameObject(i);
	}
	//=================================================================================

	//����4 ����ġ �۾�.

	CGameObject* puzzle_4 = CObjectMgr::GetInstance()->FindObjectWithName(L"����4_����Ʈ").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle_4, CPuzzleScripts::PUZZLE_TYPE::POINT);
	m_Puzzles->SetPuzzleActive(true);
	puzzle_4->AddScripts(m_Puzzles);
	for (auto&i : puzzle_4->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetClearWay(i);
	}
	m_mapPuzzle["����4"].insert({ "����4_����Ʈ",m_Puzzles });
	//=================================================================================

	//����4 ������Ʈ �۾�=================================================================

	CGameObject* Door = CObjectMgr::GetInstance()->FindObjectWithName(L"����4_��").front();
	m_PuzzlesObject["����4"].insert({ "����4_��",Door });
	Door->GetComponent<CAnimator>()->Play(L"Close", ANIMATION_ONCE);
	CGameObject* pTurret4 = CObjectMgr::GetInstance()->FindObjectWithName(L"����4_�ͷ�").front();
	pTurret4->SetObjectTag(L"Enemy");
	pTurret4->AddScripts(CEnemyScripts::Create(pTurret4, ENEMY_TYPE::TURRET));

	m_PuzzlesObject["����4"].insert({ "����4_�ͷ�",pTurret4 });
	//ī�޶� �̺�Ʈ �۾�

	CGameObject* camera_Event1 = CObjectMgr::GetInstance()->FindObjectWithName(L"ī�޶�_�̺�Ʈ1").front();
	vector<pair<CGameObject*, float>> temp;
	vector<CGameObject*> child2 = camera_Event1->GetChildernVector();
	temp.push_back(pair<CGameObject*, float>(child2[0], 4.0f));
	temp.push_back(pair<CGameObject*, float>(child2[1], 3.0f));
	temp.push_back(pair<CGameObject*, float>(child2[2], 3.0f));
	CCameraEvent* Event = CCameraEvent::Create(temp);
	camera_Event1->AddScripts(Event);
	Event->SetGameObject(camera_Event1);

	m_CameraEvents.insert({ "ī�޶�_�̺�Ʈ1",Event });

	temp.clear();
	child2.clear();
	CGameObject* camera_Event2 = CObjectMgr::GetInstance()->FindObjectWithName(L"ī�޶�_�̺�Ʈ2").front();

	child2 = camera_Event2->GetChildernVector();
	temp.push_back(pair<CGameObject*, float>(child2[0], 10.0f));
	CCameraEvent* Event2 = CCameraEvent::Create(temp);
	camera_Event2->AddScripts(Event2);
	Event2->SetGameObject(camera_Event2);

	m_CameraEvents.insert({ "ī�޶�_�̺�Ʈ2",Event2 });

	temp.clear();
	CGameObject* camera_Event3 = CObjectMgr::GetInstance()->FindObjectWithName(L"ī�޶�_�̺�Ʈ4").front();
	CCameraEvent* Event3 = CCameraEvent::Create(temp);
	camera_Event3->AddScripts(Event3);
	Event3->SetGameObject(camera_Event3);
	m_CameraEvents.insert({ "ī�޶�_�̺�Ʈ4",Event3 });

}

void CSTage1Event::Update()
{
	if (!m_bIsChangeScene)
	{
		CameraEventUpdate();
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
				CSoundMgr::GetInstance()->PlaySound(L"highlight-switch-2.ogg", CSoundMgr::EFFECT);
			}
		}
		if (m_Puzzle1ObjFade)
		{
			if (m_fAlphaValue >= 0.0f)
			{
				if (!m_PlaySoundOnce)
				{
					CSoundMgr::GetInstance()->PlaySound(L"�踮��ٿ�.ogg", CSoundMgr::EFFECT);
					m_PlaySoundOnce = true;
				}
				m_PuzzlesObject["����1"]["��_1"]->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue);
				for (auto&i : m_PuzzlesObject["����1"]["��_1"]->GetChildernVector())
				{
					i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue);
				}
				m_fAlphaValue -= CTimeMgr::GetInstance()->GetDeltaTime()*0.5f;
			}
			else
			{
				m_PuzzlesObject["����1"]["��_1"]->SetObjectDestroy(true);
				m_Puzzle1ObjFade = false;
				m_PlaySoundOnce = false;
				m_fAlphaValue = 1.f;
			}
		}

		//����2=====================================================================
		if (!m_Puzzle2Clear)
		{
			if (m_mapPuzzle["����2"]["��������Ʈ_2"]->GetPuzzleOn())
			{
				m_CameraEvents["ī�޶�_�̺�Ʈ2"]->SetCameraOn();
				if (!m_Puzzle2WallFade)
				{
					if (!m_PlaySoundOnce)
					{
						CSoundMgr::GetInstance()->PlaySound(L"barrier-up.ogg", CSoundMgr::EFFECT);
						m_PlaySoundOnce = true;
					}
					CGameObject* wall = m_PuzzlesObject["����2"]["����2_���ع�"];
					if (m_fAlpahValue2 <= 1.0f)
					{
						wall->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlpahValue2);
						for (auto&i : wall->GetChildernVector())
						{
							i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlpahValue2);
						}
						m_fAlpahValue2 += CTimeMgr::GetInstance()->GetDeltaTime()*0.7f;
					}
					else
					{
						m_Puzzle2WallFade = true;
						m_fAlpahValue2 = 1.0f;
						m_PlaySoundOnce = false;
					}
				}
				else
					m_mapPuzzle["����2"]["��������_2"]->SetPuzzleActive(true);
			}
			if (m_mapPuzzle["����2"]["��������_2"]->GetPuzzleOn())
			{
				cout << "����2 ������ Ŭ����" << endl;
				CGameObject* wall = m_PuzzlesObject["����2"]["����2_���ع�"];
				if (m_fAlpahValue2 >= 0.0f)
				{
					if (!m_PlaySoundOnce)
					{
						CSoundMgr::GetInstance()->PlaySound(L"�踮��ٿ�.ogg", CSoundMgr::EFFECT);
						m_PlaySoundOnce = true;
					}
					wall->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlpahValue2);
					for (auto&i : wall->GetChildernVector())
					{
						i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlpahValue2);
					}
					m_fAlpahValue2 -= CTimeMgr::GetInstance()->GetDeltaTime()*0.7f;
				}
				else
				{
					m_PlaySoundOnce = false;
					wall->SetObjectDestroy(true);
					m_Puzzle2Clear = true;
					m_fAlpahValue2 = 0.0f;
				}
			}
		}

		//����3=====================================================================================================

		if (!m_Puzzle3Clear)
		{
			if (m_mapPuzzle["����3"]["����3_����Ʈ2"]->GetPuzzleOn())
			{
				if (!m_Puzzle3WallFde)
				{
					if (!m_PlaySoundOnce)
					{
						CSoundMgr::GetInstance()->PlaySound(L"�踮��ٿ�.ogg", CSoundMgr::EFFECT);
						m_PlaySoundOnce = true;
					}
					if (m_fAlphaValue3 >= 0.0f)
					{
						m_PuzzlesObject["����3"]["����3_���ع�"]->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue3);
						for (auto&i : m_PuzzlesObject["����3"]["����3_���ع�"]->GetChildernVector())
						{
							i->GetComponent<CTextureRenderer>()->SetAlpha(m_fAlphaValue3);
						}
						m_fAlphaValue3 -= CTimeMgr::GetInstance()->GetDeltaTime()*0.7f;
					}
					else
					{
						m_PuzzlesObject["����3"]["����3_���ع�"]->SetObjectDestroy(true);
						m_Puzzle3WallFde = true;
						m_fAlphaValue3 = 1.0f;
						m_PlaySoundOnce = false;
					}
				}
			}
			if (m_mapPuzzle["����3"]["����3_����Ʈ"]->GetPuzzleOn() && m_mapPuzzle["����3"]["����3_����Ʈ2"]->GetPuzzleOn())
			{
				m_Puzzle3Clear = true;
			}
		}
		//=========================================================================================================
		if (!m_Puzzle4Clear)
		{
			
			if (m_mapPuzzle["����4"]["����4_����Ʈ"]->GetPuzzleOn())
			{
				if (!m_PlaySoundOnce)
				{
					CSoundMgr::GetInstance()->PlaySound(L"������.ogg", CSoundMgr::EFFECT);
					m_PlaySoundOnce = true;
				}
				m_PuzzlesObject["����4"]["����4_��"]->GetComponent<CAnimator>()->Play(L"Open", ANIMATION_ONCE);
				m_Puzzle4Clear = true;
				
			}
		}
	}
	else
	{
		m_PlaySoundOnce = false;
		CSceneMgr::GetInstance()->SceneChange(CSceneMgr::BOSS_STAGE);
	}
		
}

void CSTage1Event::CameraEventUpdate()
{
	if (!m_CamEvent1)
	{
		if (m_CameraEvents["ī�޶�_�̺�Ʈ1"]->GetOn())
		{
			//CObjectMgr::GetInstance()->SetGameStop(true);
			if (m_CameraEvents["ī�޶�_�̺�Ʈ1"]->GetEventEnd())
			{
				//CObjectMgr::GetInstance()->SetGameStop(false);
				m_CameraEvents["ī�޶�_�̺�Ʈ1"]->SetObjectDead();
				m_CamEvent1 = true;
			}

		}
	}
	
	if (!m_CamEvent2)
	{
		{
			//CObjectMgr::GetInstance()->SetGameStop(true);
			if (m_CameraEvents["ī�޶�_�̺�Ʈ2"]->GetEventEnd())
			{
				//CObjectMgr::GetInstance()->SetGameStop(false);
				m_CameraEvents["ī�޶�_�̺�Ʈ2"]->SetObjectDead();
				m_CamEvent2 = true;
			}

		}
	}

	if (!m_CamEvent3)
	{
		if (m_CameraEvents["ī�޶�_�̺�Ʈ4"]->GetEventEnd())
			{
				CObjectMgr::GetInstance()->SetGameStop(false);
				m_CameraEvents["ī�޶�_�̺�Ʈ4"]->SetObjectDead();
				m_CamEvent3 = true;
				m_bIsChangeScene = true;
				
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
