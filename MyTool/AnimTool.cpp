// AnimTool.cpp : ���� �����Դϴ�.
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





//�ִϸ��̼� ���� ������ ���� Ŭ�� �������.
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
			MessageBox(L"����Ʈ�� ���� �����ϴ�!", L"Anim Tool");
			return;
		}	
		//�ִϸ��̼� ������ Ŭ���� ���. Ŭ�� ����Ʈ �׸��� �� ����� ���ο� Ŭ�� �׸��� �����ؾ��Ѵ�.
		m_ClipList.ResetContent();

		//���� �ִϸ��̼� ������ ������ ���. Ŭ������Ʈ�� �ش� �ִϸ��̼� ������ ������ �ִ� Ŭ�� �������� ä���.
		for (auto& i : iter_find->second)
		{
			m_ClipList.AddString(i._clipName);
		}
	}
	UpdateData(FALSE);
}

// Ŭ�� ����Ʈ�� �������� ���� Ŭ�� �������
void CAnimTool::OnLbnDblclkCliplist()
{	
	int clipindex = m_ClipList.GetCurSel();
	cout << clipindex << endl;
	if (clipindex != -1)
	{
		//�ִϸ��̼� ���� �� ���� ���
		auto iter_find = m_AnimationList.find(m_AnimName); 
		if (m_AnimationList.end() == iter_find)
		{
			MessageBox(L"�ִϸ��̼� ����Ʈ�� ���� �����ϴ�!", L"Anim Tool");
			return;
		}
		CString clipName;
		m_ClipList.GetText(clipindex, clipName);

		for (auto& i : iter_find->second)
		{
			//Ŭ�� ����Ʈ�� �̸��� ���ͳ� Ŭ���̸��� ���� ���.
			if (!lstrcmp(clipName.operator LPCWSTR(), i._clipName))
			{
				m_AnimMaker.LoadClip(i);		//�ش� �ִϸ��̼� Ŭ���� ���������� �����Ѵ�.
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
		MessageBox(L"����Ʈ�� �ߺ��� ���� �ֽ��ϴ�!", L"Fail");
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
		MessageBox(L"Ŭ���� �߰��ϱ� ���� �ִϸ��̼��� Ŭ���ϰų� ������ �ֽʽÿ�!", L"AnimTool");
		return;
	}
	UpdateData(TRUE);
	// Ŭ�� �߰��� �ִϸ��̼� map�� �����Ѵ�.
	CString animName;
	m_AnimList.GetText(index, animName);
	auto iter_find = m_AnimationList.find(animName);

	//�ش� �ִϸ��̼��� ���Ϳ� �����Ͽ� �߰��Ϸ��� Ŭ���� �ߺ��Ǵ� ������ �ִ��� Ȯ��.
	for (auto& i : iter_find->second)
	{
		//���� ��� �ݺ��� Ż��
		if (!lstrcmp(m_ClipName, i._clipName))
		{
			MessageBox(L"Ŭ���̸��� �ߺ��˴ϴ�.", L"AnimTool");
			return;
			break;
		}	
	}

	//�ߺ��Ǵ°��� ���ٸ� �ش� Ŭ���̸����� ANIM_CLIP ����
	ANIM_CLIP clip;
	lstrcpy(clip._clipName, m_ClipName);
	clip._clipSize = 0;

	//������ ������ �ش� map�� ����.
	iter_find->second.push_back(clip);

	//����Ʈ���� �߰��Ѵ�.
	m_ClipList.AddString(m_ClipName);

	UpdateData(FALSE);
}
//������ �������
void CAnimTool::OnBnClickedOpentool()
{
	//���õ� Ŭ���� ������� ���� �ʴ´�.
	int index = m_ClipList.GetCurSel();
	if (index == -1)
	{
		MessageBox(L"������ Ŭ���� ������ �ֽʽÿ�.", L"AnimTool");
		return;
	}
	
	auto iter_find = m_AnimationList.find(m_AnimName);
	if (m_AnimationList.end() == iter_find)
	{
		MessageBox(L"�ִϸ��̼��� �����Ǿ� ���� �ʽ��ϴ�..", L"AnimTool");
		return;
	}
	CString clipName;
	m_ClipList.GetText(index, clipName);
	//�ش� �ִϸ��̼ǿ��� ����
	for (auto&i : iter_find->second)
	{
		if (!lstrcmp(clipName, i._clipName))
		{
			//�������� ������Ʈ�Ѵ�.
			m_AnimMaker.LoadClip(i);
			break;
		}
	}
	m_AnimMaker.ShowWindow(SW_SHOW);
	

}
//�ִϸ��̼� ������ �ε��Ѵ�.
void CAnimTool::OnBnClickedAnimload()
{
	CFileDialog dlg(TRUE, _T("*.anim"), NULL, OFN_FILEMUSTEXIST, _T("AnimFiles(*.anim)|*.anim|"), NULL);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Animation");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

		if (dlg.DoModal() == IDOK)
	{
		AnimationLoad(dlg.GetPathName().operator LPCWSTR());
	}
}

//Ŭ�� ������ �����Ѵ�.
void CAnimTool::OnBnClickedClipSave()
{
	auto iter_find = m_AnimationList.find(m_AnimName);
	if (iter_find == m_AnimationList.end())
	{
		MessageBox(L"Ŭ���� �ִϸ��̼��� �����ϴ�..", L"AnimTool");
		return;
	}

	CFileDialog dlg(FALSE, _T("*.anim"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("AnimFile(*.anim)|*.anim|"), NULL);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Animation");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	if (dlg.DoModal() == IDOK)
	{
		AnimationSave(dlg.GetPathName().operator LPCWSTR(),m_AnimName.operator LPCWSTR());
	}
}


void CAnimTool::OnBnClickedSaveAnim()
{
	TCHAR filePath[MAX_STR] = L"../Animation/AnimList.txt";
	// ���� ���� Ȥ�� ���忡 ���� ��ȭ���� MFC Ŭ����
	CFileDialog Dlg(FALSE, L".txt", L"�������.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Animation\\AnimationList");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

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
	// ���� ���� Ȥ�� ���忡 ���� ��ȭ���� MFC Ŭ����
	CFileDialog Dlg(TRUE, L".txt", L"�������.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Animation\\AnimationList");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	if (IDOK == Dlg.DoModal())
	{
		wifstream fin;
		fin.open(Dlg.GetPathName());

		if (fin.fail())
			return;

		if (!m_AnimationList.empty())		//�ҷ����µ� �ִϸ��̼� ���� ����� ���� �ʴٸ�
			m_AnimationList.clear();		//Ŭ����

		m_AnimList.ResetContent();			//����Ʈ�� Ŭ����.
		CAnimationMgr::GetInstance()->LoadAnimationFromList(Dlg.GetPathName().operator LPCWSTR());
		TCHAR		animPath[MAX_STR] = L"";
		while (true)
		{
			fin.getline(animPath, MAX_STR);	//�ִϸ��̼� Ŭ���� ����θ� �ҷ��´�.
			if (!lstrcmp(animPath, L""))
				break;
			AnimationLoad(animPath);		//�ִϸ��̼� ���� �ҷ��´�.
			
			if (fin.eof())					//�ٺҷ�������� ��������
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
	//���� �̸� ���
	m_strPath = filePath;
	lstrcpy(m_FilePath, m_strPath.c_str());
	//Ȯ���� ����.
	PathRemoveExtension(m_FilePath);
	//���� �̸���
	m_FileName = PathFindFileName(m_FilePath);
	MessageBox(m_FileName, L"Success");

	auto iter_find = m_AnimationList.find(m_FileName);
	if (m_AnimationList.end() != iter_find)
	{
		MessageBox(L"�ߺ����� �ֽ��ϴ�.", L"AnimTool");
		return;
	}

	file.Open(m_strPath.c_str(), CFile::modeReadWrite, &kEx);

	TCHAR animName[256] = L"";							//�ִϸ��̼� �̸�
	int	animSize;										//�ִϸ��̼� Ŭ�� ����

	file.Read(animName, sizeof(animName));				//���Ͽ��� �ִϸ��̼� �̸� �θ���
	file.Read(&animSize, sizeof(int));					//�ִϸ��̼� Ŭ������ �θ���

	TCHAR clipName[256] = L"";							//Ŭ���̸�
	int		clipSize;									//Ŭ��������

	ANIM_CLIP  animInfo;
	CLIP_INFO  clipInfo;
	for (int i = 0; i < animSize; ++i)
	{
		file.Read(clipName, sizeof(clipName));			//Ŭ�� �̸�		�ҷ�����
		file.Read(&clipSize, sizeof(int));				//Ŭ�� ������	�ҷ�����
		lstrcpy(animInfo._clipName, clipName);			//ANIM_CLIP Ŭ���̸� �ҷ�����
		animInfo._clipSize = clipSize;					//Ŭ���� ������ �ҷ�����
		for (int j = 0; j < clipSize; ++j)
		{
			file.Read(&clipInfo, sizeof(clipInfo));		//ANIM_CLIP�� ���Ϳ� �� ���� ���� ����
			animInfo._clips.push_back(clipInfo);		//ANIM_CLIP���� Ŭ������ ����.
		}
		//���������� �ε�� �ҷ�����
		m_AnimationList[m_FileName].push_back(animInfo);		//AnimTool �� map �����̳ʿ� ����

		lstrcpy(animInfo._clipName, L"");						//���� ������ ���� �ʱ�ȭ
		animInfo._clips.clear();
		animInfo._clipSize = 0;
	}

	m_AnimList.AddString(m_FileName);						//AnimTool �� �ִϸ��̼� ����Ʈ�ڽ��� �߰�
	file.Close();
	CString name;
	name.Format(L"%s %s", m_FileName, L"�ִϸ��̼� ���������� �ҷ���");
	MessageBox(name, L"Success");

}

void CAnimTool::AnimationSave(const wstring & filePath, const wstring& anim)
{
	auto iter_find = m_AnimationList.find(anim.c_str());
	if (iter_find == m_AnimationList.end())
	{
		MessageBox(L"�ִϸ��̼� ���� ����", L"AnimTool");
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

	//�ִϸ��̼� ���� ������ ���� ������./////
	vector<ANIM_CLIP>& info = iter_find->second;		//���� ���õ� AnimTool�� map�����̳ʿ� ����� ANIM_CLIP��������
	TCHAR   animName[256] = L"";						//�ִϸ��̼� �̸�
	int animSize = info.size();							//�ִϸ��̼� Ŭ�� ����
	lstrcpy(animName, m_AnimName);						//���ϸ��̼� �̸� ����

	TCHAR   clipName[256] = L"";						//Ŭ�� �̸�
	CLIP_INFO clipinfo;									//Ŭ������
	int clipSize;										//Ŭ�� ������
														//////////////////////////////////////////

	file.Write(animName, sizeof(animName));				//�ִϸ��̼� �̸�	����
	file.Write(&animSize, sizeof(int));					//Ŭ�� ������		����

	for (int i = 0; i < info.size(); ++i)				//Ŭ�� ������ŭ ������ ����.
	{
		lstrcpy(clipName, info[i]._clipName);			//Ŭ�� �̸�
		clipSize = info[i]._clips.size();				//Ŭ�� ������

		file.Write(clipName, sizeof(clipName));			//Ŭ�� �̸� ����
		file.Write(&clipSize, sizeof(int));				//Ŭ�� ������ ����
		for (auto& j : info[i]._clips)					//ANIM_CLIP�� CLIP ���͸� �̷�� Clip�� �Ѱ��� ������
		{
			clipinfo = j;
			file.Write(&clipinfo, sizeof(CLIP_INFO));	//����Ϸ�
		}
	}
	file.Close();
	CString name;
	name.Format(L"%s %s", m_AnimName, L"�ִϸ��̼� ���������� ����");
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
						MessageBox(L"�ߺ��Ǵ� ", L"AnimTool");
					}
				}
				UpdateData(FALSE);
			}
		}
	}

	//����Ű�� �Ȳ�����
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
				MessageBox(L"�����Ϸ��� �ִϸ��̼��� ã�� �� �����ϴ�. ", L"AnimTool");
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
