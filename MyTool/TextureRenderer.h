#pragma once
#include "Component.h"

class CShader;
class CTextureRenderer : public CComponent
{
public:
	CTextureRenderer();
	~CTextureRenderer();

public:
	// 오브젝트 렌더 함수
	void Render(const D3DXMATRIX& world);
public:
	// 오브젝트 버텍스, 텍스처 지정 함수 
	void SetTexture(const tstring& tileName);
	void SetVertex(const int& sizeX, const int& sizeY, const XMFLOAT2* tex);

public:
	//텍스처 위치 가져오기
	const XMFLOAT2&		GetTexPos(const int& index);
	//텍스쳐 이름 가져오기
	const tstring&		GetTexName();
	//텍스쳐 포인터 반환
	const TEX_INFO**	GetTexInfo();
	//텍스쳐 크기 반환
	const XMFLOAT2&		GetTexSize();
public:
	// CComponent을(를) 통해 상속됨
	// 컴포넌트 초기화
	virtual void Initialize(CGameObject * pObject)override;
	virtual void Action() override;
private:
	//텍스처 매니저
	CTextureMgr*	m_pTextureMgr;
	//디바이스 매니저
	CDeviceMgr*		m_pDeviceMgr;

	//오브젝트 정점,인덱스 버퍼
	IDirect3DVertexBuffer9*	m_pVB;
	IDirect3DIndexBuffer9*	m_pIB;

	//오브젝트 정점
	Vertex			  m_Vertex[4];

	//오브젝트 텍스쳐
	const TEX_INFO*				m_texInfo;

	
	//렌더러 쉐이더
	shared_ptr<CShader>			m_pShader;

	//크기
	XMFLOAT2					m_Size;

	
};

