#include "stdafx.h"
#include "Animator.h"
#include "GameObject.h"

#include "TextureRenderer.h"
#include "Transform.h"

CAnimator::CAnimator()
{
}


CAnimator::~CAnimator()
{
}

void CAnimator::Initialize(CGameObject * pObject)
{

}

void CAnimator::Action()
{
	//애니메이터 컴포넌트는 텍스처와 트랜스폼 컴포넌트가 필요로한다.
	if (nullptr == m_GameObject->GetComponent<CTextureRenderer>())
	{
		wstring Alert = m_GameObject->GetObjectName() + L" 오브젝트는 텍스쳐 렌더러가 없습니다";
		::MessageBox(0, Alert.c_str(), L"System Error", MB_OK);
	}
	if (nullptr == m_GameObject->GetComponent<CTransform>())
	{
		wstring Alert = m_GameObject->GetObjectName() + L" 오브젝트는 트랜스폼이 없습니다";
		::MessageBox(0, Alert.c_str(), L"System Error", MB_OK);
	}

}

bool CAnimator::Play(const wstring & clipName, ANIMATION_TYPE type)
{
	m_AnimationType = type;				//애니메이션 재생 타입 (LOOP,ONCE)
	
	switch (m_AnimationType)
	{
	case ANIMATION_LOOP:
		
		break;
	case ANIMATION_ONCE:

		break;
	default:
		break;
	}

	return false;
}

void CAnimator::Stop()
{
}

void CAnimator::ReSet()
{
}

bool CAnimator::IsPlaying(const wstring & clipName)
{
	return false;
}

void CAnimator::SetClips(const wstring & _animation)
{
	m_AnimationClip.insert({});
}
