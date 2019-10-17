#include "stdafx.h"
#include "DestructiveObject.h"

#include "GameObject.h"
#include "TextureRenderer.h"
#include "Transform.h"

CDestructiveObject::CDestructiveObject()
{
}


CDestructiveObject::~CDestructiveObject()
{
}

void CDestructiveObject::OnInit()
{
}

void CDestructiveObject::OnEnable()
{
}

void CDestructiveObject::OnCollision(CGameObject * pGameObject , XMFLOAT2 * move)
{
	if (pGameObject->GetObjectTag() == L"Bullet")
	{
		Hit();
	}
}

void CDestructiveObject::OnInput()
{
}

int CDestructiveObject::OnUpdate()
{
	return 0;
}

void CDestructiveObject::OnLateUpdate()
{
}

void CDestructiveObject::OnRender()
{
}

void CDestructiveObject::OnDisable()
{
}

void CDestructiveObject::OnDestroy()
{
}

void CDestructiveObject::Hit()
{
}
