#include "stdafx.h"
#include "Animator.h"
#include "GameObject.h"


//������Ʈ��
#include "TextureRenderer.h"
#include "Transform.h"

//�ִϸ��̼�
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

	m_pTexture   = m_GameObject->GetComponent<CTextureRenderer>();			//������Ʈ���� �ؽ��� ������Ʈ�� �����´�
	m_pTransform = m_GameObject->GetComponent<CTransform>();				//������Ʈ���� Ʈ������ ������Ʈ�� �����´�.
	
}

void CAnimator::Action()
{
	//�ִϸ����� ������Ʈ�� �ؽ�ó�� Ʈ������ ������Ʈ�� �ʿ���Ѵ�.
	if (nullptr == m_pTexture)
	{
		wstring Alert = m_GameObject->GetObjectName() + L" ������Ʈ�� �ؽ��� �������� �����ϴ�";
		::MessageBox(0, Alert.c_str(), L"System Error", MB_OK);
	}
	if (nullptr == m_pTransform)
	{
		wstring Alert = m_GameObject->GetObjectName() + L" ������Ʈ�� Ʈ�������� �����ϴ�";
		::MessageBox(0, Alert.c_str(), L"System Error", MB_OK);
	}

	Loop();
}

void CAnimator::Play(const wstring & clipName, ANIMATION_TYPE type)
{
	HRESULT hr = 0;
	hr = SetClips(clipName);			//Ŭ�� ����.
	FAILED_CHECK_RETURN(hr);

	m_AnimationType = type;				//�ִϸ��̼� ��� Ÿ�� (LOOP,ONCE)
	m_bIsPlaying = true;				//�ִϸ��̼� ���.

}

void CAnimator::Loop()
{
	wstring alert = m_GameObject->GetObjectName() + L"�� ������Ʈ m_pAnimation nullptr";
	NULL_CHECK_MSG_RETURN(m_pAnimation, alert.c_str());

	//������ �ƴϾ �� �ִϸ��̼� ���� �ƴҶ��� ùŬ���� �������
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
	//�ִϸ��̼� ����
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
	//�ʱ�ȭ.
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
		wstring alert = m_GameObject->GetObjectName() + L"�� ������Ʈ �ִϸ��̼� Ŭ�� ���� ����";
		MessageBox(0, alert.c_str(), L"Fail", 0);
		alert = _clipName + L"���� Ŭ���� �����ϴ�";
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
	wstring alert = m_GameObject->GetObjectName()+L"�� ������Ʈ �ִϸ��̼� Ŭ�� �ε� ����";
	FAILED_CHECK_MSG_RETURN(hr, alert.c_str());
}

wstring CAnimator::GetAnimationName()
{
	return m_pAnimation->GetAnimationName();
}
