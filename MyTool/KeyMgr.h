#pragma once

const DWORD KEY_UP =	0x00000001;	// 0000 0000 0000 0000 0000 0000 0000 0001
const DWORD KEY_DOWN =	0x00000002;	// 0000 0000 0000 0000 0000 0000 0000 0010
const DWORD KEY_LEFT =	0x00000004;	// 0000 0000 0000 0000 0000 0000 0000 0100
const DWORD KEY_RIGHT =	0x00000008;	// 0000 0000 0000 0000 0000 0000 0000 1000

const DWORD KEY_RETURN = 0x00000010;	// 0000 0000 0000 0000 0000 0000 0001 0000
const DWORD KEY_SPACE = 0x00000020;	// 0000 0000 0000 0000 0000 0000 0010 0000

const DWORD KEY_LBUTTON = 0x00000040;	// 0000 0000 0000 0000 0000 0000 0100 0000
const DWORD KEY_RBUTTON = 0x00000080;	// 0000 0000 0000 0000 0000 0000 1000 0000

const DWORD KEY_W = 0x00000100;
const DWORD KEY_A = 0x00000200;
const DWORD KEY_S = 0x00000400;
const DWORD KEY_D = 0x00000800;

const DWORD KEY_P = 0x00001000;
const DWORD KEY_V = 0x00002000;

class CMouse;
class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	CKeyMgr();
	~CKeyMgr();

public:
	void Update();

public:
	bool KeyPressing(DWORD dwKey);
	bool KeyDown(DWORD dwKey);
	bool KeyUp(DWORD dwKey);

private:
	DWORD m_dwKey;
	DWORD m_dwKeyDown;
	DWORD m_dwKeyUp;

	CMouse*		m_Mouse;
};
