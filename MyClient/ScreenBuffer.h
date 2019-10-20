#pragma once
class CScreenBuffer
{
public:
	CScreenBuffer();
	~CScreenBuffer();

public:
	void Initialize(int width,int height);
	void Render();
	void Release();
private:
	//������Ʈ ����,�ε��� ����
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	LPDIRECT3DDEVICE9		m_pDevice;

	BlendVertex					m_Vertex[4];
};

