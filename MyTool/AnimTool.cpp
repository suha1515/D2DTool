// AnimTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "AnimTool.h"
#include "afxdialogex.h"


#include "MainFrm.h"
#include "MyToolView.h"


IMPLEMENT_DYNAMIC(CAnimTool, CDialogEx)

CAnimTool::CAnimTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANIMTOOL, pParent)
	, m_AnimName(_T(""))
	, m_ClipName(_T(""))
{

}

CAnimTool::~CAnimTool()
{
}

void CAnimTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMLIST, m_AnimList);
	DDX_Text(pDX, IDC_ANIMNAME, m_AnimName);
	DDX_Control(pDX, IDC_CLIPLIST, m_ClipList);
	DDX_Text(pDX, IDC_CLIPNAME, m_ClipName);
}


BEGIN_MESSAGE_MAP(CAnimTool, CDialogEx)
	ON_BN_CLICKED(IDC_OPENTOOL, &CAnimTool::OnBnClickedOpentool)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_ANIMLOAD, &CAnimTool::OnBnClickedAnimload)
	ON_LBN_DBLCLK(IDC_ANIMLIST, &CAnimTool::OnLbnDblclkAnimlist)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimTool::OnBnClickedAddAnim)
	ON_LBN_DBLCLK(IDC_CLIPLIST, &CAnimTool::OnLbnDblclkCliplist)
	ON_BN_CLICKED(IDC_BUTTON5, &CAnimTool::OnBnClickedClipAdd)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnimTool::OnBnClickedClipSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnimTool::OnBnClickedSaveAnim)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimTool::OnBnClickedListLoad)
	ON_BN_CLICKED(IDC_BUTTON7, &CAnimTool::OnBnClickedRemoveAnim)
	ON_BN_CLICKED(IDC_BUTTON6, &CAnimTool::OnBnClickedRemoveClip)
END_MESSAGE_MAP()

BOOL CAnimTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (nullptr == m_AnimMaker.GetSafeHwnd())
		m_AnimMaker.Create(IDD_ANIMFRAME);


	return TRUE;
}





//애니메이션 종류 아이템 더블 클릭 했을경우.
void CAnimTool::OnLbnDblclkAnimlist()
{
	UpdateData(TRUE);
	int index = m_AnimList.GetCurSel();
	if (index != -1)
	{
		CString animName;
		m_AnimList.GetText(index, animName);
		m_AnimName = animName;
		auto iter_find = m_AnimationList.find(animName);
		if (m_AnimationList.end() == iter_find)
		{
			MessageBox(L"리스트에 값이 없습니다!", L"Anim Tool");
			return;
		}	
		//애니메이션 종류를 클릭할 경우. 클립 리스트 항목을 다 지우고 새로운 클립 항목을 생성해야한다.
		m_ClipList.ResetContent();

		//만약 애니메이션 종류가 존재할 경우. 클립리스트를 해당 애니메이션 종류가 가지고 있는 클립 내용으로 채운다.
		for (auto& i : iter_find->second)
		{
			m_ClipList.AddString(i._clipName);
		}
	}
	UpdateData(FALSE);
}

// 클립 리스트의 아이템을 더블 클릭 했을경우
void CAnimTool::OnLbnDblclkCliplist()
{	
	int clipindex = m_ClipList.GetCurSel();
	cout << clipindex << endl;
	if (clipindex != -1)
	{
		//애니메이션 종류 가 없을 경우
		auto iter_find = m_AnimationList.find(m_AnimName); 
		if (m_AnimationList.end() == iter_find)
		{
			MessageBox(L"애니메이션 리스트에 값이 없습니다!", L"Anim Tool");
			return;
		}
		CString clipName;
		m_ClipList.GetText(clipindex, clipName);

		for (auto& i : iter_find->second)
		{
			//클립 리스트의 이름과 벡터내 클립이름이 같을 경우.
			if (!lstrcmp(clipName.operator LPCWSTR(), i._clipName))
			{
				m_AnimMaker.LoadClip(i);		//해당 애니메이션 클립을 프레임툴에 전달한다.
			}		
		}
	}
}
void CAnimTool::OnBnClickedAddAnim()
{
	UpdateData(TRUE);

	auto iter = m_AnimationList.find(m_AnimName);
	if (m_AnimationList.end() != iter)
	{
		MessageBox(L"리스트에 중복된 값이 있습니다!", L"Fail");
		return;
	}
	m_AnimList.AddString(m_AnimName);
	m_AnimationList[m_AnimName];


	UpdateData(FALSE);
}

void CAnimTool::OnBnClickedClipAdd()
{
	int index = m_AnimList.GetCurSel();
	if (index == -1)
	{
		MessageBox(L"클립을 추가하기 전에 애니메이션을 클릭하거나 생성해 주십시오!", L"AnimTool");
		return;
	}
	UpdateData(TRUE);
	// 클립 추가할 애니메이션 map에 접근한다.
	CString animName;
	m_AnimList.GetText(index, animName);
	auto iter_find = m_AnimationList.find(animName);

	//해당 애니메이션의 벡터에 접근하여 추가하려는 클립과 중복되는 내용이 있는지 확인.
	for (auto& i : iter_find->second)
	{
		//같을 경우 반복문 탈출
		if (!lstrcmp(m_ClipName, i._clipName))
		{
			MessageBox(L"클립이름이 중복됩니다.", L"AnimTool");
			return;
			break;
		}	
	}

	//중복되는것이 없다면 해당 클립이름으로 ANIM_CLIP 생성
	ANIM_CLIP clip;
	lstrcpy(clip._clipName, m_ClipName);
	clip._clipSize = 0;

	//생성된 정보를 해당 map에 저장.
	iter_find->second.push_back(clip);

	//리스트에도 추가한다.
	m_ClipList.AddString(m_ClipName);

	UpdateData(FALSE);
}
//툴열기 했을경우
void CAnimTool::OnBnClickedOpentool()
{
	//선택된 클립이 없을경우 열지 않는다.
	int index = m_ClipList.GetCurSel();
	if (index == -1)
	{
		MessageBox(L"수정할 클립을 선택해 주십시오.", L"AnimTool");
		return;
	}
	
	auto iter_find = m_AnimationList.find(m_AnimName);
	if (m_AnimationList.end() == iter_find)
	{
		MessageBox(L"애니메이션이 설정되어 있지 않습니다..", L"AnimTool");
		return;
	}
	CString clipName;
	m_ClipList.GetText(index, clipName);
	//해당 애니메이션에서 루프
	for (auto&i : iter_find->second)
	{
		if (!lstrcmp(clipName, i._clipName))
		{
			//툴정보를 업데이트한다.
			m_AnimMaker.LoadClip(i);
			break;
		}
	}
	m_AnimMaker.ShowWindow(SW_SHOW);
	

}
//애니메이션 정보를 로드한다.
void CAnimTool::OnBnClickedAnimload()
{
	CFileDialog dlg(TRUE, _T("*.anim"), NULL, OFN_FILEMUSTEXIST, _T("AnimFiles(*.anim)|*.anim|"), NULL);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Animation");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

		if (dlg.DoModal() == IDOK)
	{
		AnimationLoad(dlg.GetPathName().operator LPCWSTR());
	}
}

//클립 정보를 저장한다.
void CAnimTool::OnBnClickedClipSave()
{
	auto iter_find = m_AnimationList.find(m_AnimName);
	if (iter_find == m_AnimationList.end())
	{
		MessageBox(L"클릭된 애니메이션이 없습니다..", L"AnimTool");
		return;
	}

	CFileDialog dlg(FALSE, _T("*.anim"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("AnimFile(*.anim)|*.anim|"), NULL);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Animation");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if (dlg.DoModal() == IDOK)
	{
		AnimationSave(dlg.GetPathName().operator LPCWSTR(),m_AnimName.operator LPCWSTR());
	}
}


void CAnimTool::OnBnClickedSaveAnim()
{
	TCHAR filePath[MAX_STR] = L"../Animation/AnimList.txt";
	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(FALSE, L".txt", L"제목없음.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Animation\\AnimationList");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	TCHAR animlist[MAX_STR] = L"";

	if (IDOK == Dlg.DoModal())
	{
		wofstream fout;
		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;

		for (auto& i : m_AnimationList)
		{
			wstring temp = L"../Animation/" + i.first+".anim";
			AnimationSave(temp,i.first.operator LPCWSTR());
			fout << temp << endl;
		}
		fout.close();

		AfxMessageBox(filePath);
	}
}


void CAnimTool::OnBnClickedListLoad()
{
	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(TRUE, L".txt", L"제목없음.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Animation\\AnimationList");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if (IDOK == Dlg.DoModal())
	{
		wifstream fin;
		fin.open(Dlg.GetPathName());

		if (fin.fail())
			return;

		if (!m_AnimationList.empty())		//불러오는데 애니메이션 맵이 비워져 있지 않다면
			m_AnimationList.clear();		//클리어

		m_AnimList.ResetContent();			//리스트도 클리어.
		CAnimationMgr::GetInstance()->LoadAnimationFromList(Dlg.GetPathName().operator LPCWSTR());
		TCHAR		animPath[MAX_STR] = L"";
		while (true)
		{
			fin.getline(animPath, MAX_STR);	//애니메이션 클립의 상대경로를 불러온다.
			if (!lstrcmp(animPath, L""))
				break;
			AnimationLoad(animPath);		//애니메이션 정보 불러온다.
			
			if (fin.eof())					//다불러왔을경우 루프종료
				break;
		}

		fin.close();
	}
}

void CAnimTool::AnimationLoad(const wstring & filePath)
{
	wstring m_strPath;
	CFile file;
	CFileException kEx;
	TCHAR* m_FileName;
	TCHAR m_FilePath[MAX_STR] = L"";
	//파일 이름 얻기
	m_strPath = filePath;
	lstrcpy(m_FilePath, m_strPath.c_str());
	//확장자 제거.
	PathRemoveExtension(m_FilePath);
	//파일 이름만
	m_FileName = PathFindFileName(m_FilePath);
	MessageBox(m_FileName, L"Success");

	auto iter_find = m_AnimationList.find(m_FileName);
	if (m_AnimationList.end() != iter_find)
	{
		MessageBox(L"중복값이 있습니다.", L"AnimTool");
		return;
	}

	file.Open(m_strPath.c_str(), CFile::modeReadWrite, &kEx);

	TCHAR animName[256] = L"";							//애니메이션 이름
	int	animSize;										//애니메이션 클립 개수

	file.Read(animName, sizeof(animName));				//파일에서 애니메이션 이름 부르기
	file.Read(&animSize, sizeof(int));					//애니메이션 클립개수 부르기

	TCHAR clipName[256] = L"";							//클립이름
	int		clipSize;									//클립사이즈

	ANIM_CLIP  animInfo;
	CLIP_INFO  clipInfo;
	for (int i = 0; i < animSize; ++i)
	{
		file.Read(clipName, sizeof(clipName));			//클립 이름		불러오기
		file.Read(&clipSize, sizeof(int));				//클립 사이즈	불러오기
		lstrcpy(animInfo._clipName, clipName);			//ANIM_CLIP 클립이름 불러오기
		animInfo._clipSize = clipSize;					//클립의 사이즈 불러오기
		for (int j = 0; j < clipSize; ++j)
		{
			file.Read(&clipInfo, sizeof(clipInfo));		//ANIM_CLIP의 벡터에 들어갈 한장 정보 가공
			animInfo._clips.push_back(clipInfo);		//ANIM_CLIP으로 클립정보 가공.
		}
		//정상적으로 로드시 불러오기
		m_AnimationList[m_FileName].push_back(animInfo);		//AnimTool 의 map 컨테이너에 저장

		lstrcpy(animInfo._clipName, L"");						//다음 정보를 위해 초기화
		animInfo._clips.clear();
		animInfo._clipSize = 0;
	}

	m_AnimList.AddString(m_FileName);						//AnimTool 의 애니메이션 리스트박스에 추가
	file.Close();
	CString name;
	name.Format(L"%s %s", m_FileName, L"애니메이션 성공적으로 불러옴");
	MessageBox(name, L"Success");

}

void CAnimTool::AnimationSave(const wstring & filePath, const wstring& anim)
{
	auto iter_find = m_AnimationList.find(anim.c_str());
	if (iter_find == m_AnimationList.end())
	{
		MessageBox(L"애니메이션 정보 없음", L"AnimTool");
		return;
	}
	CString m_strPath = filePath.c_str();
	if (m_strPath.Right(5) != ".anim")
	{
		m_strPath += ".anim";
	}

	CFile file;
	CFileException ex;
	file.Open(m_strPath, CFile::modeCreate | CFile::modeReadWrite, &ex);

	//애니메이션 정보 저장을 위한 변수들./////
	vector<ANIM_CLIP>& info = iter_find->second;		//현재 선택된 AnimTool의 map컨테이너에 저장된 ANIM_CLIP정보접근
	TCHAR   animName[256] = L"";						//애니메이션 이름
	int animSize = info.size();							//애니메이션 클립 개수
	lstrcpy(animName, m_AnimName);						//에니메이션 이름 복사

	TCHAR   clipName[256] = L"";						//클립 이름
	CLIP_INFO clipinfo;									//클립정보
	int clipSize;										//클립 사이즈
														//////////////////////////////////////////

	file.Write(animName, sizeof(animName));				//애니메이션 이름	쓰기
	file.Write(&animSize, sizeof(int));					//클립 개수들		쓰기

	for (int i = 0; i < info.size(); ++i)				//클립 개수만큼 루프를 돈다.
	{
		lstrcpy(clipName, info[i]._clipName);			//클립 이름
		clipSize = info[i]._clips.size();				//클립 사이즈

		file.Write(clipName, sizeof(clipName));			//클립 이름 쓰기
		file.Write(&clipSize, sizeof(int));				//클립 사이즈 쓰기
		for (auto& j : info[i]._clips)					//ANIM_CLIP의 CLIP 벡터를 이루는 Clip을 한개씩 저장함
		{
			clipinfo = j;
			file.Write(&clipinfo, sizeof(CLIP_INFO));	//쓰기완료
		}
	}
	file.Close();
	CString name;
	name.Format(L"%s %s", m_AnimName, L"애니메이션 성공적으로 저장");
	MessageBox(name, L"Success");
}


BOOL CAnimTool::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			int index = m_ClipList.GetCurSel();

			if (index != -1)
			{
				UpdateData(TRUE);
				CString name;
				m_ClipList.GetText(index, name);
				auto iter_find  = m_AnimationList.find(m_AnimName);
				auto& vec = iter_find->second;
				for (auto&i : vec)
				{
					if (lstrcmp(i._clipName, m_ClipName))
					{
						lstrcpy(i._clipName, m_ClipName);
					}
					else
					{
						MessageBox(L"중복되는 ", L"AnimTool");
					}
				}
				UpdateData(FALSE);
			}
		}
	}

	//엔터키에 안꺼지게
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAnimTool::OnBnClickedRemoveAnim()
{
	int index = m_AnimList.GetCurSel();
	if (index != -1)
	{
		CString animName;
		m_AnimList.GetText(index, animName);
		m_AnimationList.erase(animName);
		m_AnimList.DeleteString(index);
		m_ClipList.ResetContent();
	}
}


void CAnimTool::OnBnClickedRemoveClip()
{
	int animindex = m_AnimList.GetCurSel();
	if (animindex != -1)
	{
		CString animName;
		m_AnimList.GetText(animindex, animName);
		int clipindex = m_ClipList.GetCurSel();
		if (clipindex != -1)
		{
			CString clipName;
			m_ClipList.GetText(clipindex, clipName);

			auto iter_find = m_AnimationList.find(animName);
			if (iter_find == m_AnimationList.end())
			{
				MessageBox(L"삭제하려는 애니메이션을 찾을 수 없습니다. ", L"AnimTool");
				return;
			}
			auto& vec = iter_find->second;
			auto iter_find2 = find_if(vec.begin(), vec.end(), [&](ANIM_CLIP info) 
			{
				if (!lstrcmp(info._clipName, clipName))
				{
					return true;
				}
				return false;
			});
			if (iter_find2 != vec.end())
			{
				vec.erase(iter_find2);
			}
			m_ClipList.DeleteString(clipindex);
		}
	}
	
}
