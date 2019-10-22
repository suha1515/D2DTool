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
	m_pObjectMgr->Update();
	m_EventChecker->Update();
}

void CTestScene::LateUpdate()
{
	m_Cam->LateUpdate();
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

	m_Cam->Initialize(winX,winY, 0, D3DXVECTOR3(2.0f, 2.0f, 1.0f));

	m_Mouse = new CMouse;
	m_Mouse->Initialize();

	m_pKeyMgr->SetMouse(m_Mouse);

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
	CSoundMgr::GetInstance()->SetVolume(CSoundMgr::BGM, 0.4f);
	CSoundMgr::GetInstance()->PlayBGM(L"1½ºÅ×ÀÌÁöÀ½¾Ç.ogg");
	
	
	return S_OK;
}

void CTestScene::Release()
{
	m_pObjectMgr->Clear();
	CInstanceMgr::GetInstance()->Clear();
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
