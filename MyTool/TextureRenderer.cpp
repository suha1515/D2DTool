#include "stdafx.h"
#include "TextureRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Shader.h"


CTextureRenderer::CTextureRenderer()
	:m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_texInfo(nullptr),m_pVB(nullptr),m_pIB(nullptr),m_pShader(nullptr)
{
	m_Alpha = 1.0f;
}
CTextureRenderer::~CTextureRenderer()
{
	//정점,인덱스버퍼 해제
	if (m_pVB != nullptr)
		m_pVB->Release();
	if (m_pIB != nullptr)
		m_pIB->Release();
}

void CTextureRenderer::Initialize(CGameObject * pObject)
{
	m_Tex[0].x = 0.0f, m_Tex[0].y = 0.0f;
	m_Tex[1].x = 0.0f, m_Tex[1].y = 0.0f;
	m_Tex[2].x = 0.0f, m_Tex[2].y = 0.0f;
	m_Tex[3].x = 0.0f, m_Tex[3].y = 0.0f;

	//버텍스,인덱스 버퍼 생성.
	m_pDeviceMgr->GetDevice()->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, FVF_VERTEX, D3DPOOL_MANAGED, &m_pVB, 0);
	m_pDeviceMgr->GetDevice()->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);

	SetVertex(XMFLOAT2(16,16), m_Tex);

	m_pShader = CShaderMgr::GetInstance()->GetEffect(L"firstShader");

	m_GameObject = pObject;
}

void CTextureRenderer::Render(const D3DXMATRIX& world)
{
	if (m_texInfo != nullptr)
	{
		LPD3DXEFFECT pEffect = m_pShader->GetEffect();
		D3DXMATRIX   viewMat;
		D3DXMATRIX   projMat;
		D3DXVECTOR4  animTex;  //텍스처 애니메이션을 위한 변수
		D3DXVECTOR2  texSize;	//텍스쳐 사이즈

		m_pDeviceMgr->GetDevice()->GetTransform(D3DTS_VIEW, &viewMat);
		m_pDeviceMgr->GetDevice()->GetTransform(D3DTS_PROJECTION, &projMat);
		animTex.x = m_Tex[3].x - m_Tex[0].x;
		animTex.y = m_Tex[3].y - m_Tex[0].y;
		animTex.z = m_Tex[0].x;
		animTex.w = m_Tex[0].y;
		
		texSize.x = m_Size.x;
		texSize.y = m_Size.y;
		
		//글로벌 변수 전해줘야함.
		pEffect->SetMatrix("worldMat", &world);
		pEffect->SetMatrix("viewMat",  &viewMat);
		pEffect->SetMatrix("projMat", &projMat);
		pEffect->SetFloatArray("animTex", animTex, 4);
		pEffect->SetFloatArray("texSize", texSize, 2);
		pEffect->SetFloat("gFloat", m_Alpha);

		pEffect->SetTexture("tex0", m_texInfo->pTexture);
		//이 밑에 두함수 조사.
		pEffect->Begin(nullptr, 0);	//쉐이더에서 테크닉을 정해준다. 2번째인자가 쉐이더 파일에서 테크닉이 정의된 순서 0이 첫번쨰
		pEffect->BeginPass(0);		//pass는 말그대로 pass 위와 똑같다.

		m_pDeviceMgr->GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pDeviceMgr->GetDevice()->SetIndices(m_pIB);
		m_pDeviceMgr->GetDevice()->SetFVF(FVF_VERTEX);
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

void CTextureRenderer::SetTexture(const wstring & tileName)
{
	m_TexName = tileName;
	m_texInfo = m_pTextureMgr->GetTexInfo(L"TILE_MAP", m_TexName);
	NULL_CHECK_RETURN(m_texInfo);
}

void CTextureRenderer::SetVertex(const XMFLOAT2& size, const XMFLOAT2 * tex)
{
	m_Size.x = size.x;
	m_Size.y = size.y;

	m_Tex[0] = tex[0];
	m_Tex[1] = tex[1];
	m_Tex[2] = tex[2];
	m_Tex[3] = tex[3];

	m_Vertex[0] = Vertex(-1.f,  1.f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	m_Vertex[1] = Vertex(-1.f, -1.f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[2] = Vertex(1.f,   1.f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	m_Vertex[3] = Vertex(1.f,  -1.f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	Vertex* v;
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

void CTextureRenderer::SetTexPos(const XMFLOAT2 * tex)
{
	m_Tex[0] = tex[0];
	m_Tex[1] = tex[1];
	m_Tex[2] = tex[2];
	m_Tex[3] = tex[3];
}

void CTextureRenderer::SetTexSize(const XMFLOAT2 & size)
{
	m_Size.x = size.x;
	m_Size.y = size.y;
}

const XMFLOAT2 & CTextureRenderer::GetTexPos(const int & index)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	if (index > 4 || index < 0)
	{
		MessageBox(0, L"GetTexPos out of Range!", L"ERROR", ERROR);
		return XMFLOAT2(0.0f, 0.0f);
	}
	// TODO: 여기에 반환 구문을 삽입합니다.
	return XMFLOAT2(m_Vertex[index].tex.x, m_Vertex[index].tex.y);
}

const tstring & CTextureRenderer::GetTexName()
{
	if (m_texInfo == nullptr)
	{
		MessageBox(0, L"GetTexName m_texInfo nullptr!", L"ERROR", ERROR);
		return nullptr;
	}
	return m_texInfo->textureName;
}

const TEX_INFO ** CTextureRenderer::GetTexInfo()
{
	return &m_texInfo;
}

const XMFLOAT2 & CTextureRenderer::GetTexSize()
{
	return m_Size;
}

const XMFLOAT2* CTextureRenderer::GetTexPos()
{
	return m_Tex;
}

void CTextureRenderer::Action()
{
	if (m_ComponentOn)
	{
		CTransform* pTransform = m_GameObject->GetComponent<CTransform>();
		NULL_CHECK_MSG_RETURN(pTransform, L"GameObject Transform component is null");
		D3DXMATRIX worldMat = pTransform->GetWorldMat();
		Render(worldMat);

		//D3DXVECTOR3 objectPos = m_GameObject->GetComponent<CTransform>()->GetLocalPosition();
		//cout << "현재 오브젝트 위치" << objectPos.x << " , " << objectPos.y << endl;
	}
}
