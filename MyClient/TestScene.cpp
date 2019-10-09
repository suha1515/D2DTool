#include "stdafx.h"
#include "TestScene.h"

#include "Camera.h"
#include "Mouse.h"

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
	m_pObjectMgr->LoadObject(L"../Map/Map2.map");
	
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

	CGameObject* pBullet= m_pObjectMgr->AddCopy(L"CrossHair_Normal", L"my_Bullet");
	if (pBullet == nullptr)
		MessageBox(0, L"총알 널포인트입니다", L"ERROR", 0);

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
