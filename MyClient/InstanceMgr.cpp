#include "stdafx.h"
#include "InstanceMgr.h"

#include "GameObject.h"
#include "TextureRenderer.h"
#include "Transform.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(CInstanceMgr)

CInstanceMgr::CInstanceMgr()
{
}
CInstanceMgr::~CInstanceMgr()
{
	m_mapInstance.clear();
	if (m_dVB != nullptr)
		m_dVB->Release();
	if (m_pVB != nullptr)
		m_pVB->Release();
	if (m_pIB != nullptr)
		m_pIB->Release();
	if (m_pInsVB != nullptr)
		m_pInsVB->Release();

}
void CInstanceMgr::Initialize()
{
	//정점을 새로 만든다.
	//SET_FVF 해주듯이 정점에 대한 정보를 넘겨야하므로
	// 정점에 대한 정보를 밑에 element 변수로 넘겨야한다.
	D3DVERTEXELEMENT9      Element[MAX_FVF_DECL_SIZE] = {
		{ 0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT ,D3DDECLUSAGE_POSITION,0 },
		{ 0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT ,D3DDECLUSAGE_NORMAL,0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		{ 1,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
		{ 1, 64, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
		{ 1, 72, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 6 },
		D3DDECL_END()
	};

	//Declaration 버퍼 생성
	m_pDeviceMgr = CDeviceMgr::GetInstance();
	m_pDeviceMgr->GetDevice()->CreateVertexDeclaration(Element, &m_dVB);
	

	m_pDeviceMgr->GetDevice()->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, FVF_VERTEX, D3DPOOL_MANAGED, &m_pVB, 0);
	m_pDeviceMgr->GetDevice()->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);

	//왜 0,0을 넣는지 알아보자.
	m_pDeviceMgr->GetDevice()->CreateVertexBuffer((256 * 256) * sizeof(Vertex01), 0, 0, D3DPOOL_MANAGED, &m_pInsVB, 0);

	Vertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);
	v[0] = Vertex(-8.0f, 8.0f, 0, 0, 0, 0, 0.f, 0.f);
	v[1] = Vertex(-8.0f, -8.0f, 0, 0, 0,0, 0.f, 1.f);
	v[2] = Vertex( 8.0f, 8.0f, 0, 0, 0, 0, 1.f, 0.f);
	v[3] = Vertex( 8.0f,-8.0f, 0, 0, 0, 0, 1.f, 1.f);
	m_pVB->Unlock();
	
	WORD* i = 0;
	m_pIB->Lock(0, 0, (void**)&i, 0);
	i[0] = 0, i[1] = 2, i[2] = 1;
	i[3] = 1, i[4] = 2, i[5] = 3;

	m_pIB->Unlock();

	Vertex01* v2;
	m_pInsVB->Lock(0, 0, (void**)&v2, 0);
	for (int i = 0; i < 256 * 256; ++i)
	{
		D3DXMatrixIdentity(&v2[i].worldMat);
		v2[i].tex0 = D3DXVECTOR2(0.0f, 0.0f);
		v2[i].tex1 = D3DXVECTOR2(0.0f, 0.0f);
	}
	m_pInsVB->Unlock();

	m_pShader = CShaderMgr::GetInstance()->GetEffect(L"InstanceShader");
}
void CInstanceMgr::InstanceRender()
{
	for (auto &i : m_mapInstance)
	{
		int index = 0;
		Vertex01* v2;
		m_pInsVB->Lock(0, 0, (void**)&v2, 0);
		for (auto&j : i.second)
		{
			const XMFLOAT2*  tex = j->GetComponent<CTextureRenderer>()->GetTexPos();
			v2[index].worldMat = j->GetComponent<CTransform>()->GetWorldMat();
			v2[index].tex0 = D3DXVECTOR2(tex[0].x, tex[0].y);
			v2[index].tex1 = D3DXVECTOR2(tex[3].x - tex[0].x, tex[3].y - tex[0].y);		//크기
			//tex[0].x, tex[0].y
			//tex[3].x-tex[0].x,tex[3].y-tex[0].y
			index++;
			
		}
		m_pInsVB->Unlock();

		m_pDeviceMgr->GetDevice()->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | i.second.size()));
		m_pDeviceMgr->GetDevice()->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		m_pDeviceMgr->GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pDeviceMgr->GetDevice()->SetStreamSource(1, m_pInsVB, 0, sizeof(Vertex01));
		m_pDeviceMgr->GetDevice()->SetIndices(m_pIB);
		m_pDeviceMgr->GetDevice()->SetVertexDeclaration(m_dVB);

		LPD3DXEFFECT pEffect = m_pShader->GetEffect();
		D3DXMATRIX   viewMat;
		D3DXMATRIX   projMat;

		m_pDeviceMgr->GetDevice()->GetTransform(D3DTS_VIEW, &viewMat);
		m_pDeviceMgr->GetDevice()->GetTransform(D3DTS_PROJECTION, &projMat);

		//글로벌 변수 전해줘야함.
		pEffect->SetMatrix("viewMat", &viewMat);
		pEffect->SetMatrix("projMat", &projMat);

		auto object = i.second.begin();
		//(*(*object)->GetComponent<CTextureRenderer>()->GetTexInfo())->pTexture;
		pEffect->SetTexture("tex0", (*(*object)->GetComponent<CTextureRenderer>()->GetTexInfo())->pTexture);

		//이 밑에 두함수 조사.
		pEffect->Begin(nullptr, 0);	//쉐이더에서 테크닉을 정해준다. 2번째인자가 쉐이더 파일에서 테크닉이 정의된 순서 0이 첫번쨰
		pEffect->BeginPass(0);		//pass는 말그대로 pass 위와 똑같다.

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
	//원래로 돌린다.
	m_pDeviceMgr->GetDevice()->SetStreamSourceFreq(0, 0);
	m_pDeviceMgr->GetDevice()->SetStreamSourceFreq(1, 1);

	//클리어
	m_mapInstance.clear();
}
void CInstanceMgr::AddObject( CGameObject * pObject)
{
	CTextureRenderer* pTexture = pObject->GetComponent<CTextureRenderer>();
	wstring texName = pTexture->GetTexName();
	string name;
	name.assign(texName.begin(), texName.end());

	m_mapInstance[name].push_back(pObject);

}


