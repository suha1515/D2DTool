// HierarchyView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "HierarchyView.h"
#include "afxdialogex.h"

#include "GameObject.h"

//툴뷰 화면 갱신
#include "MainFrm.h"
#include "MyToolView.h"
#include "InspectView.h"

// CHierarchyView 대화 상자입니다.
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

	//선택된 아이템이 있을경우. 자식으로 생성
	if (selItem != NULL)
	{
		CGameObject* parentObj= m_objectlist[selItem];
		int index =  parentObj->GetChildernVector().size() + 1;
		wstring objectName = parentObj->GetObjectName() +L"_child_"+to_wstring(index);
		object->SetObjectName(objectName);
		//부모설정.
		object->SetParentObject(parentObj);
		HTREEITEM item = m_Hierarchy.InsertItem(objectName.c_str(), 0, 0, selItem, TVI_LAST);
		m_objectlist.insert({ item,object});
		parentObj->GetChildernVector().push_back(object);

		//자식으로 생성시 1 반환.
		return 1;
	}
	//없을시 새로운 항목 생성.
	else
	{
		CString objectName = object->GetObjectName().c_str();

		HTREEITEM item = m_Hierarchy.InsertItem(objectName, 0, 0, TVI_ROOT, TVI_LAST);
		m_objectlist.insert({ item,object, });
		
		//루트로 생성시 0반환
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

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	//툴뷰 가져오기
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

	//클릭된것이 nullptr이 아니면 인스펙터뷰 갱신 null일경우는 항목 다비우기
		pInspectView->SetObject(m_CurClicked);
		pInspectView->UpdateInfo();
	
	if (m_CurClicked != m_PreClicked)
	{
		//오브젝트 클릭시 오브젝트 클릭 함수 실행
		m_CurClicked->SetObjectCliked(true);
		//선택 객체가 자식이 있을경우.
		if (m_CurClicked->GetChildernVector().size() > 0)
		{
			for (auto &i : m_CurClicked->GetChildernVector())
				i->SetObjectCliked(true, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		}
		//이전 클릭된 객체는 선택해제
		if (m_PreClicked != nullptr)
		{
			//이전 클릭도니 객체가 자식 최근 클릭된 객체의 자식인지검사
			bool check = false;;
			for (auto&i : m_CurClicked->GetChildernVector())
			{
				if (i == m_PreClicked)
					check = true;
			}
			if (!check)
			{	
				m_PreClicked->SetObjectCliked(false);
				//이전 클릭도니 객체가 자식을 가지고 있을경우
				if (m_PreClicked->GetChildernVector().size()>0)
				{
					for (auto &i : m_PreClicked->GetChildernVector())
					{
						//현재 선택된 객체 의외는 다 해제
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	HTREEITEM hItem = m_Hierarchy.GetSelectedItem();
	if (hItem != NULL)
	{
		CCameraMgr::GetInstance()->SetCameraPosition(m_objectlist[hItem]->GetComponent<CTransform>()->GetPosition());
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		//툴뷰 가져오기
		CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK_MSG(pMyToolView, L"Hierarchy tool view nullptr");
		pMyToolView->Invalidate(FALSE);
	}
}

void CHierarchyView::OnBnClickedDeselectAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM item = m_Hierarchy.GetSelectedItem();
	if (item != NULL)
	{
		m_CurClicked = nullptr;
		m_PreClicked = nullptr;
		m_Hierarchy.SetCheck(item, FALSE);
		m_Hierarchy.SelectItem(NULL);
	}
	cout << "포커스를 잃음" << endl;
}


//오브젝트 추가 하이라키 뷰를 통한 오브젝트 생성은 위치가 0,0,0 이온다.
void CHierarchyView::OnBnClickedAddObject()
{
	cout << "오브젝트 추가" << endl;
	wstring name = L"GameObject" + to_wstring(CObjectMgr::GetInstance()->GetRootObjectCount());

	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	pGameObject->SetObjectName(name);

	//트랜스폼 컴포넌트
	CTransform* pTransform = new CTransform;
	pTransform->Initialize();
	pTransform->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	pGameObject->AddComponent(pTransform);

	//0을 반환한 경우는 부모로생성하는것
	if (AddObject(pGameObject) == 0)
	{
		CObjectMgr::GetInstance()->AddObject(pGameObject);
	}

}

//오브젝트 삭제.
void CHierarchyView::OnBnClickedDeleteObject()
{
	HTREEITEM selItem = m_Hierarchy.GetSelectedItem();

	if (NULL != selItem)
	{
		//자식이 있을경우 자식 다삭제.
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
		// 부모가있을경우.
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
