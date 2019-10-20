#include "stdafx.h"
#include "ScreenBuffer.h"


CScreenBuffer::CScreenBuffer()
{
}


CScreenBuffer::~CScreenBuffer()
{
}

void CScreenBuffer::Initialize(int width, int height)
{
	m_pDevice = CDeviceMgr::GetInstance()->GetDevice();

	m_pDevice->CreateVertexBuffer(4 * sizeof(BlendVertex),D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW| D3DFVF_TEX1, D3DPOOL_MANAGED, &m_pVB, 0);
	m_pDevice->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);
	
	m_Vertex[0] = BlendVertex(	-0.5f,		  -0.5f		   ,0.0f,  0.0f, 0.0f, 0.0f);
	m_Vertex[1] = BlendVertex(  -0.5f,		 height-0.5f   ,0.0f,  0.0f, 0.0f, 1.0f);
	m_Vertex[2] = BlendVertex(	width -0.5f, -0.5f		   ,0.0f,  0.0f, 1.0f, 0.0f);
	m_Vertex[3] = BlendVertex(  width -0.5f, height -0.5f  ,0.0f,  0.0f, 1.0f, 1.0f);
	BlendVertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	v[0] = m_Vertex[0];
	v[1] = m_Vertex[1];
	v[2] = m_Vertex[2];
	v[3] = m_Vertex[3];

	m_pVB->Unlock();

	WORD* i = 0;
	m_pIB->Lock(0, 0, (void**)&i, 0);
	i[0] = 0, i[1] = 2, i[2] = 1;
	i[3] = 1, i[4] = 2, i[5] = 3;

	m_pIB->Unlock();
}

void CScreenBuffer::Render()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(BlendVertex));
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->SetFVF(FVF_BLEND_VERTEX);
	m_pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		6,
		0,
		2
	);
}

void CScreenBuffer::Release()
{
	if (m_pVB != nullptr)
		m_pVB->Release();
	if (m_pIB != nullptr)
		m_pIB->Release();
}
