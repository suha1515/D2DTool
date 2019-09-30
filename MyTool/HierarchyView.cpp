// HierarchyView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "HierarchyView.h"
#include "afxdialogex.h"

#include "GameObject.h"

//���� ȭ�� ����
#include "MainFrm.h"
#include "MyToolView.h"
#include "InspectView.h"

// CHierarchyView ��ȭ �����Դϴ�.
#include "Transform.h"

IMPLEMENT_DYNAMIC(CHierarchyView, CDialogEx)

CHierarchyView::CHierarchyView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HIERARCHY, pParent)
{
	m_CurClicked = nullptr;
	m_PreClicked = nullptr;
}

CHierarchyView::~CHierarchyView()
{
	
}

void CHierarchyView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HierarchyTree, m_Hierarchy);
}

int CHierarchyView::AddObject(CGameObject * object)
{
	HTREEITEM selItem = m_Hierarchy.GetSelectedItem();

	//���õ� �������� �������. �ڽ����� ����
	if (selItem != NULL)
	{
		CGameObject* parentObj= m_objectlist[selItem];
		int index =  parentObj->GetChildernVector().size() + 1;
		wstring objectName = parentObj->GetObjectName() +L"_child_"+to_wstring(index);
		object->SetObjectName(objectName);
		//�θ���.
		object->SetParentObject(parentObj);
		HTREEITEM item = m_Hierarchy.InsertItem(objectName.c_str(), 0, 0, selItem, TVI_LAST);
		m_objectlist.insert({ item,object});
		parentObj->GetChildernVector().push_back(object);

		//�ڽ����� ������ 1 ��ȯ.
		return 1;
	}
	//������ ���ο� �׸� ����.
	else
	{
		CString objectName = object->GetObjectName().c_str();

		HTREEITEM item = m_Hierarchy.InsertItem(objectName, 0, 0, TVI_ROOT, TVI_LAST);
		m_objectlist.insert({ item,object, });
		
		//��Ʈ�� ������ 0��ȯ
		return 0;
	}
	
	//m_objectlist.push_back(make_pair(object,item));

	
}

void CHierarchyView::Update()
{
	for (auto&i : m_objectlist)
	{
		CString name = i.second->GetObjectName().c_str();
		m_Hierarchy.SetItemText(i.first, name);
	}
}


BEGIN_MESSAGE_MAP(CHierarchyView, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_HierarchyTree, &CHierarchyView::OnNMClickHierarchytree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_HierarchyTree, &CHierarchyView::OnTvnSelchangedHierarchytree)
	ON_NOTIFY(NM_DBLCLK, IDC_HierarchyTree, &CHierarchyView::OnNMDblclkHierarchytree)
	ON_BN_CLICKED(IDC_BUTTON1, &CHierarchyView::OnBnClickedDeselectAll)
	ON_BN_CLICKED(IDC_BUTTON2, &CHierarchyView::OnBnClickedAddObject)
	ON_BN_CLICKED(IDC_BUTTON4, &CHierarchyView::OnBnClickedDeleteObject)
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

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	//���� ��������
	CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_MSG(pMyToolView, L"Hierarchy tool view nullptr");

	CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
	NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");

	HTREEITEM hTreeItem = m_Hierarchy.GetSelectedItem();

	auto& iter_find = m_objectlist.find(hTreeItem);
	if (iter_find == m_objectlist.end())
		m_CurClicked = nullptr;
	else
		m_CurClicked = iter_find->second;

	//Ŭ���Ȱ��� nullptr�� �ƴϸ� �ν����ͺ� ���� null�ϰ��� �׸� �ٺ���
		pInspectView->SetObject(m_CurClicked);
		pInspectView->UpdateInfo();
	
	if (m_CurClicked != m_PreClicked)
	{
		//������Ʈ Ŭ���� ������Ʈ Ŭ�� �Լ� ����
		m_CurClicked->SetObjectCliked(true);
		//���� ��ü�� �ڽ��� �������.
		if (m_CurClicked->GetChildernVector().size() > 0)
		{
			for (auto &i : m_CurClicked->GetChildernVector())
				i->SetObjectCliked(true, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
		//���� Ŭ���� ��ü�� ��������
		if (m_PreClicked != nullptr)
		{
			//���� Ŭ������ ��ü�� �ڽ� �ֱ� Ŭ���� ��ü�� �ڽ������˻�
			bool check = false;;
			for (auto&i : m_CurClicked->GetChildernVector())
			{
				if (i == m_PreClicked)
					check = true;
			}
			if (!check)
			{	
				m_PreClicked->SetObjectCliked(false);
				//���� Ŭ������ ��ü�� �ڽ��� ������ �������
				if (m_PreClicked->GetChildernVector().size()>0)
				{
					for (auto &i : m_PreClicked->GetChildernVector())
					{
						//���� ���õ� ��ü �ǿܴ� �� ����
						if (i != m_CurClicked)
							i->SetObjectCliked(false);
					}
				}
			}
		}
		
		m_PreClicked = m_CurClicked;	
		pMyToolView->Invalidate(FALSE);	
	}

	wcout << m_Hierarchy.GetItemText(hTreeItem).GetString() << endl;
}


void CHierarchyView::OnNMDblclkHierarchytree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	HTREEITEM hItem = m_Hierarchy.GetSelectedItem();
	if (hItem != NULL)
	{
		CCameraMgr::GetInstance()->SetCameraPosition(m_objectlist[hItem]->GetComponent<CTransform>()->GetPosition());
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		//���� ��������
		CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK_MSG(pMyToolView, L"Hierarchy tool view nullptr");
		pMyToolView->Invalidate(FALSE);
	}
}

void CHierarchyView::OnBnClickedDeselectAll()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	HTREEITEM item = m_Hierarchy.GetSelectedItem();
	if (item != NULL)
	{
		m_CurClicked = nullptr;
		m_PreClicked = nullptr;
		m_Hierarchy.SetCheck(item, FALSE);
		m_Hierarchy.SelectItem(NULL);
	}
	cout << "��Ŀ���� ����" << endl;
}


//������Ʈ �߰� ���̶�Ű �並 ���� ������Ʈ ������ ��ġ�� 0,0,0 �̿´�.
void CHierarchyView::OnBnClickedAddObject()
{
	cout << "������Ʈ �߰�" << endl;
	wstring name = L"GameObject" + to_wstring(CObjectMgr::GetInstance()->GetRootObjectCount());

	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	pGameObject->SetObjectName(name);

	//Ʈ������ ������Ʈ
	CTransform* pTransform = new CTransform;
	pTransform->Initialize();
	pTransform->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	pGameObject->AddComponent(pTransform);

	//0�� ��ȯ�� ���� �θ�λ����ϴ°�
	if (AddObject(pGameObject) == 0)
	{
		CObjectMgr::GetInstance()->AddObject(pGameObject);
	}

}

//������Ʈ ����.
void CHierarchyView::OnBnClickedDeleteObject()
{
	HTREEITEM selItem = m_Hierarchy.GetSelectedItem();

	if (NULL != selItem)
	{
		//�ڽ��� ������� �ڽ� �ٻ���.
		if (m_Hierarchy.ItemHasChildren(selItem))
		{
			HTREEITEM hNextItem;
			HTREEITEM hChildItem = m_Hierarchy.GetChildItem(selItem);
			while (NULL != hChildItem)
			{
				hNextItem = m_Hierarchy.GetNextItem(hChildItem, TVGN_NEXT);
				m_Hierarchy.DeleteItem(hChildItem);
				hChildItem = hNextItem;
			}
		}
		// �θ��������.
		HTREEITEM hParent;
		hParent = m_Hierarchy.GetNextItem(selItem, TVGN_PARENT);
		if (NULL!=hParent)
		{
			CGameObject* pParentObject = m_objectlist.find(hParent)->second;
			CGameObject* pChildObject = m_objectlist.find(selItem)->second;
			vector<CGameObject*> vecChild = pParentObject->GetChildernVector();
			vector<CGameObject*>::iterator iter_begin = vecChild.begin();
			vector<CGameObject*>::iterator iter_end = vecChild.end();

			for (; iter_begin != iter_end;)
			{
				if ((*iter_begin) == pChildObject)
				{
					pChildObject->SetObjectDestroy(true);
					vecChild.erase(iter_begin);
					break;
				}
			}
		}
		else
		{
			SafeDelete(m_objectlist[selItem]);
			m_objectlist.erase(selItem);
		}
		m_Hierarchy.DeleteItem(selItem);
	}
}
