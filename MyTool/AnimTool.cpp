// AnimTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "AnimTool.h"
#include "afxdialogex.h"


// CAnimTool 대화 상자입니다.

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
}


BEGIN_MESSAGE_MAP(CAnimTool, CDialogEx)
	ON_BN_CLICKED(IDC_OPENTOOL, &CAnimTool::OnBnClickedOpentool)
END_MESSAGE_MAP()


// CAnimTool 메시지 처리기입니다.


void CAnimTool::OnBnClickedOpentool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
