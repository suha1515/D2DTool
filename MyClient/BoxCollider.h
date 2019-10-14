#pragma once
#include "Component.h"
class CTransform;
class CBoxCollider:
	public CComponent
{
public:
	CBoxCollider();
	~CBoxCollider();

	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize(CGameObject * pObject) override;
	virtual void Action() override;

public:
	void    DrawCollide();
	void	DrawBox();
	void	DrawTriangle();
public:
	//	�ݶ��̴� �ڽ� ũ������
	void	SetBoxSize(float width,float height);
	//  �ݶ��̴� �ڽ� ������ ����.
	void	SetBoxOffset(float x, float y);
	//  �ݶ��̴� �ڽ� �����.
	void	SetBoxCollider();
	//  �ݶ��̴� ���� ����
	void	SetCollideType(COLLIDE_TYPE type);
	//  �ݶ��̴� �ڽ� ��������
	void	SetCollideColor(D3DCOLOR color);

public:
	// �ݶ��̴� �ڽ� �ޱ�
	const D3DXVECTOR3*	GetBox();

	// �ݶ��̴� ��ġ �ޱ�
	const D3DXVECTOR3*  GetBoxPosition();

	// �ݶ��̴� �ڽ� ����,���� �ޱ�
	const float		GetBoxWidth();
	const float		GetBoxHeight();
	// �ݶ��̴� �ڽ� ������ �� �ޱ�
	const float		GetBoxOffsetX();
	const float		GetBoxoffsetY();

	//�ݶ��̴� �����ޱ�
	COLLIDE_TYPE	GetCollideType();

private:
	//�ڽ��� ������
	D3DXVECTOR3			m_BoxPos[4];
	//�ڽ� �׸�
	D3DXVECTOR3			m_BoxDrawPos[4];
	//�ڽ� ����,����
	float				m_width,m_height;
	//������Ʈ ���� �ڽ� ��ġ
	float				m_PosX, m_PosY;
	//���� ������Ʈ Ʈ������ ������Ʈ ������
	CTransform*			m_pTransform;
	//���� ������Ʈ�� ��ġ
	D3DXVECTOR3			m_ObjectPos;

	//�ݶ��̴� �ڽ� ��ġ
	D3DXVECTOR3			m_BoxPosition;

	//�ݶ��̴� ���� -�ڽ�, �ﰢ�� (�ﰢ��vs�ڽ��浹)
	COLLIDE_TYPE		m_CollideType;

	D3DXVECTOR3			m_ReflectVec[3];

	D3DXCOLOR			m_BoxColor;

};

