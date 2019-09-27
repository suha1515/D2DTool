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
	ON_BN_CLICKED(IDC_BUTTON1, &CInspectView::OnBnClickedHierarchy)
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
void CInspectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if (nullptr == m_HierarchyView.GetSafeHwnd())
		m_HierarchyView.Create(IDD_HIERARCHY);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CInspectView::OnBnClickedHierarchy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_HierarchyView.ShowWindow(SW_SHOW);
}


