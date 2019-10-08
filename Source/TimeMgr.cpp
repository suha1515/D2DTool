#include "stdafx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr()
{
	ZeroMemory(&m_OriginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CPUTick, sizeof(LARGE_INTEGER));
}


CTimeMgr::~CTimeMgr()
{
}

float CTimeMgr::GetDeltaTime()
{
	return m_fDeltaTime;
}

HRESULT CTimeMgr::InitTime()
{
	// 하드웨어(메인보드)가 갖고 있는 고해상도 타이머 값을 얻어오는 함수.
	// 여기서 말하는 타이머 값은 일정한 주기를 누적한 값을 말한다. 
	// 일정한 주기는 CPU 진동수에 비롯된다.
	QueryPerformanceCounter(&m_OriginTime);
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// CPU의 초당 진동수를 얻어오는 함수.
	QueryPerformanceFrequency(&m_CPUTick);

	return S_OK;
}

void CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);

	// 주기적으로 CPU진동수 갱신.
	if (m_CurTime.QuadPart - m_OriginTime.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OriginTime = m_CurTime;
	}

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime.QuadPart)
		/ m_CPUTick.QuadPart;

	m_OldTime = m_CurTime;
}
