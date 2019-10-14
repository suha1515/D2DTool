#pragma once
class CBoxCollider;
class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	//static bool CheckRectX(CBoxCollider* pSource, CBoxCollider* pDest);
	static bool CheckRect(CBoxCollider* pSource, CBoxCollider* pDest, float* pMoveX,float* pMoveY);
	//static bool CheckRectY()
	static bool CheckAABB(CBoxCollider* pSource, CBoxCollider* pDest);
	static bool CheckLineBox(CBoxCollider* pSource, CBoxCollider* pDest, D3DXVECTOR3* normal=nullptr);
	static bool CheckLineBox2(LINE* pSource, CBoxCollider* pDest, D3DXVECTOR3* points=nullptr, D3DXVECTOR3* normal = nullptr);
	static D3DXVECTOR3 GetNormalBox(const D3DXVECTOR3* pSrcPos,CBoxCollider* pDestBox);
	
};

