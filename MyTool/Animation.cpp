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

const vector<CLIP_INFO>* CAnimation::GetClip(const wstring & wstrClipkey)	 //Ŭ���� �̸��� Ű������ �ִϸ��̼� CLIP ������ �ּҰ��� ��ȯ�Ѵ�.
{
	if (wstrClipkey == L"")		//�ƹ��͵� �������� ������ ���� ù�� ��ȯ
		return m_Clips.begin()->second;

	auto iter_find = m_Clips.find(wstrClipkey);	//������ nullptr ��ȯ.
	if (iter_find == m_Clips.end())
		return nullptr;
	return iter_find->second;
}

const CLIP_INFO* CAnimation::operator[](int index)							//÷�� ������ �����ε����� �ε��� �����Ѵ�.
{
	auto iter = m_Clips.find(m_ClipName);
	if (iter == m_Clips.end())
		MessageBox(0, L"aniamtion ÷�� �߸� ����",L"Fail",0);

	return &(*iter->second)[index];
}

int CAnimation::GetSize()
{
	return m_Clips[m_ClipName]->size();
}

HRESULT CAnimation::LoadClip(const wstring& wstrAnimName)
{
	//�ε��� ���� map�����̳��� ���ҵ��� ����.
	if (!m_Clips.empty())
		m_Clips.clear();

	const vector<ANIM_CLIP>* anim = m_pAnimMgr->GetAnimation(wstrAnimName);  //�Ŵ������� �ش� �ִϸ��̼� Ŭ���� �����͸� �����´�.
	if (anim == nullptr)													 //���н� nullptr ��ȯ�̹Ƿ� üũ�Ѵ�.
		return E_FAIL;

	for (auto&i : *anim)
	{
		m_Clips[i._clipName] = &i._clips;									  //ANIM_CLIP�� �������ִ� Ŭ������ �̸����� Ŭ��map�� �����.
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
