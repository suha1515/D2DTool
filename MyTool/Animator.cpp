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
	//�ִϸ����� ������Ʈ�� �ؽ�ó�� Ʈ������ ������Ʈ�� �ʿ���Ѵ�.
	if (nullptr == m_GameObject->GetComponent<CTextureRenderer>())
	{
		wstring Alert = m_GameObject->GetObjectName() + L" ������Ʈ�� �ؽ��� �������� �����ϴ�";
		::MessageBox(0, Alert.c_str(), L"System Error", MB_OK);
	}
	if (nullptr == m_GameObject->GetComponent<CTransform>())
	{
		wstring Alert = m_GameObject->GetObjectName() + L" ������Ʈ�� Ʈ�������� �����ϴ�";
		::MessageBox(0, Alert.c_str(), L"System Error", MB_OK);
	}

}

bool CAnimator::Play(const wstring & clipName, ANIMATION_TYPE type)
{
	m_AnimationType = type;				//�ִϸ��̼� ��� Ÿ�� (LOOP,ONCE)
	
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
