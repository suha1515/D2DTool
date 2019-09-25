#pragma once
class CGameObject
{
public:
	CGameObject();

public:
	virtual ~CGameObject();

public:
	virtual int Update();
	virtual void LateUpdate();
	virtual void Render();

	virtual HRESULT Initialize();
	virtual HRESULT LateInit();
	virtual void Release();

public:
	void SetPosition(const D3DXVECTOR3& pos);
	void SetVertex(const int& size, const XMFLOAT2* tex);
	void SetTexture(const CString& tileName);


	void Translate(const D3DXVECTOR3& vec);
public:
	//��ġ�� �����ϴ� �Լ�.
	const D3DXVECTOR3& GetPosition() const;

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;

	//������Ʈ ũ��,ȸ��,�̵�,����
	D3DXMATRIX	m_ScaleMat;
	D3DXMATRIX	m_RotMat;
	D3DXMATRIX	m_TransMat;

	D3DXMATRIX	m_WorldMat;

	//������Ʈ ��ġ
	D3DXVECTOR3		  m_Pos;

	//������Ʈ ����
	Vertex			  m_Vertex[4];

	//������Ʈ ����,�ε��� ����
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	//������Ʈ �ؽ���
	const TEX_INFO*				m_texInfo;


	bool			m_bIsInit;
};

