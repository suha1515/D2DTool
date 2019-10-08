#include "stdafx.h"
#include "PlayerScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animator.h"
#include "Camera.h"

CPlayerScript::CPlayerScript()
{
	m_ScriptName = "CPlayerScript";

}


CPlayerScript::~CPlayerScript()
{
}

//라이프 사이클.
void CPlayerScript::OnInit()
{
	cout << "이것도 루프도나?" << endl;
	m_CurDir = DOWN;
	m_PreDir = m_CurDir;
	m_CurState = IDLE;
	m_PreState = m_CurState;
	pKeyMgr = CKeyMgr::GetInstance();
	pAnimator = m_pGameObject->GetComponent<CAnimator>();
	pTransform = m_pGameObject->GetComponent<CTransform>();

	if (nullptr == pAnimator)
		MessageBox(0,L"애니메이터 없음", L"Error",0);

	CCameraMgr::GetInstance()->GetMainCamera()->Follow(m_pGameObject);

	m_fVelocity = 0.0f;
	m_fAcc = 0.0f;
	bIsInit = true;

	pAnimator->Play(L"Player_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);
}

void CPlayerScript::OnEnable()
{
}

void CPlayerScript::OnCollision(CGameObject * pGameObject, XMFLOAT2* move)
{
	if (nullptr != pGameObject)
	{
		//CTransform* pTransform = m_pGameObject->GetComponent<CTransform>();
		//wstring pname = pGameObject->GetObjectName();
		//wstring name = m_pGameObject->GetObjectName();

		//wstring text = name + L" 오브젝트는 " + pname + L" 오브젝트와 충돌중이다";
		////string text2;
		////text2.assign(text.begin(), text.end());
		//wcout << text << endl;
		//cout << "충돌중" << endl;
		////밀어내기
		//D3DXVECTOR3 playerPos = pTransform->GetPosition();
		//D3DXVECTOR3 destPos =   pGameObject->GetComponent<CTransform>()->GetPosition();
		////파고든 깊이가 짧은쪽으로 밀어내기
		//if (move->x > move->y)
		//{
		//	//y축 밀어내기		
		//	if (playerPos.y > destPos.y)
		//		pTransform->SetPosition(D3DXVECTOR3(playerPos.x,playerPos.y + move->y,0.0f));
		//	else
		//		pTransform->SetPosition(D3DXVECTOR3(playerPos.x, playerPos.y- move->y, 0.0f));
		//}
		//else
		//	//x축 밀어내기
		//{
		//	if (playerPos.x > destPos.x)
		//		pTransform->SetPosition(D3DXVECTOR3(playerPos.x+ move->x, playerPos.y, 0.0f));
		//	else
		//		pTransform->SetPosition(D3DXVECTOR3(playerPos.x- move->x, playerPos.y , 0.0f));
		//}
	}
}

void CPlayerScript::OnInput()
{
}

void CPlayerScript::OnUpdate()
{
	if (!bIsInit)
		OnInit();

	m_Right = false;
	m_Left = false;
	m_Down = false;
	m_Up = false;

	CTransform* pTransform = m_pGameObject->GetComponent<CTransform>();
	if (pTransform != nullptr)
	{
		//어떤 키든 눌렀을경우 (방향키)
		if (pKeyMgr->KeyPressing(KEY_LEFT) || pKeyMgr->KeyPressing(KEY_RIGHT)
			|| pKeyMgr->KeyPressing(KEY_UP) || pKeyMgr->KeyPressing(KEY_DOWN))
		{
			//cout << "키눌렀다" << endl;
			m_fVelocity += (GRAVITY*powf(m_fAcc, 2.0f));
			m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime()*10.f;

			m_fAcc = __min(2.0f, m_fAcc);
			m_fVelocity = __min(7.f, m_fVelocity);
			if(m_CurState==IDLE||m_CurState==RUN_END)
				m_CurState = RUN_START;
			else if (m_CurState == RUN_START && !pAnimator->IsPlaying())
				m_CurState = RUN;
		}
		//방향 키를 놨경우 (방향키) 모두!
		else if (!pKeyMgr->KeyPressing(KEY_LEFT) && !pKeyMgr->KeyPressing(KEY_RIGHT)
			&& !pKeyMgr->KeyPressing(KEY_UP) && !pKeyMgr->KeyPressing(KEY_DOWN))
		{
			if (m_fVelocity <= 0.0f)
				m_fVelocity = __max(0.f, m_fVelocity);
			else
			{
				m_fVelocity -= (GRAVITY*powf(m_fAcc, 2.0f));
				m_fAcc -= CTimeMgr::GetInstance()->GetDeltaTime()*10.f;

				m_fAcc = __max(0.0f, m_fAcc);
				m_fVelocity = __max(0.f, m_fVelocity);
			}
			if (m_CurState ==RUN|| m_CurState==RUN_START)
				m_CurState = RUN_END;
			else if (m_CurState == RUN_END && !pAnimator->IsPlaying())
				m_CurState = IDLE;
		}
		if (pKeyMgr->KeyPressing(KEY_LEFT))
		{
			//cout << "왼쪽 이동!" << endl;
			D3DXVECTOR3& playerPos = pTransform->GetPosition();
			playerPos.x -= 0.5f*m_fVelocity;
			m_Left = true;
		}
		else if (pKeyMgr->KeyPressing(KEY_RIGHT))
		{
			D3DXVECTOR3& playerPos = pTransform->GetPosition();
			playerPos.x += 0.5f*m_fVelocity;
			m_Right = true;
		}
		if (pKeyMgr->KeyPressing(KEY_UP))
		{
			D3DXVECTOR3& playerPos = pTransform->GetPosition();
			playerPos.y += 0.5f*m_fVelocity;
			m_Up = true;
		}
		else if (pKeyMgr->KeyPressing(KEY_DOWN))
		{
			D3DXVECTOR3& playerPos = pTransform->GetPosition();
			playerPos.y -= 0.5f*m_fVelocity;
			m_Down  = true;
		}
		//방향 변화.
		DirState();
		//애니메이션 상태변화
		AnimState();

		

		/*switch (m_CurState)
		{
		case IDLE:
			cout << "대기" << endl;
			break;
		case RUN_START:
			cout << "뛰기 시작" << endl;
			break;
		case RUN_END:
			cout << "뛰기 끝" << endl;
			break;
		case RUN:
			cout << "뛰 기" << endl;
			break;
		}*/

	}
}

void CPlayerScript::OnLateUpdate()
{
}

void CPlayerScript::OnRender()
{
}

void CPlayerScript::OnDisable()
{
}

void CPlayerScript::OnDestroy()
{
}

void CPlayerScript::AnimState()
{
	if (m_CurState != m_PreState||m_CurDir!=m_PreDir)
	{
		switch (m_CurState)
		{
		case IDLE:
			cout << "대기상태" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_Idle_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				pAnimator->Play(L"Player_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;

			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_Idle_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_Idle_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_Idle_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case RUN_START:
			cout << "뛰기 준비" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_ReadyRun_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				pAnimator->Play(L"Player_ReadyRun_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_ReadyRun_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_ReadyRun_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_ReadyRun_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case RUN:
			cout << "뛰기" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_Run_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				pAnimator->Play(L"Player_Run_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_Run_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_Run_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_Run_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case  RUN_END:
			cout << "뛰기 종료" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_RunStop_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				pAnimator->Play(L"Player_RunStop_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_RunStop_RightUp", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_RunStop_Righ", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_RunStop_RightDown", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		}
		m_PreState = m_CurState;
		m_PreDir = m_CurDir;
	}


}

void CPlayerScript::DirState()
{
	if (m_Right)
	{
		m_CurDir = RIGHT;
		pTransform->SetScaling(D3DXVECTOR3(1.0f,1.0f,1.0f));
	}
	else if (m_Left)
	{
		m_CurDir = LEFT;
		pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
		

	if (m_Up)
		m_CurDir = UP;
	else if (m_Down)
		m_CurDir = DOWN;

	if (m_Right&&m_Up)
		m_CurDir = RIGHT_UP_45;
	else if (m_Right&&m_Down)
		m_CurDir = RIGHT_DOWN_45;
	else if (m_Left&&m_Down)
		m_CurDir = LEFT_DOWN_45;
	else if (m_Left&&m_Up)
		m_CurDir = LEFT_UP_45;

	if (m_CurDir != m_PreDir)
	{
		switch (m_CurDir)
		{
		case UP:
			cout << "윗방향" << endl;
			break;
		case DOWN:
			cout << "아랫방향" << endl;
			break;
		case LEFT_UP_45:
			cout << "왼쪽위" << endl;
			break;
		case RIGHT_UP_45:
			cout << "오른위" << endl;
			break;
		case LEFT:
			cout << "왼쪽" << endl;
			break;
		case RIGHT:
			cout << "오른쪽" << endl;
			break;
		case LEFT_DOWN_45:
			cout << "왼쪽아래" << endl;
			break;
		case RIGHT_DOWN_45:
			cout << "오른아래." << endl;
			break;
		}
		
	}


}

