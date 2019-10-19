#pragma once
#ifndef CTRANSFORM_H
#define CTRANSFORM_H
#include "Component.h"
class CTransform :
	public CComponent
{
public:
	CTransform();
	~CTransform();

	// CComponent��(��) ���� ��ӵ�
	virtual void Initialize(CGameObject *pObject) override;
	virtual void Action() override;
	void		 Update(CGameObject *pObject);
	
public:
	//������Ʈ �̵�,ȸ��,ũ�� ���� �Լ�
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotation(const XMFLOAT3& rot);
	void SetScaling(const D3DXVECTOR3& size);

	//������Ʈ �̵�,ȸ��,ũ�� �����Լ�
	void Translate(const D3DXVECTOR3& vec);
	void Rotate(const XMFLOAT3& rot);
	void Scale(const D3DXVECTOR3& vec);
	void SetWorld();

public:
	//��ġ�� �����ϴ� �Լ�.
	D3DXVECTOR3&  GetLocalPosition();
	//�θ���ġ�� ����Ͽ� ��ȯ
	D3DXVECTOR3*  GetWorldPos();

	//ȸ���� �����ϴ� �Լ�.
	const XMFLOAT3&		GetRotation()	 const;

	//ũ�㰪 �����ϴ� �Լ�.
	const D3DXVECTOR3&	GetScale()		 const;

	//��ġ����� �����ϴ� �Լ�.
	const D3DXMATRIX&	GetTransMat()    const;

	//ȸ������� �����ϴ� �Լ�.
	const D3DXMATRIX&	GetRotateMat()	 const;

	//ũ�� ����� �����ϴ� �Լ�.
	const D3DXMATRIX&	GetScaleMat()	 const;

	//��������� �����ϴ� �Լ�.
	const D3DXMATRIX&	GetWorldMat()	 const;

private:
	//������Ʈ ũ��,ȸ��,�̵�,����
	D3DXMATRIX				m_ScaleMat;
	D3DXMATRIX				m_RotMatX,m_RotMatY,m_RotMatZ;
	D3DXMATRIX				m_RotateMat;
	D3DXMATRIX				m_TransMat;

	D3DXMATRIX				m_WorldMat;
	D3DXMATRIX				m_ParentMat;

	//������Ʈ ��ġ,ȸ��,ũ��
	D3DXVECTOR3				m_Pos;
	D3DXVECTOR3				m_WorldPos;
	XMFLOAT3				m_Rotaion;
	D3DXVECTOR3				m_Scale;

	//�θ������Ʈ���� ����� ��ġ
	D3DXVECTOR3				m_RealPos;

	//������Ʈ �ڽ� �÷�.
	D3DXCOLOR				m_ColorBox;
};

#endif