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
	// ������Ʈ ���ؽ�, �ؽ�ó ���� �Լ� ,�ؽ�ó ũ�� �����Լ�
	void SetTexture(const wstring& tileName);
	void SetVertex(const XMFLOAT2& size, const XMFLOAT2* tex);
	void SetTexPos(const XMFLOAT2* tex);
	void SetTexSize(const XMFLOAT2& size);

public:
	//�ؽ�ó ��ġ ��������
	const XMFLOAT2&		GetTexPos(const int& index);
	//�ؽ��� �̸� ��������
	const tstring&		GetTexName();
	//�ؽ��� ������ ��ȯ
	const TEX_INFO**	GetTexInfo();
	//�ؽ��� ũ�� ��ȯ
	const XMFLOAT2&		GetTexSize();
	//�ؽ��� ��ǥ�� ��ȯ
	const XMFLOAT2*		GetTexPos();
public:
	// CComponent��(��) ���� ��ӵ�
	// ������Ʈ �ʱ�ȭ
	virtual void Initialize(CGameObject * pObject)override;
	virtual void Action() override;
private:
	//�ؽ�ó �Ŵ���
	CTextureMgr*	m_pTextureMgr;
	//����̽� �Ŵ���
	CDeviceMgr*		m_pDeviceMgr;

	//������Ʈ ����,�ε��� ����
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	//������Ʈ ����
	Vertex						m_Vertex[4];

	//������Ʈ �ؽ���
	const TEX_INFO*				m_texInfo;

	//������Ʈ �ؽ��� ��ǥ��
	XMFLOAT2					m_Tex[4];
	
	//������ ���̴�
	shared_ptr<CShader>			m_pShader;

	//ũ��
	XMFLOAT2					m_Size;
	//�ؽ�ó �̸�
	wstring						m_TexName;

	
};

