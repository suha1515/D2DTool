// BoxColliderPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "BoxColliderPage.h"
#include "afxdialogex.h"


// CBoxColliderPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBoxColliderPage, CPropertyPage)

CBoxColliderPage::CBoxColliderPage()
	: CPropertyPage(IDD_BOXCOLLIDERPAGE)
{

}

CBoxColliderPage::~CBoxColliderPage()
{
}

void CBoxColliderPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBoxColliderPage, CPropertyPage)
END_MESSAGE_MAP()


// CBoxColliderPage 메시지 처리기입니다.
