#include "stdafx.h"
#include "EnemyScripts.h"
#include "MouseBotScript.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "GameObject.h"

CEnemyScripts::CEnemyScripts()
{
	m_fAcc = 0.0f;
	m_fVelocity = 0.0f;
	m_pTransform = nullptr;
	m_pAnimator = nullptr;

	m_ScriptName = "CEnemyScripts";
}


CEnemyScripts::~CEnemyScripts()
{
}

void CEnemyScripts::CheckTiles()
{
	const vector<CGameObject*>& tiles = CObjectMgr::GetInstance()->GetTiles();

	int mapSizex = CObjectMgr::GetInstance()->m_MapSizeX;
	int mapSizey = CObjectMgr::GetInstance()->m_MapSizeY;

	D3DXVECTOR3 pos = *m_pTransform->GetWorldPos();
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

bool CEnemyScripts::CollideTiles()
{
	for (auto& i : m_NearTiles)
	{
		if (i != nullptr)
		{
			CBoxCollider* pDestBox = i->GetComponent<CBoxCollider>();
			D3DXVECTOR3	  destPos = *i->GetComponent<CTransform>()->GetWorldPos();
			if (pDestBox != nullptr && (m_pGameObject->GetObjectLayer() <= i->GetObjectLayer() || i->GetObjectLayer() == LAYER_GROUND))
			{
				//타일컬링되는 오브젝트들 확인. ( 충돌체 있는녀석만)
				i->SetObjectCliked(true, D3DCOLOR_XRGB(255, 0, 0));
				COLLIDE_TYPE coltype = pDestBox->GetCollideType();
				//사각형충돌과 삼각형충돌이 있다
				//사각형 충돌시
				D3DXVECTOR3 normal;
				if (coltype == NORMAL)
				{
					if (CCollisionMgr::GetInstance()->CheckAABB(m_pBoxCollider, pDestBox))
					{
						return true;
					}
				}
				//삼각형 충돌시. 그외 삼각형 충돌
				else if (coltype != NORMAL)
				{
					if (CCollisionMgr::GetInstance()->CheckLineBox(m_pBoxCollider, pDestBox, &normal))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

CEnemyScripts * CEnemyScripts::Create(CGameObject * pGameObject,ENEMY_TYPE type)
{
	CEnemyScripts* enemyScripts=nullptr;
	switch (type)
	{
	case MOUSE:
		enemyScripts = new CMouseBotScript;
		enemyScripts->SetGameObject(pGameObject);
		enemyScripts->m_Type = type;
		break;
	case BOT:
		break;
	case TURRET:
		break;
	}
	
	return enemyScripts;
}
