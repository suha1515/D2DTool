#include "stdafx.h"
#include "BossIceEffect.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "BoxCollider.h"
#include "Effect.h"
#include "PlayerScript.h"


CBossIceEffect::CBossIceEffect()
{
	m_fRadius = 50.f;
	m_fThrowTime = 0.0f;
	m_bDead = false;
	m_ParticleDone = false;
}


CBossIceEffect::~CBossIceEffect()
{
}

void CBossIceEffect::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Ice_Effect");
		m_pAnimator->SetClips(L"Ice");
		m_pAnimator->Play(L"Ice", ANIMATION_ONCE);
		m_fDeadTime = 3.0f;
	}
	m_pTransform->SetScaling(D3DXVECTOR3(0.7f, 0.7f, 1.0f));
	m_pBoxCollider->SetBoxSize(20, 20);
	m_pBoxCollider->SetBoxCollider();
	m_pTexture->SetPass(1);
	m_pTexture->SetFadeColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_bIsInit = true;
	m_WhiteValue = 0.5f;

	
	m_pPlayer = CObjectMgr::GetInstance()->m_pPlayer;
	CEffect::Create(*m_pTransform->GetWorldPos(), XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Ice_Effect", L"Ice_Explosive", ANIMATION_ONCE);
}

void CBossIceEffect::OnEnable()
{
}

void CBossIceEffect::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move )
{
	if (pGameObject == m_pPlayer)
	{
		CPlayerScript* script = dynamic_cast<CPlayerScript*>(pGameObject->GetScript("CPlayerScript"));
		if (!script->m_bIsHit)
		{
			D3DXVECTOR3 effectPos = *m_pTransform->GetWorldPos();
			D3DXVECTOR3 playerPos = *m_pPlayer->GetComponent<CTransform>()->GetWorldPos();
			float m_fAngle = GetAngle(effectPos, playerPos);
			//360도로 변환하기 위한것
			if (m_fAngle < 0.0f)
				m_fAngle = m_fAngle + 360.f;
			D3DXVECTOR3 dir = D3DXVECTOR3(cosf(D3DXToRadian(m_fAngle)), sinf(D3DXToRadian(m_fAngle)), 0.0f);
			D3DXVec3Normalize(&dir, &dir);
			script->GetHit(dir, 10.f, 20.f);
		}
	}
}

void CBossIceEffect::OnInput()
{
}

int CBossIceEffect::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}
	if (m_bDead)
	{
		m_pGameObject->SetObjectDestroy(true);
	}
	if (!m_bParticle)
	{
		if (m_fTime > m_fDeadTime)
		{
			m_bParticle = true;
			Destruct();
			m_WhiteValue = 0.0f;
		}
		m_fTime += CTimeMgr::GetInstance()->GetDeltaTime();
	}
	else
	{
		if (m_WhiteValue <0.5f)
		{
			m_pTexture->SetValue(m_WhiteValue * 2);
			
			m_WhiteValue += CTimeMgr::GetInstance()->GetDeltaTime();
		}
		else
		{
			if (!m_ParticleDone)
			{
				m_pTexture->SetOn(false);
				m_pBoxCollider->SetOn(false);
				Particle();
			}
		}
			
	}
	if (!m_Spawn)
	{
		if (m_WhiteValue >0.0f)
		{
			m_pTexture->SetValue(m_WhiteValue*2);
			float ratio = 0.1f - 0.1f*m_WhiteValue*2;
			m_pTransform->SetScaling(D3DXVECTOR3(0.9f+ ratio, 0.9f+ ratio, 1.0f));
			m_WhiteValue -= CTimeMgr::GetInstance()->GetDeltaTime();
		}
		else
			m_Spawn = true;
	}
	
	return 0;
}

void CBossIceEffect::OnLateUpdate()
{
}

void CBossIceEffect::OnRender()
{
}

void CBossIceEffect::OnDisable()
{
}

void CBossIceEffect::OnDestroy()
{
}

void CBossIceEffect::Destruct()
{
	for (int i = 0; i < 5; ++i)
	{
		random_device		rn;
		mt19937_64 rnd(rn());

		uniform_real_distribution<float> nHeight(70.f, 100.f);

		uniform_real_distribution<float> nDir(-1.0f, 1.0f);


		D3DXVECTOR3 dir(nDir(rnd), nDir(rnd), 0.f);

		D3DXVec3Normalize(&dir, &dir);
		D3DXVECTOR3 pos = *m_pTransform->GetWorldPos();
		D3DXVECTOR3 randompos = dir*m_fRadius + *m_pTransform->GetWorldPos();
		D3DXVECTOR3 jumpControl;

		if (randompos.x > pos.x)
			jumpControl = pos + (randompos - pos) / 2;
		else
			jumpControl = randompos + (pos - randompos) / 2;
		float randHeight = nHeight(rnd);
		jumpControl.y += randHeight;

		D3DXVECTOR3 newPos = D3DXVECTOR3(pos.x + 20.f, pos.y + 20.f, 0.0f);
		CGameObject* particle = CEffect::Create(pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), L"Particle_Block", L"Rolling", ANIMATION_LOOP, 1.5f,0,0,0,0,L"Effect",LAYER_0);
		m_fragment.push_back(tuple<CGameObject*, D3DXVECTOR3, D3DXVECTOR3>(particle, randompos, jumpControl));
	}
}

void CBossIceEffect::Particle()
{
	if (m_fThrowTime > 1.0f)
	{
		m_bDead = true;
		m_ParticleDone = true;
		m_fThrowTime -= m_fThrowTime;
	}
	else
	{
		for (int i = 0; i < 5; ++i)
		{
			CGameObject* pGameObject = get<0>(m_fragment[i]);
			D3DXVECTOR3 randPos = get<1>(m_fragment[i]);
			D3DXVECTOR3 jumpPos = get<2>(m_fragment[i]);
			D3DXVECTOR3 pos = *m_pTransform->GetWorldPos();
			D3DXVECTOR3 newPos = BezierCurve(pos, randPos, jumpPos, (m_fThrowTime));

			pGameObject->GetComponent<CTransform>()->SetPosition(newPos);
		}
		m_fThrowTime += CTimeMgr::GetInstance()->GetDeltaTime();
	}
}
