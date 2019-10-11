#pragma once
#include "Component.h"
class CTransform;
class CBoxCollider :
	public CComponent
{
public:
	CBoxCollider();
	~CBoxCollider();

	// CComponent을(를) 통해 상속됨
	virtual void Initialize(CGameObject * pObject) override;
	virtual void Action() override;

public:
	void    DrawCollide();
	void	DrawBox();
	void	DrawTriangle();
public:
	//	콜라이더 박스 크기지정
	void	SetBoxSize(float width,float height);
	//  콜라이더 박스 오프셋 지정.
	void	SetBoxOffset(float x, float y);
	//  콜라이더 박스 만들기.
	void	SetBoxCollider();

public:
	// 콜라이더 박스 받기
	const D3DXVECTOR3*	GetBox();

	// 콜라이더 위치 받기
	const D3DXVECTOR3*  GetBoxPosition();

	// 콜라이더 박스 가로,세로 받기
	const float		GetBoxWidth();
	const float		GetBoxHeight();
	// 콜라이더 박스 오프셋 값 받기
	const float		GetBoxOffsetX();
	const float		GetBoxoffsetY();

private:
	//박스의 각점들
	D3DXVECTOR3			m_BoxPos[4];
	//박스 그림
	D3DXVECTOR3			m_BoxDrawPos[4];
	//박스 가로,세로
	float				m_width,m_height;
	//오브젝트 기준 박스 위치
	float				m_PosX, m_PosY;
	//게임 오브젝트 트랜스폼 컴포넌트 포인터
	CTransform*			m_pTransform;
	//게임 오브젝트의 위치
	D3DXVECTOR3			m_ObjectPos;

	//콜라이더 박스 위치
	D3DXVECTOR3			m_BoxPosition;

	//콜라이더 종류 -박스, 삼각형 (삼각형vs박스충돌)
	COLLIDE_TYPE		m_CollideType;

};


