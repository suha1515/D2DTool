		#include "stdafx.h"
#include "TestScene.h"

#include "Camera.h"
#include "Mouse.h"
#include "GameObject.h"
#include "MouseBotScript.h"
#include "Animator.h"
#include "PuzzleScripts.h"

CTestScene::CTestScene()
{
}


CTestScene::~CTestScene()
{
	Release();
}

void CTestScene::Update()
{
	m_Cam->Update();
	m_Cam->SetTransform();
	m_Mouse->Update();
	m_pObjectMgr->Update();

}

void CTestScene::LateUpdate()
{
}

void CTestScene::Render()
{
	m_pObjectMgr->Render();
}

HRESULT CTestScene::Initialize()
{
	m_pObjectMgr->SetTileSize(2048, 2048);
	m_pObjectMgr->LoadObject(L"../Map/MapList/good2.map");
	
	m_Cam = new CCamera;

	CCameraMgr::GetInstance()->SetMainCamera(m_Cam);
	
	RECT rect;
	::GetClientRect(g_hWnd,&rect);
	float winX = rect.right - rect.left;
	float winY = rect.bottom - rect.top;

	m_Cam->Initialize(winX,winY, 0, XMFLOAT3(2.0f, 2.0f, 1.0f));

	m_Mouse = new CMouse;
	m_Mouse->Initialize();

	m_pKeyMgr->SetMouse(m_Mouse);

	CGameObject* pBullet= m_pObjectMgr->AddCopy(L"CrossHair", L"my_Bullet");
	//pBullet->AddScripts(CMouseBotScript::Create(pBullet)
	if (pBullet == nullptr)
		MessageBox(0, L"총알 널포인트입니다", L"ERROR", 0);

	vector<CGameObject*> MouseBotobjects = CObjectMgr::GetInstance()->FindObjectWithName(L"쥐로봇");
	for (auto&i : MouseBotobjects)
	{
		i->AddScripts(CEnemyScripts::Create(i,ENEMY_TYPE::MOUSE));
	}

	//퍼즐 요소 찾기
	CGameObject* puzzle1 = CObjectMgr::GetInstance()->FindObjectWithName(L"퍼즐포인트_1").front();
	m_Puzzles = CPuzzleScripts::Create(puzzle1);
	puzzle1->AddScripts(m_Puzzles);
	for (auto&i : puzzle1->GetChildernVector())
	{
		CAnimator *pScripts = CAnimator::Create(i, L"Clear_Idle", L"Clear_Way");
		i->AddComponent(pScripts);
		m_Puzzles->SetPuzzleWay(i);
	}
	
	CGameObject* puzzlePad = CObjectMgr::GetInstance()->FindObjectWithName(L"퍼즐패드_1").front();
	m_Puzzles->SetOnObject(puzzlePad);
	CGameObject* puzzleWall = CObjectMgr::GetInstance()->FindObjectWithName(L"방어벽_1").front();
	m_Puzzles->SetWallObject(puzzleWall);


	
	return S_OK;
}

void CTestScene::Release()
{
	delete m_Mouse;
	delete m_Cam;
}

CTestScene * CTestScene::Create()
{
	CTestScene* pInstance = new CTestScene;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
