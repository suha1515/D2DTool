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

	//FVF�� ��������� ������ Declaration ���� �����Ѵ�.
	//Declaration �� �ٷ�11ó�� ����ڰ� ���ϴ� ������ ���� �� �ִ�.
	//Declaration ����,����,�ε��� ����
	IDirect3DVertexDeclaration9* m_dVB;
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	//�ν��Ͻ���
	IDirect3DVertexBuffer9*	m_pInsVB;

	//�ν��Ͻ� ���̴�
	std::shared_ptr<CShader>			m_pShader;
	//����
	Vertex			  m_Vertex[4];
};

