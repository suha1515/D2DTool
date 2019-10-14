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
	float fRadiusX = fabsf((destBox[2].x - destBox[0].x)*0.5f + (srcBox[2].x - srcBox[0].x)*0.5f);

	//�� �簢���� x�� ���밪
	float fDistX = fabsf(pDest->GetBoxPosition()->x - pSource->GetBoxPosition()->x);
	//�� �簢���� y�� ������ ��
	float fRadiusY = fabsf((destBox[2].y - destBox[3].y)*0.5f + (srcBox[2].y - srcBox[3].y)*0.5f);

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

bool CCollisionMgr::CheckLineBox(CBoxCollider * pSource, CBoxCollider * pDest, D3DXVECTOR3* normal)
{
	const D3DXVECTOR3* destBox = pDest->GetBox();
	const D3DXVECTOR3* srcBox = pSource->GetBox();
	COLLIDE_TYPE coltype = pDest->GetCollideType();
	bool left;
	bool right;
	bool top;
	bool bottom;

	LINE line1, line2;

	//�ݶ��̴� Ÿ���� ���ʻ�� �ﰢ���ϰ�� 
	if (coltype == LEFT_TOP)
	{
		//�밢��
		line1.startPoint = destBox[1];
		line1.endPoint = destBox[2];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if(normal!=nullptr)
			*normal = temp;
			return true;
		}

		//������
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[1];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}

		//��� ��.
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[2];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}
	}
	//�����ϴܻﰢ��
	else if (coltype == LEFT_BOTTOM)
	{
		//�밢��
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[3];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}
		//������
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[1];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}

		//�ϴܼ�
		line1.startPoint = destBox[1];
		line1.endPoint = destBox[3];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}
	}
	//������ܻﰢ��
	else if (coltype == RIGHT_TOP)
	{

		//�밢��
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[3];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}
		//��ܼ�
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[2];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}

		//������
		line1.startPoint = destBox[2];
		line1.endPoint = destBox[3];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}
	}
	else if (coltype == RIGHT_BOTTOM)
	{
		//�밢��
		line1.startPoint = destBox[2];
		line1.endPoint = destBox[1];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}

		//������
		line1.startPoint = destBox[2];
		line1.endPoint = destBox[3];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}

		//�ϴܼ�
		line1.startPoint = destBox[1];
		line1.endPoint = destBox[3];
		if (LineRect(line1, srcBox))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
			*normal = temp;
			return true;
		}
	}
}

bool CCollisionMgr::CheckLineBox2(LINE * pSource, CBoxCollider * pDest,D3DXVECTOR3* points, D3DXVECTOR3 * normal)
{
	const D3DXVECTOR3*  destBox = pDest->GetBox();
	const LINE*			srcLine = pSource;
	COLLIDE_TYPE coltype = pDest->GetCollideType();
	bool left=false;
	bool right=false;
	bool top=false;
	bool bottom=false;
	bool diagnol=false;

	LINE line1, line2;

	vector<D3DXVECTOR3> interPoints;
	float interSectionX;
	float interSectionY;
	//�ݶ��̴� Ÿ���� ���ʻ�� �ﰢ���ϰ�� 
	if (coltype == NORMAL)
	{
		//������
		line1.startPoint = destBox[2];
		line1.endPoint = destBox[3];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			right = true;
		}

		//������
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[1];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			left = true;
		}

		//��� ��.
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[2];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			top = true;
		}
		//�ϴ� ��.
		line1.startPoint = destBox[1];
		line1.endPoint = destBox[3];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			bottom = true;
		}
	}
	else if (coltype == LEFT_TOP)
	{
		//�밢��
		line1.startPoint = destBox[1];
		line1.endPoint = destBox[2];
		if (LineDection(line1, *srcLine,&interSectionX,&interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY,0.0f));
			diagnol = true;
		}

		//������
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[1];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			left = true;
		}

		//��� ��.
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[2];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			top = true;
		}
	}
	//�����ϴܻﰢ��
	else if (coltype == LEFT_BOTTOM)
	{
		//�밢��
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[3];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			diagnol = true;
		}
		//������
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[1];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			left = true;
		}

		//�ϴܼ�
		line1.startPoint = destBox[1];
		line1.endPoint = destBox[3];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			bottom = true;
		}
	}
	//������ܻﰢ��
	else if (coltype == RIGHT_TOP)
	{

		//�밢��
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[3];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			diagnol = true;
		}
		//��ܼ�
		line1.startPoint = destBox[0];
		line1.endPoint = destBox[2];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			top = true;
		}

		//������
		line1.startPoint = destBox[2];
		line1.endPoint = destBox[3];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			right = true;
		}
	}
	else if (coltype == RIGHT_BOTTOM)
	{
		//�밢��
		line1.startPoint = destBox[2];
		line1.endPoint = destBox[1];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			diagnol = true;
		}

		//������
		line1.startPoint = destBox[2];
		line1.endPoint = destBox[3];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			right = true;
		}

		//�ϴܼ�
		line1.startPoint = destBox[1];
		line1.endPoint = destBox[3];
		if (LineDection(line1, *srcLine, &interSectionX, &interSectionY))
		{
			D3DXVECTOR3 temp = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			D3DXVec3Normalize(&temp, &temp);
			if (normal != nullptr)
				*normal = temp;
			interPoints.push_back(D3DXVECTOR3(interSectionX, interSectionY, 0.0f));
			bottom = true;
		}
	}
	if (left || right || top || bottom || diagnol)
	{
		sort(interPoints.begin(), interPoints.end(),
			[&](const D3DXVECTOR3& points, const D3DXVECTOR3& points2)
		{
			return points< points2;
		}
		);
		if (points != nullptr && !interPoints.empty())
			*points = interPoints.back();

		return true;
	}
return false;
}

D3DXVECTOR3 CCollisionMgr::GetNormalBox(const D3DXVECTOR3 * pSrcPos, CBoxCollider * pDestBox)
{
	 const D3DXVECTOR3* destBox = pDestBox->GetBox();
	 const D3DXVECTOR3* boxPos = pDestBox->GetBoxPosition();
	 D3DXVECTOR3 normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	 //�ڽ��� ������ �ִ°��
	if (pSrcPos->x < boxPos->x&&(pSrcPos->y<destBox[0].y&&pSrcPos->y>destBox[1].y))
	{
		normal = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		return normal;
	}
	//�ڽ��� ������ �ִ°��.
	else if (pSrcPos->x > boxPos->x && (pSrcPos->y<destBox[0].y&&pSrcPos->y>destBox[1].y))
	{
		normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		return normal;
	}
	//�ڽ��� ��ܿ� �ִ°��
	else if (pSrcPos->y > boxPos->y && (pSrcPos->x > destBox[0].x&&pSrcPos->x < destBox[2].x))
	{
		normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		return normal;
	}
	//�ڽ��� �ϴܿ� �ִ°��
	else if (pSrcPos->y < boxPos->y && (pSrcPos->x > destBox[0].x&&pSrcPos->x < destBox[2].x))
	{
		normal = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		return normal;
	}
	return normal;
}
