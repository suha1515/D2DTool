#include "stdafx.h"
#include "PuzzleScripts.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"

#include "Effect.h"
CPuzzleScripts::CPuzzleScripts()
{
}


CPuzzleScripts::~CPuzzleScripts()
{
}

void CPuzzleScripts::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_fTime = 0.5f;

	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Puzzle_Point");
		m_pAnimator->Play(L"Puzzle_Idle", ANIMATION_LOOP);
	}

	m_CurState = IDLE;
	m_PreState = m_CurState;

	m_PuzzleOn = false;
	m_PuzzleOpen = false;
	m_PuzzleLight = false;
}

		

void CPuzzleScripts::OnEnable()
{
}

void CPuzzleScripts::OnCollision(CGameObject * pGameObject , XMFLOAT2 * move)
{
	if (m_bIsInit&&!m_PuzzleOn)
	{
		if (pGameObject->GetObjectTag() == L"Bullet")
		{
			pGameObject->SetObjectDestroy(true);
			m_CurState = HIT;
			m_PuzzleOn = true;
			D3DXVECTOR3* pos = m_pTransform->GetWorldPos();
			XMFLOAT3& rot = XMFLOAT3(0, 0, 0.0f);
			D3DXVECTOR3 scale = D3DXVECTOR3(1.0f,1.0f,1.0f);
			CEffect::Create(*pos, rot, scale, L"Bullet_Effect", L"Bullet_Hit", ANIMATION_ONCE);
		}
	}
}

void CPuzzleScripts::OnInput()
{
}

int CPuzzleScripts::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}

	if (m_CurState == HIT && !m_pAnimator->IsPlaying())
		m_CurState = WAY;

	AnimState();

	if (m_PuzzleOn)
	{
			if(m_fTime > 0.3f)
			{
				if (!m_pPuzzleWay.empty())
				{
					CGameObject* pGameObject = m_pPuzzleWay.front();
					m_pPuzzleWay.pop_front();
					pGameObject->GetComponent<CAnimator>()->Play(L"Clear_On", ANIMATION_ONCE);
					m_fTime -= m_fTime;
				}
				else
				{
					m_CurState = LIGHT;
					m_fTime -= m_fTime;
				}
			}
			m_fTime += CTimeMgr::GetInstance()->GetDeltaTime();	
	}

	if (m_PuzzleLight)
	{
		
		m_CurState = OPEN;
	}
	return 0;
}

void CPuzzleScripts::OnLateUpdate()
{
}

void CPuzzleScripts::OnRender()
{
}

void CPuzzleScripts::OnDisable()
{
}

void CPuzzleScripts::OnDestroy()
{
}

void CPuzzleScripts::AnimState()
{
	if (m_PreState != m_CurState)
	{
		switch (m_CurState)
		{
		case IDLE:
			m_pAnimator->Play(L"Puzzle_Idle", ANIMATION_LOOP);
			break;
		case HIT:
			m_pAnimator->Play(L"Puzzle_Hit", ANIMATION_ONCE);
			break;
		case WAY:
			m_PuzzleOn = true;
			break;
		case LIGHT:
			m_PuzzleOn = false;
			m_pPuzzlePad->GetComponent<CAnimator>()->Play(L"Pad_On", ANIMATION_LOOP);
			m_PuzzleLight = true;
			break;
		case OPEN:
			for (auto&i : m_pPuzzleWall)
				i->SetObjectDestroy(true);
			break;
		}
	}
	
}

void CPuzzleScripts::SetOnObject(CGameObject * pGameObject)
{
	m_pPuzzlePad = pGameObject;
}

void CPuzzleScripts::SetWallObject(CGameObject * pGameObject)
{
	m_pPuzzleWall.push_back(pGameObject);
	for (auto&i : pGameObject->GetChildernVector())
		m_pPuzzleWall.push_back(i);
}

void CPuzzleScripts::SetPuzzleWay(CGameObject * pGameObject)
{
	m_pPuzzleWay.push_back(pGameObject);
}

bool CPuzzleScripts::GetPuzzleOn()
{
	return m_PuzzleOn;
}

CPuzzleScripts * CPuzzleScripts::Create(CGameObject * pGameObject)
{
	CPuzzleScripts* pScript = new CPuzzleScripts;

	pScript->SetGameObject(pGameObject);

	return pScript;
}
