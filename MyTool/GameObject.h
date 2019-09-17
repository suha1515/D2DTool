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
	//��ġ�� �����ϴ� �Լ�.
	const D3DXVECTOR3& GetPosition() const;

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;	

	//������Ʈ ũ��,ȸ��,�̵�,����

	//������Ʈ ��ġ
	D3DXVECTOR3		  m_Pos;

	//������Ʈ ����
	Vertex			  m_Vertex[4];
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	bool			m_bIsInit;	
};

