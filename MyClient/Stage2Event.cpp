#include "stdafx.h"
#include "Stage2Event.h"

#include "Animator.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "Camera.h"
#include "BoxCollider.h"

#include "GameObject.h"
#include "BossScript.h"
#include "PlayerScript.h"
#include "CameraEvent.h"


CStage2Event::CStage2Event()
{
}


CStage2Event::~CStage2Event()
{
}

void CStage2Event::Initialize()
{
	CGameObject* camera_Event1 = CObjectMgr::GetInstance()->FindObjectWithName(L"이벤트_1").front();
	vector<pair<CGameObject*, float>> temp;
	CCameraEvent* Event = CCameraEvent::Create(temp);
	camera_Event1->AddScripts(Event);
	Event->SetGameObject(camera_Event1);
	m_CameraEvents = Event;

	m_pBoss = CObjectMgr::GetInstance()->FindObjectWithName(L"Boss_Bot").front();
	m_pPlayer= CObjectMgr::GetInstance()->FindObjectWithName(L"플레이어").front();

	
}

void CStage2Event::Update()
{
	if (!m_bCameraEvent)
	{
		if (m_CameraEvents->GetEventEnd())
		{
			cout << "플레이어밞음" << endl;
			m_bEvent2 = true;
			m_bCameraEvent = true;
			CCameraMgr::GetInstance()->Follow(m_pBoss);
		}
		
	}

	if (m_bEvent2)
	{
		if (m_fTime > 2.0f)
		{
			dynamic_cast<CBossScript*>(m_pBoss->GetScript("CBossScript"))->SetPhaseIn();
			m_bEvent2 = false;
		}
		else
			m_fTime += CTimeMgr::GetInstance()->GetDeltaTime();
	}

}
