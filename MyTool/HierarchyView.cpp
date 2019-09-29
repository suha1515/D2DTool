// HierarchyView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "HierarchyView.h"
#include "afxdialogex.h"

#include "GameObject.h"


// CHierarchyView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHierarchyView, CDialogEx)

CHierarchyView::CHierarchyView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HIERARCHY, pParent)
{

}

CHierarchyView::~CHierarchyView()
{
}

void CHierarchyView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HierarchyTree, m_Hierarchy);
}

void CHierarchyView::AddObject(CGameObject * object)
{
	HTREEITEM selItem = m_Hierarchy.GetSelectedItem();

	//선택된 아이템이 있을경우. 자식으로 생성
	if (selItem != NULL)
	{
		CGameObject* parentObj= m_objectlist[selItem];
		int index =  parentObj->GetChildernVector().size() + 1;
		wstring objectName = parentObj->GetObjectName() +L"_child_"+to_wstring(index);
		HTREEITEM item = m_Hierarchy.InsertItem(objectName.c_str(), 0, 0, selItem, TVI_LAST);
		m_objectlist.insert({ item,object});
		parentObj->GetChildernVector().push_back(object);
	}
	//없을시 새로운 항목 생성.
	else
	{
		CString objectName = object->GetObjectName().c_str();

		HTREEITEM item = m_Hierarchy.InsertItem(objectName, 0, 0, TVI_ROOT, TVI_LAST);
		m_objectlist.insert({ item,object, });
	}
	
	//m_objectlist.push_back(make_pair(object,item));

	
}


BEGIN_MESSAGE_MAP(CHierarchyView, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_HierarchyTree, &CHierarchyView::OnNMClickHierarchytree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_HierarchyTree, &CHierarchyView::OnTvnSelchangedHierarchytree)
	ON_NOTIFY(NM_DBLCLK, IDC_HierarchyTree, &CHierarchyView::OnNMDblclkHierarchytree)
END_MESSAGE_MAP()


// CHierarchyView 메시지 처리기입니다.


BOOL CHierarchyView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	/*HTREEITEM root, hChild1, hChild2,root2;

	root    = m_Hierarchy.InsertItem(L"root", 0, 0, TVI_ROOT, TVI_LAST);
	root2   = m_Hierarchy.InsertItem(L"Third", 0, 0, TVI_ROOT, TVI_LAST);
	hChild1 = m_Hierarchy.InsertItem(L"First", 0, 0, root, TVI_LAST);
	hChild2 = m_Hierarchy.InsertItem(L"Second", 0, 0, root, TVI_LAST);
	m_Hierarchy.InsertItem(L"Forth", 0, 0, root2, TVI_LAST);*/
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CHierarchyView::OnNMClickHierarchytree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	//HTREEITEM hTreeItem = m_Hierarchy.GetSelectedItem();

	//wcout << m_Hierarchy.GetItemText(hTreeItem).operator LPCWSTR() << endl;
}


void CHierarchyView::OnTvnSelchangedHierarchytree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	HTREEITEM hTreeItem = m_Hierarchy.GetSelectedItem();

	wcout << m_Hierarchy.GetItemText(hTreeItem).GetString() << endl;
}


void CHierarchyView::OnNMDblclkHierarchytree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
