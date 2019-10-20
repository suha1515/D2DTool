#pragma once

//
class CTarget
{
public:
	CTarget();
	~CTarget();

public:
	HRESULT	Initialize(const int& width, const int& height,const D3DFORMAT& format,const D3DXCOLOR& color);
	HRESULT	ChangeNewDevice(int target);
	HRESULT	ChangeToPreDevice(int target);
	HRESULT	ClearColor();
	HRESULT Release();
	LPDIRECT3DTEXTURE9 GetTexture();


private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPDIRECT3DTEXTURE9      m_pTexture;

	LPDIRECT3DSURFACE9		m_pCurSurface;
	LPDIRECT3DSURFACE9		m_pPreSurface;

	D3DXCOLOR				m_ClearColor;




};

