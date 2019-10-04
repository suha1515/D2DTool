#include "stdafx.h"
#include "AnimationMgr.h"

IMPLEMENT_SINGLETON(CAnimationMgr)
CAnimationMgr::CAnimationMgr()
{
}


CAnimationMgr::~CAnimationMgr()
{
	m_Clips.clear();
}

const vector<ANIM_CLIP>* CAnimationMgr::GetAnimation(const wstring & animName)
{
	auto iter_find = m_Clips.find(animName);

	if (m_Clips.end() == iter_find)
		return nullptr;

	return &iter_find->second;
}

const map<wstring, vector<ANIM_CLIP>>& CAnimationMgr::GetClip()
{
	return m_Clips;
}

HRESULT CAnimationMgr::LoadAnimationFromList(const wstring & wstrFilePath)
{
	wifstream fin;
	fin.open(wstrFilePath);

	if (fin.fail())
		return E_FAIL;
	//���� �ִϸ��̼��� �����ϸ� ����.
	if (!m_Clips.empty())
	{
		m_Clips.clear();
	}
	//�ؽ�Ʈ ���Ͽ��� �ҷ��� �ִϸ��̼� ������ ��θ� �ҷ��´�.
	TCHAR  animPath[MAX_STR] = L"";
	HRESULT	hr = 0;
	while (true)
	{
		fin.getline(animPath, MAX_STR);
		if (!lstrcmp(animPath, L""))
			break;
		if (fin.eof())
			break;
		hr = LoadAnimation(animPath);		//�ش� ��θ� ������ �ִϸ��̼����ε��Ѵ�
		wstring path = animPath;
		path = path + L"�ִϸ��̼� �ҷ����� ����";
		FAILED_CHECK_MSG_RETURN(hr, path.c_str(), E_FAIL);
	}
	fin.close();


	//HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	//if (INVALID_HANDLE_VALUE == hFile)
	//	return E_FAIL;

	////���� �ִϸ��̼��� �����ϸ� ����.
	//if (!m_Clips.empty())
	//{
	//	m_Clips.clear();
	//}
	//DWORD dwBytes = 0;
	////�ؽ�Ʈ ���Ͽ��� �ҷ��� �ִϸ��̼� ������ ��θ� �ҷ��´�.
	//TCHAR  animPath[MAX_STR] = L"";
	//HRESULT	hr = 0;
	//while (true)
	//{
	//	ReadFile(hFile, animPath, sizeof(animPath), &dwBytes, nullptr);		//�ҷ�����
	//	if (0 == dwBytes)
	//		break;

	//	hr = LoadAnimation(animPath);		//�ش� ��θ� ������ �ִϸ��̼����ε��Ѵ�.
	//	wstring path = animPath;
	//	path = path+L"�ִϸ��̼� �ҷ����� ����";
	//	FAILED_CHECK_MSG_RETURN(hr,path.c_str(),E_FAIL);
	//}
	//CloseHandle(hFile);

	return S_OK;
}

//�ִϸ��̼� �ε�
HRESULT CAnimationMgr::LoadAnimation(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	//string �Լ��� �̿��� �����̸��� Ȯ���ڸ� �����ؾ��Ѵ�.
	wstring path = wstrFilePath;
	wstring name;
	size_t sep = path.find_last_of(L"\\/");								//���ϰ�ο��� ������ / or \\��ã�´�

	if (sep != std::wstring::npos)										//ã�� ���ڿ��� ������� npos�� ��ȯ�ϰԵȴ�.
		path = path.substr(sep + 1, path.size() - sep - 1);				//substr�� �̿��Ͽ� �ش� �ε������� ������ ����

	size_t dot = path.find_last_of(L".");								//����� ���뿡�� .������ �ε����� ã�´�.
	if (dot != std::wstring::npos)
	{
		name = path.substr(0, dot);										//ó������ .������ ���ڸ� �����ϸ� �����̸�
	}
	else
		name = path;								
	
	
	//�ߺ��Ǵ� �ִϸ��̼��� ������� ����
	auto iter_find = m_Clips.find(name);
	if (iter_find!=m_Clips.end())
	{
		wstring text = name + L"�� �ߺ��Ǵ� �̸��� �ִϸ��̼� �Դϴ�!";
		MessageBox(0, name.c_str(), L"ERROR", 0);
		return E_FAIL;
	}

	DWORD dwBytes = 0;

	TCHAR animName[256] = L"";											//�ִϸ��̼� �̸�
	int	  animSize      = 0;											//�ִϸ��̼� Ŭ�� ����

	ReadFile(hFile,  animName, sizeof(animName), &dwBytes, nullptr);	//���Ͽ��� �ִϸ��̼� �̸� �ҷ�����
	ReadFile(hFile, &animSize, sizeof(int), &dwBytes, nullptr);			//���Ͽ��� �ִϸ��̼� Ŭ�� ���� �ҷ�����

	TCHAR clipName[256] = L"";											//Ŭ�� �̸�
	int   clipSize		= 0;											//Ŭ�� ������

	ANIM_CLIP animInfo;
	CLIP_INFO clipInfo;

	for (int i = 0; i < animSize; ++i)
	{
		ReadFile(hFile, clipName, sizeof(clipName), &dwBytes, nullptr);	//���Ͽ��� Ŭ���̸� �ҷ�����
		ReadFile(hFile, &clipSize, sizeof(int), &dwBytes, nullptr);		//���Ͽ��� Ŭ�������� �ҷ�����

		lstrcpy(animInfo._clipName, clipName);							//ANIM_CLIP�� Ŭ�� �̸� �����ϱ�.
		animInfo._clipSize = clipSize;									//ANIM_CLIP�� �ִ� Ŭ�� ������ ����.

		for (int j = 0; j < clipSize; ++j)
		{
			ReadFile(hFile, &clipInfo, sizeof(CLIP_INFO), &dwBytes, nullptr);	//���Ͽ��� CLIP_INFO �б� (Ŭ�������ŭ)
			animInfo._clips.push_back(clipInfo);								//ANIM_CLIP�� Ŭ�����Ϳ� ����
		}
		//���������� �ε尡 ��ٸ�. map �����̳ʿ� �߰��Ѵ�. ���� �̸��� map�� key���� �ȴ�.
		m_Clips[name].push_back(animInfo);

		lstrcpy(animInfo._clipName, L"");						//���� ������ ���� �ʱ�ȭ
		animInfo._clips.clear();
		animInfo._clipSize = 0;
	}

	CloseHandle(hFile);
	
	return S_OK;
}
