// ComponentSheet.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "ComponentSheet.h"

#include "GameObject.h"


#include "TextureRenderer.h"

// CComponentSheet

IMPLEMENT_DYNAMIC(CComponentSheet, CPropertySheet)

CComponentSheet::CComponentSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CComponentSheet::CComponentSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CComponentSheet::CComponentSheet(CWnd * pParentWnd)
{
	AddPage(&m_TexCompPage);
	AddPage(&m_BoxCompPage);
	AddPage(&m_AnimatorCompPage);
	AddPage(&m_ScriptCompPage);

	SetActivePage(&m_TexCompPage);
	SetActivePage(&m_BoxCompPage);
	SetActivePage(&m_AnimatorCompPage);
//	SetActivePage(&m_ScriptCompPage);
}

CComponentSheet::~CComponentSheet()
{
}


BEGIN_MESSAGE_MAP(CComponentSheet, CPropertySheet)
	ON_COMMAND(ID_32773, &CComponentSheet::OnRemove)
END_MESSAGE_MAP()


// CComponentSheet �޽��� ó�����Դϴ�.


void CComponentSheet::OnRemove()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	cout << "������Ʈ ����" << endl;
}


BOOL CComponentSheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CPropertySheet::OnCommand(wParam, lParam);
}

void CComponentSheet::UpdateInfo(CGameObject * pObject)
{
	m_TexCompPage.SetObject(pObject);
	m_AnimatorCompPage.SetObject(pObject);
}

void CComponentSheet::RemoveAll()
{
	int pageCount = GetPageCount();
	for (int i = 0; i < pageCount; ++i)
	{
		RemovePage(i);
	}
}
