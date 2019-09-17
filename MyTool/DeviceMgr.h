#pragma once
class CDeviceMgr
{
	DECLARE_SINGLETON(CDeviceMgr)

public:
	enum MODE { MODE_FULL, MODE_WIN };

private:
	CDeviceMgr();
	~CDeviceMgr();

public:
	LPDIRECT3DDEVICE9 GetDevice();
	LPD3DXSPRITE GetSprite();
	LPD3DXFONT GetFont();

public:
	HRESULT InitDevice(MODE eMode);
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);

private:
	void Release();

private:
	// IDirect3D9의 포인터
	// IDirect3D9: 장치를 조사 및 검증하고 IDirect3DDevice9을 생성하는 역할 담당.
	LPDIRECT3D9			m_pSDK;

	// IDirect3DDevice9의 포인터
	// IDirect3DDevice9: 장치를 통해서 그래픽 렌더링을 수행하도록 하는 역할 담당.
	LPDIRECT3DDEVICE9	m_pDevice;

	// ID3DXSprite의 포인터
	// ID3DXSprite: 다이렉트 상에서 2D 텍스처 렌더링을 수행하는 Com객체.
	LPD3DXSPRITE		m_pSprite;

	// ID3DXFont의 포인터
	// ID3DXFont: 다이렉트 상에서 2D 폰트 렌더링을 수행하는 Com객체.
	LPD3DXFONT			m_pFont; // 다이렉트 폰트

	// I는 Interface의 약자
	// DirectX에서 제공되는 자료형 중 I로 시작되는 녀석들을 Com객체라 한다.

	// COM (Component Object Model)
	// 마이크로소프트사에서 제시하는 프로그래밍 모델
	// 프로그래밍을 부품 조립하듯이 해보자.

	
};

