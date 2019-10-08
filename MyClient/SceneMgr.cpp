#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"


//¾ÀÁ¾·ù
#include "TestScene.h"


IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_pCurrentScene(nullptr), m_eCurSceneType(END), m_ePreSceneType(END),
	m_iEvent(NO_EVENT)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

HRESULT CSceneMgr::SceneChange(SCENE_TYPE eType)
{
	if (m_ePreSceneType != eType)
	{
		SafeDelete(m_pCurrentScene);

		switch (eType)
		{
		case TEST_SCENE:
			m_pCurrentScene = CTestScene::Create();
			NULL_CHECK_MSG_RETURN(m_pCurrentScene, L"TestScene Create Failed", E_FAIL);
			break;
		case STAGE:
			//m_pCurrentScene = CStage::Create();
			//NULL_CHECK_MSG_RETURN(m_pCurrentScene, L"Stage Create Failed", E_FAIL);
			break;
		}

		m_eCurSceneType = eType;
		m_ePreSceneType = m_eCurSceneType;
		m_iEvent = CHANGE_SCENE;
	}

	return S_OK;
}

void CSceneMgr::Update()
{
	m_iEvent = NO_EVENT;

	NULL_CHECK(m_pCurrentScene);
	m_pCurrentScene->Update();
}

void CSceneMgr::LateUpdate()
{
	if (CHANGE_SCENE == m_iEvent)
		return;

	NULL_CHECK(m_pCurrentScene);
	m_pCurrentScene->LateUpdate();
}

void CSceneMgr::Render()
{
	if (CHANGE_SCENE == m_iEvent)
		return;

	NULL_CHECK(m_pCurrentScene);
	m_pCurrentScene->Render();
}

void CSceneMgr::Release()
{
	SafeDelete(m_pCurrentScene);
}
