#pragma once
class CFrameMgr
{
	DECLARE_SINGLETON(CFrameMgr)

private:
	CFrameMgr();
	~CFrameMgr();

public:
	HRESULT InitTime();
	bool LockFrame(float fps);
	void RenderFPS();

private:
	LARGE_INTEGER	m_OriginTime;
	LARGE_INTEGER	m_OldTime;
	LARGE_INTEGER	m_CurTime;
	LARGE_INTEGER	m_CPUTick;

	float			m_fTimeCount;
	float			m_fTimeCount2;

	TCHAR			m_szFPS[MIN_STR];
	int				m_iFPS;
};

