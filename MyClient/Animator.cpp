#include "stdafx.h"
#include "Animator.h"
#include "GameObject.h"


//컴포넌트들
#include "TextureRenderer.h"
#include "Transform.h"

//애니메이션
#include "Animation.h"


CAnimator::CAnimator() :m_pAnimation(nullptr)
					, m_pTexture(nullptr), m_pTransform(nullptr)
{
	m_Time = 0.0f;
	m_StartIndex = 0;
	m_EndIndex = 0;
	m_bIsPlaying = false;
}


CAnimator::~CAnimator()
{
	SafeDelete(m_pAnimation);
}

void CAnimator::Initialize(CGameObject * pObject)
{
	m_GameObject = pObject;

	m_pAnimation = new CAnimation;

	m_pTexture   = m_GameObject->GetComponent<CTextureRenderer>();			//오브젝트에서 텍스쳐 컴포넌트를 가져온다
	m_pTransform = m_GameObject->GetComponent<CTransform>();				//오브젝트에서 트랜스폼 컴포넌트를 가져온다.
	
}

void CAnimator::Action()
{
	//애니메이터 컴포넌트는 텍스처와 트랜스폼 컴포넌트가 필요로한다.
	if (nullptr == m_pTexture)
	{
		wstring Alert = m_GameObject->GetObjectName() + L" 오브젝트는 텍스쳐 렌더러가 없습니다";
		::MessageBox(0, Alert.c_str(), L"System Error", MB_OK);
	}
	if (nullptr == m_pTransform)
	{
		wstring Alert = m_GameObject->GetObjectName() + L" 오브젝트는 트랜스폼이 없습니다";
		::MessageBox(0, Alert.c_str(), L"System Error", MB_OK);
	}

	Loop();
}

void CAnimator::Play(const wstring & clipName, ANIMATION_TYPE type)
{
	HRESULT hr = 0;
	hr = SetClips(clipName);			//클립 지정.
	FAILED_CHECK_RETURN(hr);

	m_AnimationType = type;				//애니메이션 재생 타입 (LOOP,ONCE)
	m_bIsPlaying = true;				//애니메이션 재생.

}

void CAnimator::Loop()
{
	wstring alert = m_GameObject->GetObjectName() + L"번 오브젝트 m_pAnimation nullptr";
	NULL_CHECK_MSG_RETURN(m_pAnimation, alert.c_str());

	//루프가 아니어도 즉 애니메이션 실행 아닐때는 첫클립의 장면으로
	if (!m_bIsPlaying)
	{
		if (m_pAnimation->SetClip())
		{
			auto vec = (*m_pAnimation)[0];
			XMFLOAT2 size = XMFLOAT2(vec->_texSizeX, vec->_texSizeY);
			m_pTexture->SetTexture(vec->_texName);
			m_pTexture->SetTexPos(vec->_Tex);
			m_pTexture->SetTexSize(size);
		}
		else
			return;
	}
	//애니메이션 루프
	if (m_bIsPlaying)
	{
		if (m_StartIndex < m_EndIndex)
		{
			auto vec = (*m_pAnimation)[m_StartIndex];
			XMFLOAT2 size = XMFLOAT2(vec->_texSizeX, vec->_texSizeY);
			m_pTexture->SetTexture(vec->_texName);
			m_pTexture->SetTexPos(vec->_Tex);
			m_pTexture->SetTexSize(size);
			if (m_Time >= vec->_startTime)
			{
				m_Time -= m_Time;
				m_StartIndex++;
			}
			m_Time += CTimeMgr::GetInstance()->GetDeltaTime();
		}
		else
		{
			switch (m_AnimationType)
			{
			case ANIMATION_LOOP:
				m_StartIndex = 0;
				m_bIsPlaying = true;
				break;
			case ANIMATION_ONCE:
				m_bIsPlaying = false;
				break;
			default:
				break;
			}
		}
	}
}

void CAnimator::Stop()
{
	m_bIsPlaying = false;
}

void CAnimator::ReSet()
{
	//초기화.
	auto vec = (*m_pAnimation)[0];
	XMFLOAT2 size = XMFLOAT2(vec->_texSizeX, vec->_texSizeY);
	m_pTexture->SetTexture(vec->_texName);
	m_pTexture->SetTexPos(vec->_Tex);
	m_pTexture->SetTexSize(size);
	m_bIsPlaying = false;

	m_StartIndex = 0;
}

bool CAnimator::IsPlaying(const wstring& _clipName)
{
	if (m_ClipName == _clipName)
		return m_bIsPlaying;
	if (_clipName == L"")
		return m_bIsPlaying;

		return FALSE;
}

HRESULT CAnimator::SetClips(const wstring & _clipName)
{
	if (!m_pAnimation->SetClip(_clipName))
	{
		wstring alert = m_GameObject->GetObjectName() + L"번 오브젝트 애니메이션 클립 변경 실패";
		MessageBox(0, alert.c_str(), L"Fail", 0);
		alert = _clipName + L"같은 클립이 없습니다";
		MessageBox(0, alert.c_str(), L"Fail", 0);
		return E_FAIL;
	}
	m_ClipName = _clipName;
	m_EndIndex = m_pAnimation->GetSize();
	m_StartIndex = 0;

	return S_OK;
}
void CAnimator::LoadClips(const wstring & _animation)
{
	HRESULT hr = 0;
	hr = m_pAnimation->LoadClip(_animation);
	wstring alert = m_GameObject->GetObjectName()+L"번 오브젝트 애니메이션 클립 로드 실패";
	FAILED_CHECK_MSG_RETURN(hr, alert.c_str());
}

wstring CAnimator::GetAnimationName()
{
	return m_pAnimation->GetAnimationName();
}
