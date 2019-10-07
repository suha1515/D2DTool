#include "stdafx.h"
#include "CollisionMgr.h"

#include "BoxCollider.h"

IMPLEMENT_SINGLETON(CCollisionMgr)
CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

bool CCollisionMgr::CheckRect(CBoxCollider * pSource, CBoxCollider* pDest, float * pMoveX, float * pMoveY)
{
	const D3DXVECTOR3*  srcBox = pSource->GetBox();
	const D3DXVECTOR3*	destBox = pDest->GetBox();

	//�� �簢�� X �� ���� ��������
	float fRadiusX = (destBox[2].x - destBox[0].x)*0.5f + (srcBox[2].x - srcBox[0].x)*0.5f;

	//�� �簢���� x�� ���밪
	float fDistX = fabsf(pDest->GetBoxPosition()->x - pSource->GetBoxPosition()->x);

	//�� �簢���� y�� ������ ��
	float fRadiusY = fabsf(destBox[2].y - destBox[3].y)*0.5f + (srcBox[2].y - srcBox[3].y)*0.5f;

	//�� �簢���� Y�� �Ÿ�
	float fDistY = fabsf(pDest->GetBoxPosition()->y - pSource->GetBoxPosition()->y);

	if (fRadiusX >= fDistX&& fRadiusY >= fDistY)
	{
		//�İ�� ���� x,y ����
		*pMoveX = fRadiusX - fDistX;
		*pMoveY = fRadiusY - fDistY;
		return true;
	}
	return false;

	return false;
}

bool CCollisionMgr::CheckAABB(CBoxCollider * pSource, CBoxCollider * pDest)
{

	float fminsrcX, fmindstX;
	float fmaxsrcX, fmaxdstX;
	float fminsrcY, fmindstY;
	float fmaxsrcY, fmaxdstY;

	const D3DXVECTOR3* srcBox  = pSource->GetBox();
	const D3DXVECTOR3* destBox = pDest->GetBox();


	fmaxsrcX = __max(srcBox[1].x, __max(srcBox[0].x, __max(srcBox[3].x, srcBox[2].x)));
	fminsrcX = __min(srcBox[1].x, __min(srcBox[0].x, __min(srcBox[3].x, srcBox[2].x)));

	fmaxsrcY = __max(srcBox[1].y, __max(srcBox[0].y, __max(srcBox[3].y, srcBox[2].y)));
	fminsrcY = __min(srcBox[1].y, __min(srcBox[0].y, __min(srcBox[3].y, srcBox[2].y)));

	fmaxdstX = __max(destBox[1].x, __max(destBox[0].x, __max(destBox[3].x, destBox[2].x)));
	fmindstX = __min(destBox[1].x, __min(destBox[0].x, __min(destBox[3].x, destBox[2].x)));

	fmaxdstY = __max(destBox[1].y, __max(destBox[0].y, __max(destBox[3].y, destBox[2].y)));
	fmindstY = __min(destBox[1].y, __min(destBox[0].y, __min(destBox[3].y, destBox[2].y)));

	//x�� �˻�
	if (fmaxsrcX < fmindstX || fminsrcX > fmaxdstX)
		return false;

	//y�� �˻�
	if (fmaxsrcY < fmindstY || fminsrcY > fmaxdstY)
		return false;

	return true;
	return false;
}
