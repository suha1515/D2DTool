// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

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


// CUnitTool �޽��� ó�����Դϴ�.
