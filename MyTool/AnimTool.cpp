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
{

}

CAnimTool::~CAnimTool()
{
}

void CAnimTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMLIST, m_AnimList);
}


BEGIN_MESSAGE_MAP(CAnimTool, CDialogEx)
	ON_BN_CLICKED(IDC_OPENTOOL, &CAnimTool::OnBnClickedOpentool)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_ANIMLOAD, &CAnimTool::OnBnClickedAnimload)
	ON_LBN_DBLCLK(IDC_ANIMLIST, &CAnimTool::OnLbnDblclkAnimlist)
END_MESSAGE_MAP()


// CAnimTool �޽��� ó�����Դϴ�.


void CAnimTool::OnBnClickedOpentool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_AnimMaker.ShowWindow(SW_SHOW);
}




BOOL CAnimTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (nullptr == m_AnimMaker.GetSafeHwnd())
		m_AnimMaker.Create(IDD_ANIMFRAME);


	return TRUE;
}


void CAnimTool::OnBnClickedAnimload()
{
	CStdioFile	file;
	CFileException kEx;
	CFileDialog dlg(TRUE, _T("*.anim"), NULL, OFN_FILEMUSTEXIST, _T("Anim Files(*.anim)|*.anim|"), NULL);
	CString m_strPath;

	if (dlg.DoModal() == IDOK)
	{
		//���� �̸� ���
		m_strPath = dlg.GetPathName();
		file.Open(m_strPath, CFile::modeReadWrite, &kEx);
		TCHAR _Animname[256]=L"";
		file.Read(&_Animname, sizeof(_Animname));

		ANIM_CLIP info;
		while (file.Read(&info, sizeof(ANIM_CLIP)))
		{
			ANIM_CLIP read;
			read = info;
			m_AnimationList[_Animname].push_back(read);
		}
		m_AnimList.AddString(_Animname);
		file.Close();
		MessageBox(L"�������� ���������� �ҷ���", L"Success");
	}
}


void CAnimTool::OnLbnDblclkAnimlist()
{
	int index = m_AnimList.GetCurSel();
	if (index != -1)
	{
		CString animName;
		m_AnimList.GetText(index, animName);
		m_AnimMaker.LoadAnimation(animName,m_AnimationList[animName]);
	}
}
