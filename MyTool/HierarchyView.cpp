// HierarchyView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "HierarchyView.h"
#include "afxdialogex.h"

#include "GameObject.h"


// CHierarchyView ��ȭ �����Դϴ�.

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

	//���õ� �������� �������. �ڽ����� ����
	if (selItem != NULL)
	{
		CGameObject* parentObj= m_objectlist[selItem];
		int index =  parentObj->GetChildernVector().size() + 1;
		wstring objectName = parentObj->GetObjectName() +L"_child_"+to_wstring(index);
		HTREEITEM item = m_Hierarchy.InsertItem(objectName.c_str(), 0, 0, selItem, TVI_LAST);
		m_objectlist.insert({ item,object});
		parentObj->GetChildernVector().push_back(object);
	}
	//������ ���ο� �׸� ����.
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


// CHierarchyView �޽��� ó�����Դϴ�.


BOOL CHierarchyView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	/*HTREEITEM root, hChild1, hChild2,root2;

	root    = m_Hierarchy.InsertItem(L"root", 0, 0, TVI_ROOT, TVI_LAST);
	root2   = m_Hierarchy.InsertItem(L"Third", 0, 0, TVI_ROOT, TVI_LAST);
	hChild1 = m_Hierarchy.InsertItem(L"First", 0, 0, root, TVI_LAST);
	hChild2 = m_Hierarchy.InsertItem(L"Second", 0, 0, root, TVI_LAST);
	m_Hierarchy.InsertItem(L"Forth", 0, 0, root2, TVI_LAST);*/
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CHierarchyView::OnNMClickHierarchytree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	//HTREEITEM hTreeItem = m_Hierarchy.GetSelectedItem();

	//wcout << m_Hierarchy.GetItemText(hTreeItem).operator LPCWSTR() << endl;
}


void CHierarchyView::OnTvnSelchangedHierarchytree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	HTREEITEM hTreeItem = m_Hierarchy.GetSelectedItem();

	wcout << m_Hierarchy.GetItemText(hTreeItem).GetString() << endl;
}


void CHierarchyView::OnNMDblclkHierarchytree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}
