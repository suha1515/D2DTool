#pragma once
#include "Component.h"
class CTransform :
	public CComponent
{
public:
	CTransform();
	~CTransform();

	// CComponent을(를) 통해 상속됨
	virtual void Initialize(CGameObject *pObject) override;
	virtual void Action() override;
	void		 Update(CGameObject *pObject);
	
public:
	//오브젝트 이동,회전,크기 지정 함수
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotation(const XMFLOAT3& rot);
	void SetScaling(const D3DXVECTOR3& size);

	//오브젝트 이동,회전,크기 조절함수
	void Translate(const D3DXVECTOR3& vec);
	void Rotate(const XMFLOAT3& rot);
	void Scale(const D3DXVECTOR3& vec);
	void SetWorld();

public:
	//위치값 리턴하는 함수.
	D3DXVECTOR3&  GetPosition();

	//회전값 리턴하는 함수.
	const XMFLOAT3&		GetRotation()	 const;

	//크긱밧 리턴하는 함수.
	const D3DXVECTOR3&	GetScale()	 const;

	//월드행렬을 리턴하는 함수.
	const D3DXMATRIX&	GetWorldMat()	 const;

private:
	//오브젝트 크기,회전,이동,월드
	D3DXMATRIX				m_ScaleMat;
	D3DXMATRIX				m_RotMatX,m_RotMatY,m_RotMatZ;
	D3DXMATRIX				m_TransMat;

	D3DXMATRIX				m_WorldMat;
	D3DXMATRIX				m_ParentMat;

	//오브젝트 위치,회전,크기
	D3DXVECTOR3				m_Pos;
	XMFLOAT3				m_Rotaion;
	D3DXVECTOR3				m_Scale;

	//오브젝트 박스 컬러.
	D3DXCOLOR				m_ColorBox;
};

