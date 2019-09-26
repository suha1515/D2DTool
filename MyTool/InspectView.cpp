// InspectView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "InspectView.h"


// CInspectView

IMPLEMENT_DYNCREATE(CInspectView, CFormView)

CInspectView::CInspectView()
	: CFormView(IDD_INSPECTOR)
{

}

CInspectView::~CInspectView()
{
}

void CInspectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInspectView, CFormView)
END_MESSAGE_MAP()


// CInspectView 진단입니다.

#ifdef _DEBUG
void CInspectView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInspectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInspectView 메시지 처리기입니다.
