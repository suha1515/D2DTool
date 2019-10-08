#pragma once

class CShader;
class CShaderMgr
{
	DECLARE_SINGLETON(CShaderMgr)
public:
	CShaderMgr();
	~CShaderMgr();
public:
	void		  Initialize();
public:
	shared_ptr<CShader> GetEffect(const wstring& effectKey);

private:
	map<wstring, shared_ptr<CShader>> m_Efftects;

private:
	CDeviceMgr*				  m_pDeviceMgr;
};

