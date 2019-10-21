#pragma once
class CGameObject;
class CShader;
class CInstanceMgr
{
	DECLARE_SINGLETON(CInstanceMgr)
public:
	CInstanceMgr();
	~CInstanceMgr();
public:
	void		Initialize();
public:
	void		Clear();
public:
	void		InstanceRender();
public:
	void		AddObject(CGameObject* pObject);

private:
	CDeviceMgr*						m_pDeviceMgr;
	map<string, list<CGameObject*>> m_mapInstance;

	//FVF는 정해진대로 넣지만 Declaration 으로 지정한다.
	//Declaration 은 다렉11처럼 사용자가 원하는 정점을 만들 수 있다.
	//Declaration 정점,정점,인덱스 버퍼
	IDirect3DVertexDeclaration9* m_dVB;
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	//인스턴스용
	IDirect3DVertexBuffer9*	m_pInsVB;

	//인스턴스 쉐이더
	std::shared_ptr<CShader>			m_pShader;
	//정점
	Vertex			  m_Vertex[4];
};

