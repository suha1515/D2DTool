#include "stdafx.h"
#include "FrameMgr.h"

IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
	: m_fTimeCount(0.f), m_fTimeCount2(0.f), m_szFPS(L""), m_iFPS(0)
{
	ZeroMemory(&m_OriginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CPUTick, sizeof(LARGE_INTEGER));
}


CFrameMgr::~CFrameMgr()
{
}

HRESULT CFrameMgr::InitTime()
{
	QueryPerformanceCounter(&m_OriginTime);
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);
	QueryPerformanceFrequency(&m_CPUTick);

	return S_OK;
}

bool CFrameMgr::LockFrame(float fps)
{
	QueryPerformanceCounter(&m_CurTime);

	// 주기적으로 CPU진동수 갱신.
	if (m_CurTime.QuadPart - m_OriginTime.QuadPart > m_CPUTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CPUTick);
		m_OriginTime = m_CurTime;
	}

	m_fTimeCount += float(m_CurTime.QuadPart - m_OldTime.QuadPart)
		/ m_CPUTick.QuadPart;
	m_OldTime = m_CurTime;

	if (1.f / fps <= m_fTimeCount)
	{
		++m_iFPS;
		m_fTimeCount = 0.f;
		return true;
	}

	return false;
}

void CFrameMgr::RenderFPS()
{
	m_fTimeCount2 += CTimeMgr::GetInstance()->GetDeltaTime();

	if (1.f <= m_fTimeCount2)
	{
		swprintf_s(m_szFPS, L"FPS: %d", m_iFPS);
		m_iFPS = 0;
		m_fTimeCount2 = 0.f;
	}

	// FPS 출력
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 600.f, 100.f, 0.f);

	CDeviceMgr::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::GetInstance()->GetFont()->DrawText(
		CDeviceMgr::GetInstance()->GetSprite(), /* 스프라이트 Com 객체*/
		m_szFPS, /* 출력할 문자열 */
		lstrlen(m_szFPS), /* 문자열 길이 */
		nullptr, /* 사각형 영역 */
		0, /* 사각형 영역 안에 그릴 때의 옵션 */
		D3DCOLOR_ARGB(255, 0, 255, 0) /* 출력 색상 */);
}
