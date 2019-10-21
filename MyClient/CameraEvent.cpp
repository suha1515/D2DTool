#include "stdafx.h"
#include "CameraEvent.h"

#include "GameObject.h"
#include "Camera.h"

CCameraEvent::CCameraEvent()
{
}


CCameraEvent::~CCameraEvent()
{
}

void CCameraEvent::OnInit()
{
	m_iIndexRoute = 0;
	m_pCameraMgr = CCameraMgr::GetInstance();
	m_RestTime = 0.0f;
}

void CCameraEvent::OnEnable()
{
}

void CCameraEvent::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move)
{
	if (pGameObject->GetObjectTag() == L"Player")
	{
		m_bIsEventOn = true;
	}
}

void CCameraEvent::OnInput()
{
}

int CCameraEvent::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}


	if (m_bIsEventOn)
	{
		if (!m_CamRoute.empty())
		{
			if (m_iIndexRoute < m_CamRoute.size())
			{
				if (m_RestTime < m_CamRoute[m_iIndexRoute].second)
				{
					m_pCameraMgr->GetMainCamera()->Follow(m_CamRoute[m_iIndexRoute].first);
					m_RestTime += CTimeMgr::GetInstance()->GetDeltaTime();
				}
				else
				{
					m_iIndexRoute++;
					m_RestTime -= m_RestTime;
				}
			}
			else
			{
				m_bIsEventOn = false;
				m_bIsEventEnd = true;
				m_pCameraMgr->GetMainCamera()->Follow(CObjectMgr::GetInstance()->m_pPlayer);
			}
		}
		else
			m_bIsEventEnd = true;
	}
	return 0;
}

void CCameraEvent::OnLateUpdate()
{
}

void CCameraEvent::OnRender()
{
}

void CCameraEvent::OnDisable()
{
}

void CCameraEvent::OnDestroy()
{
}

void CCameraEvent::GetCameraRoute(CGameObject * pos, float _restTIme)
{
	m_CamRoute.push_back(pair<CGameObject*, float>(pos, _restTIme));
}

void CCameraEvent::GetCameraRoute(vector<pair<CGameObject*, float>> camRoute)
{
	m_CamRoute = camRoute;
}

void CCameraEvent::MoveCamera()
{
}

bool CCameraEvent::GetOn()
{
	return m_bIsEventOn;
}

bool CCameraEvent::GetEventEnd()
{
	return m_bIsEventEnd;
}

void CCameraEvent::SetObjectDead()
{
	m_pGameObject->SetObjectDestroy(true);
}

void CCameraEvent::CutCamera()
{
	m_bIsEventOn = false;
	m_bIsEventEnd = true;
	m_pCameraMgr->GetMainCamera()->Follow(CObjectMgr::GetInstance()->m_pPlayer);
}

void CCameraEvent::SetCameraOn()
{
	m_bIsEventOn = true;
}

CCameraEvent * CCameraEvent::Create(vector<pair<CGameObject*, float>> camRoute)
{
	CCameraEvent* m_CamEvent = new CCameraEvent;
	m_CamEvent->GetCameraRoute(camRoute);
	return m_CamEvent;
}
