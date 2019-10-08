#pragma once
class CAnimationMgr
{
	DECLARE_SINGLETON(CAnimationMgr)
public:
	CAnimationMgr();
	~CAnimationMgr();

public:
	const vector<ANIM_CLIP>*			   GetAnimation(const wstring& animName);
	const map<wstring, vector<ANIM_CLIP>>& GetClip();

public:
	HRESULT	LoadAnimationFromList(const wstring& wstrFilePath);		//txt ���ϸ���Ʈ���� ������ �ҷ�����
	HRESULT LoadAnimation(const wstring& wstrFilePath);				//�ϳ��� ��ο��� �ҷ�����


private:
	map<wstring,vector<ANIM_CLIP>>		m_Clips;					//Ŭ������ ���� �����̳� Ŭ������ ������ �ٲ�� �ȵȴ�.

};

