// ComponentSheet.cpp : 구현 파일입니다.
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


// CComponentSheet 메시지 처리기입니다.


void CComponentSheet::OnRemove()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	cout << "컴포넌트 삭제" << endl;
}


BOOL CComponentSheet::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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
