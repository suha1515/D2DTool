#include "stdafx.h"
#include "BulletScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"

CBulletScript::CBulletScript()
{
	m_ScriptName = "BulletScript";
}


CBulletScript::~CBulletScript()
{
}

void CBulletScript::OnInit()
{
	pAnimator = m_pGameObject->GetComponent<CAnimator>();
	pTransform = m_pGameObject->GetComponent<CTransform>();

	m_fLifeTime = 3.0f;

	if(pAnimator!=nullptr)
	pAnimator->Play(L"Basic_Big_Bullet", ANIMATION_LOOP);

	bIsInit = true;
}

void CBulletScript::OnEnable()
{
}

void CBulletScript::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move)
{
}

void CBulletScript::OnInput()
{
}

void CBulletScript::OnUpdate()
{
	if (!bIsInit)
		OnInit();

	D3DXVECTOR3& bulletPos = pTransform->GetPosition();
	bulletPos.x += cosf(D3DXToRadian(m_fAngle))*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	bulletPos.y += sinf(D3DXToRadian(m_fAngle))*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	

	if (m_fLifeTime <0.0f)
		m_pGameObject->SetObjectDestroy(true);
	m_fLifeTime-= CTimeMgr::GetInstance()->GetDeltaTime();
}

void CBulletScript::OnLateUpdate()
{
}

void CBulletScript::OnRender()
{
}

void CBulletScript::OnDisable()
{
}

void CBulletScript::OnDestroy()
{
}

void CBulletScript::SetAngle(const float & angle)
{
	m_fAngle = angle;
}

void CBulletScript::SetSpeed(const float & speed)
{
	m_fVelocity = speed;
}

CBulletScript * CBulletScript::Create(const float & angle, const float & speed,CGameObject* pGameObject)
{
	CBulletScript* pBulletScript = new CBulletScript;
	pBulletScript->SetGameObject(pGameObject);
	pBulletScript->SetAngle(angle);
	pBulletScript->SetSpeed(speed);
	return pBulletScript;
}
