#include "stdafx.h"
#include "PlayerIceSkill.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "BoxCollider.h"
#include "Effect.h"

CPlayerIceSkill::CPlayerIceSkill()
{
}


CPlayerIceSkill::~CPlayerIceSkill()
{
}

void CPlayerIceSkill::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Ice_Effect");
		m_pAnimator->SetClips(L"Ice_Fragment3");
		m_pAnimator->Play(L"Ice_Fragment3", ANIMATION_ONCE);
		m_fDeadTime = 3.0f;
	}
	m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	//m_pBoxCollider->SetBoxSize(30, 30);
	//m_pBoxCollider->SetBoxCollider();

	m_pPlayer = CObjectMgr::GetInstance()->m_pPlayer;
	m_pTexture->SetOn(false);
	m_fSpawnTime = 0.0f;
	m_fSpawnTimeEnd = 0.f;

	playerPos = m_pPlayer->GetComponent<CTransform>()->GetWorldPos();
	effectPos = &m_pTransform->GetLocalPosition();
	m_pTarget = CObjectMgr::GetInstance()->FindObjectWithName(L"Boss_Bot").front();
	if (m_pTarget != nullptr)
	{
		m_pBossTarget = m_pTarget->GetComponent<CTransform>()->GetWorldPos();
	}
   m_fAngle = GetAngle(*effectPos, *m_pBossTarget);

   //360도로 변환하기 위한것
   if (m_fAngle < 0.0f)
	   m_fAngle = m_fAngle + 360.f;
   else
	   m_fAngle = m_fAngle;

   m_fRadius = 30.f;

   m_CurState = SPREAD;
   m_PreState = m_CurState;

   m_fSpeed = 200.f;
}

void CPlayerIceSkill::OnEnable()
{
}

void CPlayerIceSkill::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move )
{
}

void CPlayerIceSkill::OnInput()
{
}

int CPlayerIceSkill::OnUpdate()
{
	if (!m_bIsInit)
	{
		MakeBullet();
		OnInit();
		m_bIsInit = true;
	}
	State();
	//m_DirVec = D3DXVECTOR3(cosf(D3DXToRadian(m_fAngle)), sinf(D3DXToRadian(m_fAngle)), 0.0f);
	//D3DXVec3Normalize(&m_DirVec, &m_DirVec);
	//if (m_fRotateTime < 1.0f)
	//{
	//	float angle = LerpAngle(0.0f, m_fAngle, m_fRotateTime);
	//	m_DirVec = D3DXVECTOR3(cosf(D3DXToRadian(angle)), sinf(D3DXToRadian(angle)), 0.0f);
	//	D3DXVec3Normalize(&m_DirVec, &m_DirVec);
	//	m_pTransform->SetRotation(XMFLOAT3(0.0f, 0.0f, angle -90.f));
	//	m_fRotateTime += CTimeMgr::GetInstance()->GetDeltaTime();

	

	//for (int i = 0; i < 10; ++i)
	//{
	//	m_DirVec = D3DXVECTOR3(cosf(D3DXToRadian(bullet[i].second)), sinf(D3DXToRadian(bullet[i].second)), 0.0f);
	//	D3DXVec3Normalize(&m_DirVec, &m_DirVec);
	//	D3DXVECTOR3* pos =&bullet[i].first->GetComponent<CTransform>()->GetLocalPosition();
	//	pos->x += m_DirVec.x*20.f*CTimeMgr::GetInstance()->GetDeltaTime();
	//	pos->y += m_DirVec.y*20.f*CTimeMgr::GetInstance()->GetDeltaTime();
	//}
	//

	//effectPos->x += m_DirVec.x*20.f*CTimeMgr::GetInstance()->GetDeltaTime();
	//effectPos->y += m_DirVec.y*20.f*CTimeMgr::GetInstance()->GetDeltaTime();

	//m_pTransform->SetPosition(*effectPos);
	if (m_fSpawnTime > 8.0f)
		m_pGameObject->SetObjectDestroy(true);
	else
		m_fSpawnTime += CTimeMgr::GetInstance()->GetDeltaTime();

	return 0;
}

void CPlayerIceSkill::OnLateUpdate()
{
}

void CPlayerIceSkill::OnRender()
{
}

void CPlayerIceSkill::OnDisable()
{
}

void CPlayerIceSkill::OnDestroy()
{
}

void CPlayerIceSkill::State()
{
	switch (m_CurState)
	{
	case SPREAD:
		m_fSpeed -= (20.0f*powf(m_fAcc, 2.0f));
		m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime()*2.0f;
		m_fSpeed = __max(0.f, m_fSpeed);
		for (int i = 0; i < 10; ++i)
		{
			bullet[i].first->GetComponent<CTextureRenderer>()->SetPass(1);
			bullet[i].first->GetComponent<CTextureRenderer>()->SetFadeColor(XMFLOAT3(1.0f,1.0f,1.0f));
			bullet[i].first->GetComponent<CTextureRenderer>()->SetValue(m_fSpeed/200.f);
			m_DirVec = D3DXVECTOR3(cosf(D3DXToRadian(bullet[i].second)), sinf(D3DXToRadian(bullet[i].second)), 0.0f);
			D3DXVec3Normalize(&m_DirVec, &m_DirVec);
			D3DXVECTOR3* pos = &bullet[i].first->GetComponent<CTransform>()->GetLocalPosition();
			pos->x += m_DirVec.x*m_fSpeed*CTimeMgr::GetInstance()->GetDeltaTime();
			pos->y += m_DirVec.y*m_fSpeed*CTimeMgr::GetInstance()->GetDeltaTime();
			
		}
			for (int i = 0; i < 10; ++i)
			{
				m_fAngle = GetAngle(*bullet[i].first->GetComponent<CTransform>()->GetWorldPos(), *m_pBossTarget);
				float angle = bullet[i].second;
				bullet[i].second = LerpAngle(angle, m_fAngle, 1-m_fSpeed / 200.f);
				bullet[i].first->GetComponent<CTransform>()->SetRotation(XMFLOAT3(0.0f, 0.0f, bullet[i].second - 90.f));
			}

		if (m_fSpeed == 0.0f)
			m_CurState = SHOOT;
		break;
	case AIM:

		if (m_fRotateTime < 1.0f)
		{
			for (int i = 0; i < 10; ++i)
			{
				m_fAngle = GetAngle(*bullet[i].first->GetComponent<CTransform>()->GetWorldPos(), *m_pBossTarget);
				float angle = bullet[i].second;
				bullet[i].second = LerpAngle(angle, m_fAngle, m_fRotateTime);
				bullet[i].first->GetComponent<CTransform>()->SetRotation(XMFLOAT3(0.0f, 0.0f, bullet[i].second - 90.f));
			}
			m_fRotateTime += CTimeMgr::GetInstance()->GetDeltaTime();
		}
		else
		{
			m_fRotateTime -= m_fRotateTime;
			m_CurState = SHOOT;
		}
				
		break;
	case SHOOT:
		m_fSpeed += (20.f*powf(m_fAcc, 2.0f));
		m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime()*2.0f;
		m_fSpeed = __min(600.f, m_fSpeed);
		for (int i = 0; i < 10; ++i)
		{
			m_DirVec = D3DXVECTOR3(cosf(D3DXToRadian(bullet[i].second)), sinf(D3DXToRadian(bullet[i].second)), 0.0f);
			D3DXVec3Normalize(&m_DirVec, &m_DirVec);
			D3DXVECTOR3* pos = &bullet[i].first->GetComponent<CTransform>()->GetLocalPosition();
			pos->x += m_DirVec.x*m_fSpeed*CTimeMgr::GetInstance()->GetDeltaTime();
			pos->y += m_DirVec.y*m_fSpeed*CTimeMgr::GetInstance()->GetDeltaTime();
		}
		break;
	}
}

void CPlayerIceSkill::SetTarget()
{
	
}

void CPlayerIceSkill::MakeBullet()
{

	random_device	rn;
	mt19937_64 rnd(rn());
	uniform_real_distribution<float> nRandAngle(0.0f,360.f);
	uniform_real_distribution<float> nDir(-1.0f, 1.f);
	uniform_real_distribution<float> nRadius(0.0f, 30.f);
				
	for (int i = 0; i < 10; ++i)
	{
		D3DXVECTOR3 dir(nDir(rnd), nDir(rnd), 0.f);

		D3DXVec3Normalize(&dir, &dir);
		float fRadius = nRadius(rnd);
		D3DXVECTOR3 pos = dir*fRadius + (*effectPos);

		float ranAngle = nRandAngle(rnd);
		CGameObject* pbullet = CEffect::Create(pos, XMFLOAT3(0.0f, 0.0f, ranAngle - 90.f), D3DXVECTOR3(1.0f, 1.0f, 0.0f), L"Ice_Effect", L"Ice_Fragment3", ANIMATION_LOOP, 10.0f, 10, 10, 0, 0, L"Player_Ice",LAYER_5);
		
		bullet.push_back(pair<CGameObject*,float>(pbullet, ranAngle));
	}
}
