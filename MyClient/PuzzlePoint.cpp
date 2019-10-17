#include "stdafx.h"
#include "PuzzlePoint.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "Effect.h"

CPuzzlePoint::CPuzzlePoint()
{
}


CPuzzlePoint::~CPuzzlePoint()
{
}

void CPuzzlePoint::Action()
{
}

void CPuzzlePoint::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();

	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Puzzle_Point");
		m_pAnimator->Play(L"Puzzle_Idle", ANIMATION_LOOP);
	}

	m_CurState = IDLE;
	m_PreState = m_CurState;

	m_PuzzleOn = false;
}

void CPuzzlePoint::OnEnable()
{
	m_ScriptName = "PuzzlePoint";
}

void CPuzzlePoint::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move )
{
	if (m_bIsInit&&m_CurState!=CLEAR)
	 {
		if (pGameObject->GetObjectTag() == L"Bullet")
		{
			if (pGameObject->GetObjectLayer() == m_pGameObject->GetObjectLayer())
			{
				pGameObject->SetObjectDestroy(true);
				m_CurState = HIT;

				D3DXVECTOR3* pos = m_pTransform->GetWorldPos();
				XMFLOAT3& rot = XMFLOAT3(0, 0, 0.0f);
				D3DXVECTOR3 scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				CEffect::Create(*pos, rot, scale, L"Bullet_Effect", L"Bullet_Hit", ANIMATION_ONCE);
			}
		}
	}
}

void CPuzzlePoint::OnInput()
{
}

int CPuzzlePoint::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}

	if (m_bPuzzleActive)
	{
		AnimState();
		if (m_CurState == HIT)
		{
			{
				if (m_fTime > 0.2f)
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
						m_fTime -= m_fTime;
						m_CurState = CLEAR;
					}
				}
				m_fTime += CTimeMgr::GetInstance()->GetDeltaTime();
			}
		}

	}	
	return 0;
}

void CPuzzlePoint::OnLateUpdate()
{
}

void CPuzzlePoint::OnRender()
{
}

void CPuzzlePoint::OnDisable()
{
}

void CPuzzlePoint::OnDestroy()
{
}

void CPuzzlePoint::AnimState()
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
			case CLEAR:
				m_PuzzleOn = true;
				m_bPuzzleActive = false;
			break;
		}
		m_PreState = m_CurState;
	}
}
