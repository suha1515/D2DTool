#include "stdafx.h"
#include "MainGame.h"


CMainGame::CMainGame()
	:m_pDeviceMgr(CDeviceMgr::GetInstance())
	, m_pTextureMgr(CTextureMgr::GetInstance())
	, m_pTimeMgr(CTimeMgr::GetInstance())
	, m_pFrameMgr(CFrameMgr::GetInstance())
	, m_pObjectMgr(CObjectMgr::GetInstance())
	, m_pAnimationMgr(CAnimationMgr::GetInstance())
	, m_pCameraMgr(CCameraMgr::GetInstance())
	, m_pScriptMgr(CScriptMgr::GetInstance())
	, m_pInstanceMgr(CInstanceMgr::GetInstance())
	, m_pCollisionMgr(CCollisionMgr::GetInstance())
	, m_pSceneMgr(CSceneMgr::GetInstance())
	, m_pShaderMgr(CShaderMgr::GetInstance())
	, m_pKeyMgr(CKeyMgr::GetInstance())
{

}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Update()
{
	m_pTimeMgr->UpdateTime();
	m_pKeyMgr->Update();
	m_pSceneMgr->Update();
}

void CMainGame::LateUpdate()
{
	m_pSceneMgr->LateUpdate();
}

void CMainGame::Render()
{
	m_pDeviceMgr->Render_Begin();


	m_pSceneMgr->Render();
	m_pFrameMgr->RenderFPS();


	m_pDeviceMgr->Render_End();
}

void CMainGame::RenderLine()
{
}

HRESULT CMainGame::Initialize()
{
	HRESULT hr = 0;

	hr = m_pTimeMgr->InitTime();
	FAILED_CHECK_MSG_RETURN(hr, L"InitTime 실패", E_FAIL);

	hr = m_pDeviceMgr->InitDevice(CDeviceMgr::MODE_WIN);
	FAILED_CHECK_MSG_RETURN(hr, L"장치초기화 실패", E_FAIL);

	m_pShaderMgr->Initialize();

	m_pObjectMgr->LoadCopyObjectFromFile(L"../ObjectList/copy2.dat");

	m_pTextureMgr->LoadTextureFromList(L"../Texture/map/22.dat");
	CInstanceMgr::GetInstance()->Initialize();

	m_pAnimationMgr->LoadAnimationFromList(L"../Animation/AnimationList/AnimationList.txt");

	hr = m_pSceneMgr->SceneChange(CSceneMgr::BOSS_STAGE);
	FAILED_CHECK_MSG_RETURN(hr, L"SceneChange 실패", E_FAIL);

	

	return S_OK;
}

void CMainGame::Release()
{
	m_pSceneMgr->DestroyInstance();
	m_pFrameMgr->DestroyInstance();
	m_pTimeMgr->DestroyInstance();
	m_pTextureMgr->DestroyInstance();
	m_pInstanceMgr->DestroyInstance();
	m_pShaderMgr->DestroyInstance();
	m_pObjectMgr->DestroyInstance();
	m_pAnimationMgr->DestroyInstance();

	m_pDeviceMgr->DestroyInstance();
}

CMainGame * CMainGame::Create()
{
	CMainGame* pInstance = new CMainGame;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
