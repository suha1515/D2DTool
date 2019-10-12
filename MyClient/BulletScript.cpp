#include "stdafx.h"
#include "BulletScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"

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
	else
	{
		CheckTiles();

		Move();

		if (m_fLifeTime <0.0f)
			m_pGameObject->SetObjectDestroy(true);
		m_fLifeTime -= CTimeMgr::GetInstance()->GetDeltaTime();


		m_NearTiles.clear();
	}
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
	D3DXVECTOR3& pos = pTransform->GetLocalPosition();
	m_PrePos = pos;
	// (*m_BulletPos) = m_DirVec*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	/*pos.x += m_DirVec.x*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	pos.y += m_DirVec.y*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();*/
	pos.x  += m_DirVec.x*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	pBoxCollider->SetBoxCollider();
	if (CollideTiles())
	{
		pos = m_PrePos;
		pBoxCollider->SetBoxCollider();
	}
	m_PrePos = pos;
	pos.y+= m_DirVec.y*m_fVelocity*CTimeMgr::GetInstance()->GetDeltaTime();
	pBoxCollider->SetBoxCollider();
	if (CollideTiles())
	{
		pos = m_PrePos;
		pBoxCollider->SetBoxCollider();
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
			if (pDestBox != nullptr)
			{
				//타일컬링되는 오브젝트들 확인. ( 충돌체 있는녀석만)
				i->SetObjectCliked(true, D3DCOLOR_XRGB(255, 0, 0));
				COLLIDE_TYPE coltype = pDestBox->GetCollideType();
				//사각형충돌과 삼각형충돌이 있다
				//사각형 충돌시
				if (coltype == NORMAL)
				{
					if (CCollisionMgr::GetInstance()->CheckAABB(pBoxCollider, pDestBox))
					{
						cout << "총알 사각형충돌" << endl;
						D3DXVECTOR3 Upvec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
						
						D3DXVECTOR3 normal = CCollisionMgr::GetInstance()->GetNormalBox(m_BulletPos,pDestBox);
						D3DXVECTOR3 reflect = GetReflectVector(&m_DirVec, &normal);
						SetDirection(reflect);
						return true;
					}
				}
				//삼각형 충돌시. 그외 삼각형 충돌
				else if (coltype != NORMAL)
				{
					if (CCollisionMgr::GetInstance()->CheckLineBox(pBoxCollider, pDestBox))
					{
						return true;
					}
				}
			}
		}
	}
	return false;

}

CBulletScript * CBulletScript::Create(const float & angle, const float & speed,CGameObject* pGameObject)
{
	CBulletScript* pBulletScript = new CBulletScript;
	pBulletScript->SetGameObject(pGameObject);
	pBulletScript->SetAngle(angle);
	pBulletScript->SetSpeed(speed);
	return pBulletScript;
}
