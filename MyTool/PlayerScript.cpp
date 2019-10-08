#include "stdafx.h"
#include "PlayerScript.h"
#include "GameObject.h"
#include "Transform.h"


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
	m_Dir = UP;
	m_CurState = IDLE;
	m_PreState = m_CurState;
}

void CPlayerScript::OnEnable()
{
}

void CPlayerScript::OnCollision(CGameObject * pGameObject,XMFLOAT2* move)
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
	CTransform* pTransform = m_pGameObject->GetComponent<CTransform>();
	if (pTransform != nullptr)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			D3DXVECTOR3& playerPos = pTransform->GetPosition();
			playerPos.x -= 0.5f;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			D3DXVECTOR3& playerPos = pTransform->GetPosition();
			playerPos.x += 0.5f;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			D3DXVECTOR3& playerPos = pTransform->GetPosition();
			playerPos.y += 0.5f;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			D3DXVECTOR3& playerPos = pTransform->GetPosition();
			playerPos.y -= 0.5f;
		}
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
	if (m_CurState != m_PreState)
	{
		switch (m_CurState)
		{
		case IDLE:

			break;
		case RUN_START:
			break;
		case RUN_END:
			break;
		}
		m_PreState = m_CurState;
	}
}

void CPlayerScript::DirState()
{
	switch (m_Dir)
	{
	case UP:
		break;
	case RIGHT_UP_45:
		break;
	case RIGHT:
		break;
	case RIGHT_DOWN_45:
		break;
	case DOWN:
		break;
	case LEFT_UP_45:
		break;
	case LEFT:
		break;
	case LEFT_DOWN_45:
		break;
	}
}

