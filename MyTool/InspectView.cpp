// InspectView.cpp : ���� �����Դϴ�.
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


// CInspectView �����Դϴ�.

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


// CInspectView �޽��� ó�����Դϴ�.
void CInspectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if (nullptr == m_HierarchyView.GetSafeHwnd())
		m_HierarchyView.Create(IDD_HIERARCHY);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void CInspectView::OnBnClickedHierarchy()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_HierarchyView.ShowWindow(SW_SHOW);
}


