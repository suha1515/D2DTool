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
	//위치값 리턴하는 함수.
	const D3DXVECTOR3& GetPosition() const;

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;

	//오브젝트 크기,회전,이동,월드
	D3DXMATRIX	m_ScaleMat;
	D3DXMATRIX	m_RotMat;
	D3DXMATRIX	m_TransMat;

	D3DXMATRIX	m_WorldMat;

	//오브젝트 위치
	D3DXVECTOR3		  m_Pos;

	//오브젝트 정점
	Vertex			  m_Vertex[4];

	//오브젝트 정점,인덱스 버퍼
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	//오브젝트 텍스쳐
	const TEX_INFO*				m_texInfo;


	bool			m_bIsInit;
};

