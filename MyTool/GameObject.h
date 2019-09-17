#pragma once
class CGameObject
{
protected:
	CGameObject();

public:
	virtual ~CGameObject();

public:
	virtual int Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual HRESULT LateInit();
	virtual void Release() PURE;

public:
	//위치값 리턴하는 함수.
	const D3DXVECTOR3& GetPosition() const;

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;	

	//오브젝트 크기,회전,이동,월드

	//오브젝트 위치
	D3DXVECTOR3		  m_Pos;

	//오브젝트 정점
	Vertex			  m_Vertex[4];
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	bool			m_bIsInit;	
};

