// InspectorView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "InspectorView.h"
#include "afxdialogex.h"


// CInspectorView ��ȭ �����Դϴ�.

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


// CInspectorView �޽��� ó�����Դϴ�.
