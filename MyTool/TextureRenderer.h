#pragma once
#include "Component.h"

class CShader;
class CTextureRenderer : public CComponent
{
public:
	CTextureRenderer();
	~CTextureRenderer();

public:
	// ������Ʈ ���� �Լ�
	void Render(const D3DXMATRIX& world);
public:
	// ������Ʈ ���ؽ�, �ؽ�ó ���� �Լ� 

	void SetTexture(const tstring& tileName);
	void SetVertex(const int& size, const XMFLOAT2* tex);

public:
	const XMFLOAT2& GetTexPos(const int& index);
	const tstring& GetTexName();
public:
	// CComponent��(��) ���� ��ӵ�
	// ������Ʈ �ʱ�ȭ
	virtual void Initialize();
	virtual void Action(CGameObject * pObject) override;
private:
	//�ؽ�ó �Ŵ���
	CTextureMgr*	m_pTextureMgr;
	//����̽� �Ŵ���
	CDeviceMgr*		m_pDeviceMgr;

	//������Ʈ ����,�ε��� ����
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	//������Ʈ ����
	Vertex			  m_Vertex[4];

	//������Ʈ �ؽ���
	const TEX_INFO*				m_texInfo;

	
	//������ ���̴�
	shared_ptr<CShader>			m_pShader;
	
};

