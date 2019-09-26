// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MyForm.h"
#include "Camera.h"

#include "MainFrm.h"
#include "MiniView.h"





// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{

}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ToolList, m_ToolList);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ToolList, &CMyForm::OnTcnSelchangeToollist)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.



void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//���̾�α� ����
	m_ToolList.InsertItem(0, _T("MapTool"));
	m_ToolList.InsertItem(1, _T("UnitTool"));

	m_ToolList.SetCurSel(0);

	//�ǿ� �� ���̾�α׵� �ʱ�ȭ
	CRect rect;
	m_ToolList.GetWindowRect(&rect);
	if (nullptr == m_MapTool.GetSafeHwnd())
	{
		m_MapTool.Create(IDD_MAPTOOL, &m_ToolList);
		m_MapTool.MoveWindow(0, 25, rect.Width(), rect.Height());
		m_MapTool.ShowWindow(SW_HIDE);
	}

	if (nullptr == m_UnitTool.GetSafeHwnd())
	{
		m_UnitTool.Create(IDD_UNITTOOL, &m_ToolList);
		m_UnitTool.MoveWindow(0, 25, rect.Width(), rect.Height());
		m_UnitTool.ShowWindow(SW_HIDE);
	}
}


void CMyForm::OnDraw(CDC* /*pDC*/)
{
	
}
CMapTool * CMyForm::GetMapTool()
{
	return &m_MapTool;
}
void CMyForm::OnTcnSelchangeToollist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iSel = m_ToolList.GetCurSel();

	switch (iSel)
	{
	case 0:
		m_MapTool.ShowWindow(SW_SHOW);
		m_UnitTool.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_MapTool.ShowWindow(SW_HIDE);
		m_UnitTool.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

	*pResult = 0;


}
