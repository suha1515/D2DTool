#include "stdafx.h"
#include "DestructiveObject.h"

#include "GameObject.h"
#include "TextureRenderer.h"
#include "Transform.h"
#include "BoxCollider.h"

#include "Effect.h"

CDestructiveObject::CDestructiveObject()
{
	m_bIsHit = false;
	m_bIsActive = true;
	m_iHitCount = 5;
	m_bIsGone = false;
	m_Radius = 50.f;
}


CDestructiveObject::~CDestructiveObject()
{
}

void CDestructiveObject::OnInit()
{
	m_pTexture = m_pGameObject->GetComponent<CTextureRenderer>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_pTexture->SetPass(1);
}

void CDestructiveObject::OnEnable()
{
}

void CDestructiveObject::OnCollision(CGameObject * pGameObject , XMFLOAT2 * move)
{
	if (pGameObject->GetObjectTag() == L"Bullet")
	{
		m_bIsHit = true;
		m_fWhiteValue = 0.0f;
		m_iHitCount--;
	}
}

void CDestructiveObject::OnInput()
{
}

int CDestructiveObject::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}

	
	if (m_bIsActive)
	{
		if (m_bIsHit)
		{
			Hit();
		}
		if (!m_bIsGone)
		{
			if (m_iHitCount < 0)
			{
				CEffect::Create(*m_pTransform->GetWorldPos(), XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.f, 2.f, 1.0f), L"Explosion_Effect", L"Small_Explosion", ANIMATION_ONCE);
				m_bIsGone = true;
				m_iHitCount = 5;
				m_pTexture->SetOn(false);
				Destruct();
			}
		}
		else
		{
			Throw();
		}

	}
	
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
	if (m_fWhiteValue < 0.5f)
	{
		m_pTexture->SetValue(1.0f-m_fWhiteValue*2.f);
		m_fWhiteValue += CTimeMgr::GetInstance()->GetDeltaTime();
	}
	else
	{
		m_fWhiteValue -= m_fWhiteValue;
		m_bIsHit = false;
	}
		
}

void CDestructiveObject::Destruct()
{
	for (int i = 0; i < 5; ++i)
	{
		random_device		rn;
		mt19937_64 rnd(rn());

		uniform_real_distribution<float> nHeight(70.f, 100.f);

		uniform_real_distribution<float> nDir(-1.0f, 1.0f);


		D3DXVECTOR3 dir(nDir(rnd), nDir(rnd),0.f);

		D3DXVec3Normalize(&dir, &dir);
		D3DXVECTOR3 pos = *m_pTransform->GetWorldPos();
		D3DXVECTOR3 randompos = dir*m_Radius+ *m_pTransform->GetWorldPos();
		D3DXVECTOR3 jumpControl;
		
		if(randompos.x > pos.x)
			jumpControl = pos+(randompos - pos) / 2;
		else
			jumpControl = randompos+(pos - randompos) / 2;
		float randHeight = nHeight(rnd);
		jumpControl.y += randHeight;
		
		D3DXVECTOR3 newPos = D3DXVECTOR3(pos.x + 20.f, pos.y + 20.f, 0.0f);
		CGameObject* particle = CEffect::Create(pos, XMFLOAT3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f,1.0f,1.0f), L"Particle_Block", L"Rolling", ANIMATION_LOOP,1.0f);
		m_fragment.push_back(tuple<CGameObject*, D3DXVECTOR3,D3DXVECTOR3>( particle,randompos, jumpControl));
	}
}

void CDestructiveObject::Throw()
{
	if (m_fThrowTime > 1.0f)
	{
		m_fThrowTime -= m_fThrowTime;
		m_bIsActive = false;
		m_pBoxCollider->SetOn(false);
	}
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
