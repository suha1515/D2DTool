// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


#include "MainFrm.h"
#include "MyToolView.h"



IMPLEMENT_DYNAMIC(CUnitTool, CDialogEx)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UNITTOOL, pParent)
	, m_Search(_T(""))
	, m_name(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Search);
	DDX_Text(pDX, IDC_EDIT2, m_name);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialogEx)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

