#include "stdafx.h"
#include "PuzzleStack.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "Effect.h"

CPuzzleStack::CPuzzleStack()
{
	m_ScriptName = "PuzzleStack";
}


CPuzzleStack::~CPuzzleStack()
{
}

void CPuzzleStack::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();

	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Puzzle_bar");
		m_pAnimator->Play(L"Puzzle_Stack0", ANIMATION_ONCE);
	}

	m_CurState = STACK_0;
	m_PreState = m_CurState;

	m_PuzzleOn = false;

	m_fStackDownTime = 0.0f;
}

void CPuzzleStack::OnEnable()
{
}

void CPuzzleStack::OnCollision(CGameObject * pGameObject , XMFLOAT2 * move )
{
	if (m_bIsInit && m_CurState!=FULL_STACK)
	{
		if (pGameObject->GetObjectTag() == L"Bullet")
		{
			pGameObject->SetObjectDestroy(true);
			int curStack = m_CurState;
			m_CurState = (STATE)(curStack+1);
			D3DXVECTOR3* pos = m_pTransform->GetWorldPos();
			XMFLOAT3& rot = XMFLOAT3(0, 0, 0.0f);
			D3DXVECTOR3 scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			CEffect::Create(*pos, rot, scale, L"Bullet_Effect", L"Bullet_Hit", ANIMATION_ONCE);
			CSoundMgr::GetInstance()->PlaySound(L"스위치카운터.ogg", CSoundMgr::EFFECT);
		}
	}
}

void CPuzzleStack::OnInput()
{
}

int CPuzzleStack::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}
	if (m_bPuzzleActive)
	{
		AnimState();
		if (m_CurState != STACK_0&&m_CurState != FULL_STACK&&m_CurState != CLEAR)
		{
			if (m_fStackDownTime > 1.0f)
			{
				int curStack = m_CurState;
				m_CurState = (STATE)(curStack - 1);
				m_fStackDownTime -= m_fStackDownTime;
			}
			m_fStackDownTime += CTimeMgr::GetInstance()->GetDeltaTime();
		}

		if (m_CurState == FULL_STACK)
		{
			{
				if (m_fTime > 0.3f)
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

void CPuzzleStack::OnLateUpdate()
{
}

void CPuzzleStack::OnRender()
{
}

void CPuzzleStack::OnDisable()
{
}

void CPuzzleStack::OnDestroy()
{
}

void CPuzzleStack::Action()
{
}

void CPuzzleStack::AnimState()
{
	if (m_PreState != m_CurState)
	{
		switch (m_CurState)
		{
		case STACK_0:
			m_pAnimator->Play(L"Puzzle_Stack0", ANIMATION_ONCE);
			break;
		case STACK_1:
			m_pAnimator->Play(L"Puzzle_Stack1", ANIMATION_ONCE);
			break;
		case STACK_2:
			m_pAnimator->Play(L"Puzzle_Stack2", ANIMATION_ONCE);
			break;
		case STACK_3:
			m_pAnimator->Play(L"Puzzle_Stack3", ANIMATION_ONCE);
			break;
		case STACK_4:
			m_pAnimator->Play(L"Puzzle_Stack4", ANIMATION_ONCE);
			break;
		case FULL_STACK:
			m_pAnimator->Play(L"Puzzle_Clear", ANIMATION_ONCE);
			CSoundMgr::GetInstance()->PlaySound(L"스위치온.ogg", CSoundMgr::EFFECT);
			break;
		case CLEAR:
			m_PuzzleOn = true;
			m_bPuzzleActive = false;
			break;
		}
		m_PreState = m_CurState;
	}
}
