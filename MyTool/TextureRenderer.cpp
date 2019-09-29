#include "stdafx.h"
#include "TextureRenderer.h"
#include "GameObject.h"
#include "Shader.h"


CTextureRenderer::CTextureRenderer()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_texInfo(nullptr),m_pVB(nullptr),m_pIB(nullptr),m_pShader(nullptr)
{
}


CTextureRenderer::~CTextureRenderer()
{
	//����,�ε������� ����
	if (m_pVB != nullptr)
		m_pVB->Release();
	if (m_pIB != nullptr)
		m_pIB->Release();
}

void CTextureRenderer::Initialize()
{
	XMFLOAT2 tex[4];
	tex[0].x = 0.0f, tex[0].y = 0.0f;
	tex[1].x = 0.0f, tex[1].y = 0.0f;
	tex[2].x = 0.0f, tex[2].y = 0.0f;
	tex[3].x = 0.0f, tex[3].y = 0.0f;
	SetVertex(16, tex);

	m_pShader = CShaderMgr::GetInstance()->GetEffect(L"firstShader");

}

void CTextureRenderer::Render(const D3DXMATRIX& world)
{
	if (m_texInfo != nullptr)
	{
	
		LPD3DXEFFECT pEffect = m_pShader->GetEffect();
		D3DXMATRIX   viewMat;
		D3DXMATRIX   projMat;

		m_pDeviceMgr->GetDevice()->GetTransform(D3DTS_VIEW, &viewMat);
		m_pDeviceMgr->GetDevice()->GetTransform(D3DTS_PROJECTION, &projMat);

		//�۷ι� ���� ���������.
		pEffect->SetMatrix("worldMat", &world);
		pEffect->SetMatrix("viewMat",  &viewMat);
		pEffect->SetMatrix("projMat", &projMat);

		pEffect->SetTexture("tex0", m_texInfo->pTexture);
		//�� �ؿ� ���Լ� ����.
		pEffect->Begin(nullptr, 0);	//���̴����� ��ũ���� �����ش�. 2��°���ڰ� ���̴� ���Ͽ��� ��ũ���� ���ǵ� ���� 0�� ù����
		pEffect->BeginPass(0);		//pass�� ���״�� pass ���� �Ȱ���.

		//m_pDeviceMgr->GetDevice()->SetTransform(D3DTS_WORLD, &world);

		
		m_pDeviceMgr->GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pDeviceMgr->GetDevice()->SetIndices(m_pIB);
		m_pDeviceMgr->GetDevice()->SetFVF(FVF_VERTEX);
		//m_pDeviceMgr->GetDevice()->SetTexture(0, m_texInfo->pTexture);

		//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		//m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		m_pDeviceMgr->GetDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			6,
			0,
			2
		);
		

		pEffect->EndPass();
		pEffect->End();
	}	
}

void CTextureRenderer::SetTexture(const tstring & tileName)
{
	m_texInfo = m_pTextureMgr->GetTexInfo(L"TILE_MAP", tileName);
	NULL_CHECK_RETURN(m_texInfo);
}

void CTextureRenderer::SetVertex(const int & size, const XMFLOAT2 * tex)
{
	if (m_pVB != nullptr)
		m_pVB->Release();
	if (m_pIB != nullptr)
		m_pIB->Release();

	m_Vertex[0] = Vertex(-size*0.5f,  size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[0].x, tex[0].y);
	m_Vertex[1] = Vertex(-size*0.5f, -size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[1].x, tex[1].y);
	m_Vertex[2] = Vertex( size*0.5f,  size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[2].x, tex[2].y);
	m_Vertex[3] = Vertex( size*0.5f, -size*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, tex[3].x, tex[3].y);

	m_pDeviceMgr->GetDevice()->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, FVF_VERTEX, D3DPOOL_MANAGED, &m_pVB, 0);
	Vertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	v[0] = m_Vertex[0];
	v[1] = m_Vertex[1];
	v[2] = m_Vertex[2];
	v[3] = m_Vertex[3];

	m_pVB->Unlock();

	m_pDeviceMgr->GetDevice()->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);

	WORD* i = 0;
	m_pIB->Lock(0, 0, (void**)&i, 0);
	i[0] = 0, i[1] = 2, i[2] = 1;
	i[3] = 1, i[4] = 2, i[5] = 3;


	m_pIB->Unlock();
}

const XMFLOAT2 & CTextureRenderer::GetTexPos(const int & index)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	if (index > 4 || index < 0)
	{
		MessageBox(0, L"GetTexPos out of Range!", L"ERROR", ERROR);
		return XMFLOAT2(0.0f, 0.0f);
	}
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return XMFLOAT2(m_Vertex[index].tex.x, m_Vertex[index].tex.y);
}

const tstring & CTextureRenderer::GetTexName()
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	if (m_texInfo == nullptr)
	{
		MessageBox(0, L"GetTexName m_texInfo nullptr!", L"ERROR", ERROR);
		return nullptr;
	}
	return m_texInfo->textureName;
}

void CTextureRenderer::Action(CGameObject * pObject)
{
	Render(pObject->GetWorldMat());

	D3DXVECTOR3 objectPos = pObject->GetPosition();
	cout<<"���� ������Ʈ ��ġ"<<objectPos.x<<" , "<<objectPos.y<<endl;
}
