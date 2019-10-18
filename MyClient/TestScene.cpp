#include "stdafx.h"
#include "TestScene.h"

#include "Camera.h"
#include "Mouse.h"
#include "GameObject.h"
#include "MouseBotScript.h"
#include "STage1Event.h"


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
	m_EventChecker->Update();
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
	m_pObjectMgr->LoadObject(L"../Map/MapList/good.map");
	
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
		MessageBox(0, L"ÃÑ¾Ë ³ÎÆ÷ÀÎÆ®ÀÔ´Ï´Ù", L"ERROR", 0);

	vector<CGameObject*> MouseBotobjects = CObjectMgr::GetInstance()->FindObjectWithName(L"Áã·Îº¿");
	for (auto&i : MouseBotobjects)
	{
		i->AddScripts(CEnemyScripts::Create(i,ENEMY_TYPE::MOUSE));
	}
	CGameObject* enemy = CObjectMgr::GetInstance()->FindObjectWithName(L"Åé´Ï·Îº¿").front();
	enemy->AddScripts(CEnemyScripts::Create(enemy, ENEMY_TYPE::BOT));

	enemy = CObjectMgr::GetInstance()->FindObjectWithName(L"ÅÍ·¿").front();
	enemy->AddScripts(CEnemyScripts::Create(enemy, ENEMY_TYPE::TURRET));


	m_EventChecker = new CSTage1Event;
	m_EventChecker->Initialize();
	
	return S_OK;
}

void CTestScene::Release()
{
	delete m_Mouse;
	delete m_Cam;
	delete m_EventChecker;
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
