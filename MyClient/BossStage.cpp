#include "stdafx.h"
#include "BossStage.h"

#include "Camera.h"
#include "Mouse.h"
#include "GameObject.h"

#include "EnemyScripts.h"

CBossStage::CBossStage()
{
}


CBossStage::~CBossStage()
{
}

void CBossStage::Update()
{
	m_Cam->Update();
	m_Cam->SetTransform();
	m_Mouse->Update();

	m_pObjectMgr->Update();
}

void CBossStage::LateUpdate()
{
}

void CBossStage::Render()
{
	m_pObjectMgr->Render();
}

HRESULT CBossStage::Initialize()
{
	m_pObjectMgr->SetTileSize(2048, 2048);
	m_pObjectMgr->LoadObject(L"../Map/MapList/boss.map");

	m_Cam = new CCamera;

	CCameraMgr::GetInstance()->SetMainCamera(m_Cam);

	RECT rect;
	::GetClientRect(g_hWnd, &rect);
	float winX = rect.right - rect.left;
	float winY = rect.bottom - rect.top;

	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(2.5f, 2.5f, 1.0f));

	m_Mouse = new CMouse;
	m_Mouse->Initialize();

	m_pKeyMgr->SetMouse(m_Mouse);

	CGameObject* Boss=CObjectMgr::GetInstance()->FindObjectWithName(L"Boss_Bot").front();
	Boss->AddScripts(CEnemyScripts::Create(Boss, ENEMY_TYPE::BOSS));

	return S_OK;
}

void CBossStage::Release()
{
	delete m_Mouse;
	delete m_Cam;
}

CBossStage * CBossStage::Create()
{
	CBossStage* pInstance = new CBossStage;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
