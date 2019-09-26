// InspectorView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "InspectorView.h"
#include "afxdialogex.h"


// CInspectorView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInspectorView, CDialogEx)

CInspectorView::CInspectorView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INSPECTOR, pParent)
{

}

CInspectorView::~CInspectorView()
{
}

void CInspectorView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInspectorView, CDialogEx)
END_MESSAGE_MAP()


// CInspectorView 메시지 처리기입니다.
