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

	//오브젝트 이동,회전,크기 지정 함수
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotation(const XMFLOAT3& rot);
	void SetScaling(const D3DXVECTOR3& size);

	//오브젝트 이동,회전,크기 조절함수
	void Translate(const D3DXVECTOR3& vec);
	void Rotate(const XMFLOAT3& rot);
	void Scale(const D3DXVECTOR3& vec);

	//오브젝트 버텍스, 텍스쳐 지정 함수. (임시 컴포넌트로 바꿀것.)
	void SetVertex(const int& size, const XMFLOAT2* tex);
	void SetTexture(const CString& tileName);


	
public:
	//위치값 리턴하는 함수.
	const D3DXVECTOR3& GetPosition() const;
	const XMFLOAT2& GetTexPos(const int& index);
	const CString&  GetTexName();

protected:
	CDeviceMgr*		m_pDeviceMgr;
	CTextureMgr*	m_pTextureMgr;

	CString			m_ObjectName;
	CString			m_ObjectTag;
	CString			m_ObjectLayer;
	
	//오브젝트 크기,회전,이동,월드
	D3DXMATRIX	m_ScaleMat;
	D3DXMATRIX	m_RotMat;
	D3DXMATRIX	m_TransMat;

	D3DXMATRIX	m_WorldMat;

	//오브젝트 위치,회전,크기
	D3DXVECTOR3		  m_Pos;
	XMFLOAT3		  m_Rotaion;
	D3DXVECTOR3		  m_Scale;

	//오브젝트 정점
	Vertex			  m_Vertex[4];

	//오브젝트 정점,인덱스 버퍼
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	//오브젝트 텍스쳐
	const TEX_INFO*				m_texInfo;


	bool			m_bIsInit;
};

