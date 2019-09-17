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
	// IDirect3D9�� ������
	// IDirect3D9: ��ġ�� ���� �� �����ϰ� IDirect3DDevice9�� �����ϴ� ���� ���.
	LPDIRECT3D9			m_pSDK;

	// IDirect3DDevice9�� ������
	// IDirect3DDevice9: ��ġ�� ���ؼ� �׷��� �������� �����ϵ��� �ϴ� ���� ���.
	LPDIRECT3DDEVICE9	m_pDevice;

	// ID3DXSprite�� ������
	// ID3DXSprite: ���̷�Ʈ �󿡼� 2D �ؽ�ó �������� �����ϴ� Com��ü.
	LPD3DXSPRITE		m_pSprite;

	// ID3DXFont�� ������
	// ID3DXFont: ���̷�Ʈ �󿡼� 2D ��Ʈ �������� �����ϴ� Com��ü.
	LPD3DXFONT			m_pFont; // ���̷�Ʈ ��Ʈ

	// I�� Interface�� ����
	// DirectX���� �����Ǵ� �ڷ��� �� I�� ���۵Ǵ� �༮���� Com��ü�� �Ѵ�.

	// COM (Component Object Model)
	// ����ũ�μ���Ʈ�翡�� �����ϴ� ���α׷��� ��
	// ���α׷����� ��ǰ �����ϵ��� �غ���.

	
};

