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
	//기존 애니메이션이 존재하면 비운다.
	if (!m_Clips.empty())
	{
		m_Clips.clear();
	}
	//텍스트 파일에서 불러올 애니메이션 파일의 경로를 불러온다.
	TCHAR  animPath[MAX_STR] = L"";
	HRESULT	hr = 0;
	while (true)
	{
		fin.getline(animPath, MAX_STR);
		if (!lstrcmp(animPath, L""))
			break;
		if (fin.eof())
			break;
		hr = LoadAnimation(animPath);		//해당 경로를 가지고 애니메이션을로드한다
		wstring path = animPath;
		path = path + L"애니메이션 불러오기 실패";
		FAILED_CHECK_MSG_RETURN(hr, path.c_str(), E_FAIL);
	}
	fin.close();


	//HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	//if (INVALID_HANDLE_VALUE == hFile)
	//	return E_FAIL;

	////기존 애니메이션이 존재하면 비운다.
	//if (!m_Clips.empty())
	//{
	//	m_Clips.clear();
	//}
	//DWORD dwBytes = 0;
	////텍스트 파일에서 불러올 애니메이션 파일의 경로를 불러온다.
	//TCHAR  animPath[MAX_STR] = L"";
	//HRESULT	hr = 0;
	//while (true)
	//{
	//	ReadFile(hFile, animPath, sizeof(animPath), &dwBytes, nullptr);		//불러오기
	//	if (0 == dwBytes)
	//		break;

	//	hr = LoadAnimation(animPath);		//해당 경로를 가지고 애니메이션을로드한다.
	//	wstring path = animPath;
	//	path = path+L"애니메이션 불러오기 실패";
	//	FAILED_CHECK_MSG_RETURN(hr,path.c_str(),E_FAIL);
	//}
	//CloseHandle(hFile);

	return S_OK;
}

//애니메이션 로드
HRESULT CAnimationMgr::LoadAnimation(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	//string 함수를 이용해 파일이름과 확장자를 추출해야한다.
	wstring path = wstrFilePath;
	wstring name;
	size_t sep = path.find_last_of(L"\\/");								//파일경로에서 마지막 / or \\을찾는다

	if (sep != std::wstring::npos)										//찾는 문자열이 없을경우 npos를 반환하게된다.
		path = path.substr(sep + 1, path.size() - sep - 1);				//substr을 이용하여 해당 인덱스부터 끝까지 추출

	size_t dot = path.find_last_of(L".");								//추출된 내용에서 .까지의 인덱스를 찾는다.
	if (dot != std::wstring::npos)
	{
		name = path.substr(0, dot);										//처음부터 .까지의 문자를 추출하면 파일이름
	}
	else
		name = path;								
	
	
	//중복되는 애니메이션이 있을경우 금지
	auto iter_find = m_Clips.find(name);
	if (iter_find!=m_Clips.end())
	{
		wstring text = name + L"는 중복되는 이름의 애니메이션 입니다!";
		MessageBox(0, name.c_str(), L"ERROR", 0);
		return E_FAIL;
	}

	DWORD dwBytes = 0;

	TCHAR animName[256] = L"";											//애니메이션 이름
	int	  animSize      = 0;											//애니메이션 클립 개수

	ReadFile(hFile,  animName, sizeof(animName), &dwBytes, nullptr);	//파일에서 애니메이션 이름 불러오기
	ReadFile(hFile, &animSize, sizeof(int), &dwBytes, nullptr);			//파일에서 애니메이션 클립 개수 불러오기

	TCHAR clipName[256] = L"";											//클립 이름
	int   clipSize		= 0;											//클립 사이즈

	ANIM_CLIP animInfo;
	CLIP_INFO clipInfo;

	for (int i = 0; i < animSize; ++i)
	{
		ReadFile(hFile, clipName, sizeof(clipName), &dwBytes, nullptr);	//파일에서 클립이름 불러오기
		ReadFile(hFile, &clipSize, sizeof(int), &dwBytes, nullptr);		//파일에서 클립사이즈 불러오기

		lstrcpy(animInfo._clipName, clipName);							//ANIM_CLIP에 클립 이름 지정하기.
		animInfo._clipSize = clipSize;									//ANIM_CLIP에 있는 클립 사이즈 지정.

		for (int j = 0; j < clipSize; ++j)
		{
			ReadFile(hFile, &clipInfo, sizeof(CLIP_INFO), &dwBytes, nullptr);	//파일에서 CLIP_INFO 읽기 (클립사이즈만큼)
			animInfo._clips.push_back(clipInfo);								//ANIM_CLIP의 클립벡터에 삽입
		}
		//정상적으로 로드가 됬다면. map 컨테이너에 추가한다. 파일 이름이 map의 key값이 된다.
		m_Clips[name].push_back(animInfo);

		lstrcpy(animInfo._clipName, L"");						//다음 정보를 위해 초기화
		animInfo._clips.clear();
		animInfo._clipSize = 0;
	}

	CloseHandle(hFile);
	
	return S_OK;
}
