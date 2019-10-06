#include "stdafx.h"
#include "PlayerScript.h"
#include "GameObject.h"
#include "Transform.h"


CPlayerScript::CPlayerScript()
{
	m_ScriptName = "PlayerScript";
}


CPlayerScript::~CPlayerScript()
{
}

//라이프 사이클.
void CPlayerScript::OnInit()
{
}

void CPlayerScript::OnEnable()
{
}

void CPlayerScript::OnCollision(CGameObject * pGameObject)
{
}

void CPlayerScript::OnInput()
{
}

void CPlayerScript::OnUpdate()
{
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

