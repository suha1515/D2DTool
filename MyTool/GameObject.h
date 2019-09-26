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

	//������Ʈ �̵�,ȸ��,ũ�� ���� �Լ�
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotation(const XMFLOAT3& rot);
	void SetScaling(const D3DXVECTOR3& size);

	//������Ʈ �̵�,ȸ��,ũ�� �����Լ�
	void Translate(const D3DXVECTOR3& vec);
	void Rotate(const XMFLOAT3& rot);
	void Scale(const D3DXVECTOR3& vec);

	//������Ʈ ���ؽ�, �ؽ��� ���� �Լ�. (�ӽ� ������Ʈ�� �ٲܰ�.)
	void SetVertex(const int& size, const XMFLOAT2* tex);
	void SetTexture(const CString& tileName);


	
public:
	//��ġ�� �����ϴ� �Լ�.
	const D3DXVECTOR3& GetPosition() const;
	const XMFLOAT2& GetTexPos(const int& index);
	const CString&  GetTexName();

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;

	CString			m_ObjectName;
	CString			m_ObjectTag;
	CString			m_ObjectLayer;
	
	//������Ʈ ũ��,ȸ��,�̵�,����
	D3DXMATRIX	m_ScaleMat;
	D3DXMATRIX	m_RotMat;
	D3DXMATRIX	m_TransMat;

	D3DXMATRIX	m_WorldMat;

	//������Ʈ ��ġ,ȸ��,ũ��
	D3DXVECTOR3		  m_Pos;
	XMFLOAT3		  m_Rotaion;
	D3DXVECTOR3		  m_Scale;

	//������Ʈ ����
	Vertex			  m_Vertex[4];

	//������Ʈ ����,�ε��� ����
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	//������Ʈ �ؽ���
	const TEX_INFO*				m_texInfo;


	bool			m_bIsInit;
};

