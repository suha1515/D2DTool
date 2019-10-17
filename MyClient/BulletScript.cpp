#include "stdafx.h"
#include "BulletScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Effect.h"
#include "Scripts.h"
#include "EnemyScripts.h"
#include "PlayerScript.h"

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
	pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	m_fLifeTime = 3.0f;


	m_BulletPos = &pTransform->GetLocalPosition();
	if (m_BulletType == CHARGED)
		pAnimator->Play(L"Basic_Big_Bullet", ANIMATION_LOOP);
	else if (m_BulletType == TURRET)
		pAnimator->Play(L"Turrect_Bullet", ANIMATION_ONCE);
	else if(m_BulletType == TURRET_CHARGE)
		pAnimator->Play(L"Turrect_Big_Bullet", ANIMATION_LOOP);

	m_bIsInit = true;
}

void CBulletScript::OnEnable()
{
}

void CBulletScript::OnCollision(CGameObject * pGameObject, XMFLOAT2 * move)
{
	if (m_bIsInit)
	{
		if ((m_BulletType == SMALL || m_BulletType == CHARGED) && pGameObject->GetObjectTag() == L"Enemy")
		{
			if (pGameObject->GetObjectLayer() == m_pGameObject->GetObjectLayer())
			{
				cout << "적을때렸다" << endl;
				D3DXVECTOR3* pos = pTransform->GetWorldPos();
				XMFLOAT3& rot = XMFLOAT3(0, 0, 0.0f);
				D3DXVECTOR3 scale;
				float power = 0.0f;
				if (m_BulletType == CHARGED)
				{
					scale = D3DXVECTOR3(2.0f, 2.0f, 1.0f);
					power = 40.f;
				}
				else
				{
					scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					power = 20.f;
				}

					CEffect::Create(*pos, rot, scale, L"Bullet_Effect", L"Bullet_Hit", ANIMATION_ONCE);
					CScripts* pscripts = pGameObject->GetScript("CEnemyScripts");
					if (pscripts != nullptr)
						dynamic_cast<CEnemyScripts*>(pscripts)->GetHit(m_DirVec, power, 0.0f);

				m_pGameObject->SetObjectDestroy(true);
			}
		}
		else if ((m_BulletType == TURRET || m_BulletType == TURRET_CHARGE) && pGameObject->GetObjectTag() == L"Player")
		{
			if (pGameObject->GetObjectLayer() == m_pGameObject->GetObjectLayer())
			{
				cout << "플레이어를때렸다" << endl;
				D3DXVECTOR3* pos = pTransform->GetWorldPos();
				XMFLOAT3& rot = XMFLOAT3(0, 0, 0.0f);
				D3DXVECTOR3 scale;
				float power = 0.0f;
				if (m_BulletType == TURRET_CHARGE)
				{
					scale = D3DXVECTOR3(2.0f, 2.0f, 1.0f);
					power = 10.f;
					CEffect::Create(*pos, rot, scale, L"Turret_Effect", L"Turrect_Big_Bullet_Hit", ANIMATION_ONCE);
				}
				else
				{
					scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
					power = 5.f;
					CEffect::Create(*pos, rot, scale, L"Turret_Effect", L"Turrect_Bullet_Hit", ANIMATION_ONCE);
				}

				CScripts* pscripts = pGameObject->GetScript("CPlayerScript");
				if (pscripts != nullptr)
					dynamic_cast<CPlayerScript*>(pscripts)->GetHit(m_DirVec, power, 0.0f);


				m_pGameObject->SetObjectDestroy(true);
			}
		}
	}
}

void CBulletScript::OnInput()
{
}

int CBulletScript::OnUpdate()
{
	if (!m_bIsInit)
		OnInit();
	else
	{
		CheckTiles();

		Move();

		if (m_fLifeTime < 0.0f)
		{
			D3DXVECTOR3* pos = pTransform->GetWorldPos();
			XMFLOAT3& rot = XMFLOAT3(0, 0, 0.0f);
			D3DXVECTOR3 scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			CEffect::Create(*pos, rot, scale, L"Bullet_Effect", L"Bullet_Fragile", ANIMATION_ONCE);
			m_pGameObject->SetObjectDestroy(true);
		}
			
		m_fLifeTime -= CTimeMgr::GetInstance()->GetDeltaTime();


		m_NearTiles.clear();
	}
	return NO_EVENT;
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

void CBulletScript::Move()
{

	m_PrePos = (*m_BulletPos);
	for (int i = 0; i < 5; ++i)
	{
		(*m_BulletPos).x += m_DirVec.x*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime()/5.0f;
		pBoxCollider->SetBoxCollider();
		if (CollideTiles())
		{
			(*m_BulletPos) = m_PrePos;
			pBoxCollider->SetBoxCollider();
		}
	}
	m_PrePos = (*m_BulletPos);
	for (int i = 0; i < 5; ++i)
	{
		(*m_BulletPos).y += m_DirVec.y*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime()/5.0f;
		pBoxCollider->SetBoxCollider();
		if (CollideTiles())
		{
			(*m_BulletPos) = m_PrePos;
			pBoxCollider->SetBoxCollider();
		}
	}

}

void CBulletScript::SetAngle(const float & angle)
{
	m_fAngle = angle;
	m_DirVec.x = cosf(D3DXToRadian(m_fAngle));
	m_DirVec.y = sinf(D3DXToRadian(m_fAngle));
	
	D3DXVec3Normalize(&m_DirVec, &m_DirVec);
}

void CBulletScript::SetDirection(const D3DXVECTOR3& dir)
{
	m_DirVec = dir;
}

void CBulletScript::SetSpeed(const float & speed)
{
	m_fVelocity = speed;
}

void CBulletScript::SetBulletType(BULLET_TYPE type)
{
	m_BulletType = type;
}

void CBulletScript::SetBulletDmg(const float & dmg)
{
	m_Damage = dmg;
}

void CBulletScript::CheckTiles()
{
	const vector<CGameObject*>& tiles = CObjectMgr::GetInstance()->GetTiles();

	int mapSizex = CObjectMgr::GetInstance()->m_MapSizeX;
	int mapSizey = CObjectMgr::GetInstance()->m_MapSizeY;

	D3DXVECTOR3 pos = *pTransform->GetWorldPos();
	int indexX = (((mapSizex / 2) + pos.x) / 16);
	int indexY = (((mapSizey / 2) - pos.y) / 16);
	int index = indexX + indexY*mapSizex;

	m_NearTiles.push_back(tiles[(indexX - 1) + (indexY - 1)*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + (indexY - 1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX + 1) + (indexY - 1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX - 1) + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX + 1) + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX - 1) + (indexY + 1)*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + (indexY + 1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX + 1) + (indexY + 1)*mapSizex]);
}

bool CBulletScript::CollideTiles()
{
	for (auto& i : m_NearTiles)
	{
		if (i != nullptr)
		{
			CBoxCollider* pDestBox= i->GetComponent<CBoxCollider>();
			D3DXVECTOR3	  destPos = *i->GetComponent<CTransform>()->GetWorldPos();
			if (pDestBox != nullptr&&(m_pGameObject->GetObjectLayer() <= i->GetObjectLayer() || i->GetObjectLayer() == LAYER_GROUND))
			{
				//타일컬링되는 오브젝트들 확인. ( 충돌체 있는녀석만)
				i->SetObjectCliked(true, D3DCOLOR_XRGB(255, 0, 0));
				COLLIDE_TYPE coltype = pDestBox->GetCollideType();
				//사각형충돌과 삼각형충돌이 있다
				//사각형 충돌시
				D3DXVECTOR3 normal;
				if (coltype == NORMAL)
				{
					if (CCollisionMgr::GetInstance()->CheckAABB(pBoxCollider, pDestBox))
					{
						if (m_BulletType == CHARGED || m_BulletType == TURRET_CHARGE)
						{
							normal = CCollisionMgr::GetInstance()->GetNormalBox(m_BulletPos, pDestBox);
							D3DXVECTOR3 reflect = GetReflectVector(&m_DirVec, &normal);
							SetDirection(reflect);
							D3DXVECTOR3* pos = pTransform->GetWorldPos();
							normal = CCollisionMgr::GetInstance()->GetNormalBox(m_BulletPos, pDestBox);
							float angle;
							if (normal == D3DXVECTOR3(-1.0f, 0.0f, 0.0f))
								angle = -90.f;
							else if (normal == D3DXVECTOR3(1.0f, 0.0f, 0.0f))
								angle = 90.f;
							else if (normal == D3DXVECTOR3(0.0f, -1.0f, 0.0f))
								angle = 0.f;
							else if (normal == D3DXVECTOR3(0.0f, 1.0f, 0.0f))
								angle = 180.f;
							XMFLOAT3& rot = XMFLOAT3(0, 0, angle);
							D3DXVECTOR3 scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
							if (m_BulletType == TURRET_CHARGE)
								CEffect::Create(*pos, rot, scale, L"Turret_Effect", L"Turrect_Big_Bullet_Bounce", ANIMATION_ONCE);
							else
							CEffect::Create(*pos, rot, scale, L"Bullet_Effect", L"Bullet_Bounce", ANIMATION_ONCE);
						}
						else
						{
							m_pGameObject->SetObjectDestroy(true);
							D3DXVECTOR3* pos = pTransform->GetWorldPos();
							normal = CCollisionMgr::GetInstance()->GetNormalBox(m_BulletPos, pDestBox);
							float angle;
							if (normal == D3DXVECTOR3(-1.0f, 0.0f, 0.0f))
								angle = -90.f;
							else if (normal == D3DXVECTOR3(1.0f, 0.0f, 0.0f))
								angle = 90.f;
							else if (normal == D3DXVECTOR3(0.0f, -1.0f, 0.0f))
								angle = 0.f;
							else if (normal == D3DXVECTOR3(0.0f, 1.0f, 0.0f))
								angle = 180.f;
							XMFLOAT3& rot = XMFLOAT3(0, 0, angle);
							D3DXVECTOR3 scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
							if (m_BulletType == TURRET)
								CEffect::Create(*pos, rot, scale, L"Turret_Effect", L"Turrect_Bullet_Hit", ANIMATION_ONCE);
							else
							CEffect::Create(*pos, rot, scale, L"Bullet_Effect", L"Bullet_Fragile", ANIMATION_ONCE);
						}
							
						return true;
					}
				}
				//삼각형 충돌시. 그외 삼각형 충돌
				else if (coltype != NORMAL)
				{
					if (CCollisionMgr::GetInstance()->CheckLineBox(pBoxCollider, pDestBox,&normal))
					{
						if (m_BulletType == CHARGED||m_BulletType==TURRET_CHARGE)
						{
							D3DXVECTOR3 reflect = GetReflectVector(&m_DirVec, &normal);
							SetDirection(reflect);
							float angle = GetAngle(m_DirVec, normal);
							if (angle < 0.0f)
								angle = angle + 360.f;
							D3DXVECTOR3* pos = pTransform->GetWorldPos();
							XMFLOAT3& rot = XMFLOAT3(0, 0, angle);
							D3DXVECTOR3 scale = D3DXVECTOR3(1.4f, 1.4f, 1.0f);
							if (m_BulletType == TURRET_CHARGE)		
								CEffect::Create(*pos, rot, scale, L"Turret_Effect", L"Turrect_Big_Bullet_Bounce", ANIMATION_ONCE);		
							else
							CEffect::Create(*pos, rot, scale, L"Bullet_Effect", L"Bullet_Bounce", ANIMATION_ONCE);
						}
						else
						{
							m_pGameObject->SetObjectDestroy(true);
							float angle = GetAngle(m_DirVec , normal);
							if (angle < 0.0f)
								angle = angle + 360.f;
							D3DXVECTOR3* pos = pTransform->GetWorldPos();
							XMFLOAT3& rot =XMFLOAT3(0,0, angle+90.f);
							D3DXVECTOR3 scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
							if (m_BulletType == TURRET)
							CEffect::Create(*pos, rot, scale, L"Turret_Effect", L"Turrect_Bullet_Hit", ANIMATION_ONCE);
							else
							CEffect::Create(*pos, rot, scale, L"Bullet_Effect", L"Bullet_Fragile", ANIMATION_ONCE);
						}
							

						return true;
					}
				}
			}
		}
	}
	return false;

}

bool CBulletScript::CollideTilesLine()
{
	m_PrePos = (*m_BulletPos);
	LINE l1;
	l1.endPoint = m_PrePos;

	return false;
}

CBulletScript * CBulletScript::Create(const float & angle,const float& damage, const float & speed,CGameObject* pGameObject,BULLET_TYPE type)
{
	CBulletScript* pBulletScript = new CBulletScript;
	pBulletScript->SetBulletDmg(damage);
	pBulletScript->SetGameObject(pGameObject);
	pBulletScript->SetAngle(angle);
	pBulletScript->SetSpeed(speed);
	pBulletScript->SetBulletType(type);
	return pBulletScript;
}
