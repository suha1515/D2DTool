#include "stdafx.h"
#include "Animation.h"


CAnimation::CAnimation() :m_pAnimMgr(CAnimationMgr::GetInstance())
{

}


CAnimation::~CAnimation()
{
	m_Clips.clear();
}

bool CAnimation::SetClip(const wstring & _clipName)
{
	m_ClipName = _clipName;
	if (!m_Clips.empty())
	{
		if (m_ClipName == L"")
			m_ClipName = m_Clips.begin()->first;
	}
	auto iter = m_Clips.find(m_ClipName);
	if (iter == m_Clips.end())
		return false;

	return true;
}

const vector<CLIP_INFO>* CAnimation::GetClip(const wstring & wstrClipkey)	 //클립의 이름을 키값으로 애니메이션 CLIP 벡터의 주소값을 반환한다.
{
	if (wstrClipkey == L"")		//아무것도 전달하지 않을시 가장 첫장 반환
		return m_Clips.begin()->second;

	auto iter_find = m_Clips.find(wstrClipkey);	//없으면 nullptr 반환.
	if (iter_find == m_Clips.end())
		return nullptr;
	return iter_find->second;
}

const CLIP_INFO* CAnimation::operator[](int index)							//첨자 연산자 오버로딩으로 인덱스 접근한다.
{
	auto iter = m_Clips.find(m_ClipName);
	if (iter == m_Clips.end())
		MessageBox(0, L"aniamtion 첨자 잘못 접근",L"Fail",0);

	return &(*iter->second)[index];
}

int CAnimation::GetSize()
{
	return m_Clips[m_ClipName]->size();
}

HRESULT CAnimation::LoadClip(const wstring& wstrAnimName)
{
	//로드전 기존 map컨테이너의 원소들을 제거.
	if (!m_Clips.empty())
		m_Clips.clear();

	const vector<ANIM_CLIP>* anim = m_pAnimMgr->GetAnimation(wstrAnimName);  //매니저에서 해당 애니메이션 클립의 포인터를 가져온다.
	if (anim == nullptr)													 //실패시 nullptr 반환이므로 체크한다.
		return E_FAIL;

	for (auto&i : *anim)
	{
		m_Clips[i._clipName] = &i._clips;									  //ANIM_CLIP이 가지고있는 클립들의 이름으로 클립map을 만든다.
	}

	return S_OK;
}

HRESULT CAnimation::RemoveClip(const wstring & wstrClipkey)
{
	return E_NOTIMPL;
}

CAnimation * CAnimation::Create(const wstring & wstrFilePath, const wstring & wstrStateKey)
{
	return nullptr;
}
