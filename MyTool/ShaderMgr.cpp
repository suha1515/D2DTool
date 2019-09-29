#include "stdafx.h"
#include "ShaderMgr.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(CShaderMgr)
CShaderMgr::CShaderMgr()
	:m_pDeviceMgr(CDeviceMgr::GetInstance())
{

}

CShaderMgr::~CShaderMgr()
{
	for (auto& i : m_Efftects)
		i.second.reset();
	m_Efftects.clear();
}

void CShaderMgr::Initialize()
{
	m_Efftects.insert({ L"firstShader",make_shared<CShader>() });
	m_Efftects[L"firstShader"]->SetEffect(L"../Shaders/myShader1.fx");

}

shared_ptr<CShader> CShaderMgr::GetEffect(const wstring& effectKey)
{
	return m_Efftects[effectKey];
}
