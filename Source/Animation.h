#pragma once
class CAnimation
{
public:
	CAnimation();
	~CAnimation();
public:
	wstring								GetAnimationName();
public:
	bool								SetClip(const wstring& _clipName=L"");
	const vector<CLIP_INFO>*			GetClip(const wstring& wstrClipkey=L"");
public:
	const CLIP_INFO*					operator[](int index);							//첨자 연산자 오버로딩 클립하나씩 반환한다.

public:
	int									GetSize();

public:
	HRESULT								LoadClip(const wstring& wstrAnimName);			//애니메이션을 매니저에서 불러온다.
public:
	HRESULT								RemoveClip(const wstring& wstrClipkey);
public:
	static	CAnimation*					Create(const wstring&	wstrFilePath,
											   const wstring&	wstrStateKey);

private:
	CAnimationMgr*								m_pAnimMgr;			//애니메이션 매니저
	wstring										m_AnimName;			//애니메이션 이름
	wstring										m_ClipName;			//클립 이름
	map<wstring, const vector<CLIP_INFO>*>		m_Clips;			//애니메이션이 소유한 클립들. key값은 클립들의 이름.
	
};

