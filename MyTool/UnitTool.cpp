// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnitTool, CDialogEx)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UNITTOOL, pParent)
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialogEx)
END_MESSAGE_MAP()


// CUnitTool 메시지 처리기입니다.
