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
	// �ϵ����(���κ���)�� ���� �ִ� ���ػ� Ÿ�̸� ���� ������ �Լ�.
	// ���⼭ ���ϴ� Ÿ�̸� ���� ������ �ֱ⸦ ������ ���� ���Ѵ�. 
	// ������ �ֱ�� CPU �������� ��Եȴ�.
	QueryPerformanceCounter(&m_OriginTime);
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	// CPU�� �ʴ� �������� ������ �Լ�.
	QueryPerformanceFrequency(&m_CPUTick);

	return S_OK;
}

void CTimeMgr::UpdateTime()
{
	QueryPerformanceCounter(&m_CurTime);

	// �ֱ������� CPU������ ����.
	if (m_CurTime.QuadPart - m_OriginTime.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OriginTime = m_CurTime;
	}

	m_fDeltaTime = float(m_CurTime.QuadPart - m_OldTime.QuadPart)
		/ m_CPUTick.QuadPart;

	m_OldTime = m_CurTime;
}
