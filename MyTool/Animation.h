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
	const CLIP_INFO*					operator[](int index);							//÷�� ������ �����ε� Ŭ���ϳ��� ��ȯ�Ѵ�.

public:
	int									GetSize();

public:
	HRESULT								LoadClip(const wstring& wstrAnimName);			//�ִϸ��̼��� �Ŵ������� �ҷ��´�.
public:
	HRESULT								RemoveClip(const wstring& wstrClipkey);
public:
	static	CAnimation*					Create(const wstring&	wstrFilePath,
											   const wstring&	wstrStateKey);

private:
	CAnimationMgr*								m_pAnimMgr;			//�ִϸ��̼� �Ŵ���
	wstring										m_AnimName;			//�ִϸ��̼� �̸�
	wstring										m_ClipName;			//Ŭ�� �̸�
	map<wstring, const vector<CLIP_INFO>*>		m_Clips;			//�ִϸ��̼��� ������ Ŭ����. key���� Ŭ������ �̸�.
	
};

