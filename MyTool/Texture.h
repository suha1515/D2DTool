#pragma once
class CTexture
{
protected:
	CTexture();

public:
	virtual ~CTexture();

public:
	virtual const TEX_INFO* GetTexInfo(
		const wstring& wstrStateKey = L"", /* ���� Ű*/
		int iIndex = 0	/* �̹��� ��ȣ */) PURE;

	TEX_INFO*	GetTexInfo(const wstring& wstrStateKey = L"");

public:
	virtual HRESULT LoadTexture(
		const wstring& wstrFilePath, /* �ҷ��� �̹��� ��� */
		const wstring& wstrStateKey = L"", /* ���� Ű*/
		int iImgCount = 0 /* �̹��� ���� */) PURE;

	HRESULT LoadTexture(const wstring& wstrFilePath,
						const wstring& wstrStateKey = L"");

protected:
	virtual void Release() PURE;

protected:
	CDeviceMgr*	m_pDeviceMgr;

private:
	TEX_INFO*	m_pTexInfo;
};

