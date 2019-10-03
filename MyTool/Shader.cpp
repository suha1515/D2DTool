#include "stdafx.h"
#include "Shader.h"


CShader::CShader()
	:m_pDeviceMgr(CDeviceMgr::GetInstance())
	,m_Effect(nullptr)
{
}


CShader::~CShader()
{
	if(m_Effect!=nullptr)
	m_Effect->Release();
}

LPD3DXEFFECT CShader::GetEffect()
{
	return m_Effect;
}

void CShader::SetEffect(const wstring & filePath)
{
	HRESULT hr = 0;
	//�������� ����
	hr = D3DXCreateEffectFromFile(
		m_pDeviceMgr->GetDevice(), filePath.c_str(), nullptr, nullptr,
		0, nullptr, &m_Effect, nullptr);
	FAILED_CHECK_MSG_RETURN(hr, L"����Ʈ ���� �б� ����.");
}

void CShader::Action()
{

}

void CShader::Initialize(CGameObject * pObject)
{
}

