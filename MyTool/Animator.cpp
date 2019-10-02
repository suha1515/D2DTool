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

void CAnimator::Initialize()
{
}

void CAnimator::Action(CGameObject * pObject)
{
	if (nullptr == pObject->GetComponent<CTextureRenderer>())
	{
		wstring Alert = pObject->GetObjectName()+L" 오브젝트는 텍스쳐 렌더러가 없습니다";
		AfxMessageBox(L"");
	}

}

bool CAnimator::Play(const wstring & clipName, ANIMATION_TYPE type)
{
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
