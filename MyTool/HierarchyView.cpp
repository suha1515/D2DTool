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
#include "TextureRenderer.h"
#include "Animator.h"
#include "BoxCollider.h"
#include "Scripts.h"

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
		CGameObject* parentObj = m_objectlist.find(selItem)->second;
		int index = parentObj->GetChildernVector().size() + 1;
		wstring objectName = parentObj->GetObjectName() + L"_child_" + to_wstring(index);
		object->SetObjectName(objectName);

		HTREEITEM item = m_Hierarchy.InsertItem(objectName.c_str(), 0, 0, selItem, TVI_LAST);
		m_objectlist.insert({ item,object });

		//부모설정.
		object->SetParentObject(parentObj);
		//부모객체에 등록.
		parentObj->GetChildernVector().push_back(object);
		//부모객체보다 높은 계층에 존재
		object->SetObjectLevel(parentObj->GetLevel() + 1);
		//부모객체 기준으로 좌표계 설정
		D3DXVECTOR3 pos =object->GetComponent<CTransform>()->GetPosition();
		D3DXMATRIX parentMat = parentObj->GetComponent<CTransform>()->GetWorldMat();
		//월드행렬의 역행렬은 전치행렬이므로. 전치화
		D3DXMatrixInverse(&parentMat, 0, &parentMat);

		//D3DXMatrixTranspose(&parentMat, &parentMat);
		
		D3DXVECTOR4 newPos;
		D3DXVec3Transform(&newPos, &pos, &parentMat);

		object->GetComponent<CTransform>()->SetPosition(D3DXVECTOR3(newPos.x, newPos.y, newPos.z));

		//자식으로 생성시 1 반환.
		return 1;
	}
	//없을시 새로운 항목 생성.
	else
	{
		CString objectName = object->GetObjectName().c_str();

		//계층은 0이다.
		object->SetObjectLevel(0);

		HTREEITEM item = m_Hierarchy.InsertItem(objectName, 0, 0, TVI_ROOT, TVI_LAST);
		m_objectlist.insert({ item,object, });

		//루트로 생성시 0반환
		return 0;
	}
}

int CHierarchyView::AddCopyObject(const OBJ_COPY* copy,const wstring& name)
{
	Clear();
	CObjectMgr::GetInstance()->MakeObjectByCopy(copy, name, nullptr);
	LoadObject();
	return 0;
}

void CHierarchyView::AddList(HTREEITEM * item, const OBJ_COPY * copy, const wstring& name,CGameObject* parent)
{
	//CString ObjectName = name.c_str();
	//HTREEITEM _item;
	//if(item==nullptr)
	//	_item = m_Hierarchy.InsertItem(ObjectName, 0, 0, TVI_ROOT, TVI_LAST);
	//else
	//{
	//	_item = m_Hierarchy.InsertItem(ObjectName, 0, 0, *item, TVI_LAST);
	//}

	//CGameObject* pGameObject = new CGameObject;

	////오브젝트 정보넣기
	//if (name == L"")
	//	pGameObject->SetObjectName(copy->objInfo._ObjectName);
	//else
	//	pGameObject->SetObjectName(name);		//새로운 이름을 넣어도된다.
	//pGameObject->SetObjectLayer(copy->objInfo._ObjectLayer);
	//pGameObject->SetObjectLevel(copy->objInfo._ObjectLevel);
	//pGameObject->SetObjectTag(copy->objInfo._ObjectTag);

	//if (parent != nullptr)
	//{
	//	pGameObject->SetParentObject(parent);
	//	parent->GetChildernVector().push_back(pGameObject);
	//}
	////Comp정보참조
	//if (copy->compInfo._Transform == 1)
	//{
	//	CTransform* pTransform = new CTransform;
	//	pTransform->Initialize(pGameObject);
	//	pTransform->SetPosition(copy->transformInfo._ObjectPos);
	//	pTransform->SetRotation(copy->transformInfo._ObjectRotation);
	//	pTransform->SetScaling(copy->transformInfo._ObjectScale);

	//	pGameObject->AddComponent(pTransform);
	//}
	//if (copy->compInfo._Texture == 1)
	//{
	//	CTextureRenderer* pTexture = new CTextureRenderer;
	//	pTexture->Initialize(pGameObject);
	//	pTexture->SetTexture(copy->textureInfo._TextrueName);
	//	pTexture->SetTexSize(copy->textureInfo._TextureSize);
	//	pTexture->SetTexPos(copy->textureInfo._TexturPos);

	//	pGameObject->AddComponent(pTexture);
	//}
	//if (copy->compInfo._BoxCol == 1)
	//{
	//	CBoxCollider* pBoxCollider = new CBoxCollider;
	//	pBoxCollider->Initialize(pGameObject);
	//	pBoxCollider->SetBoxSize(copy->boxcolInfo._BoxWidth, copy->boxcolInfo._BoxHeight);
	//	pBoxCollider->SetBoxOffset(copy->boxcolInfo._BoxOffsetX, copy->boxcolInfo._BoxOffsetY);

	//	pGameObject->AddComponent(pBoxCollider);
	//}
	//if (copy->compInfo._Animator == 1)
	//{
	//	CAnimator* pAnimator = new CAnimator;
	//	pAnimator->Initialize(pGameObject);
	//	pAnimator->LoadClips(copy->animInfo._AnimationName);

	//	pGameObject->AddComponent(pAnimator);
	//}

	////스크립트
	//if (copy->scriptInfo.size() > 0)
	//{
	//	for (auto&i : copy->scriptInfo)
	//	{
	//		CScriptMgr::GetInstance()->LoadScripts(i._ScriptName, pGameObject);
	//	}
	//}
	////자식이 있을경우
	//if (copy->childInfo.size() > 0)
	//{
	//	for (auto&i : copy->childInfo)
	//		AddList(&_item, &i, i.objInfo._ObjectName, pGameObject);
	//}
	//m_objectlist.insert({ _item,pGameObject });
	//AddObject(pGameObject);
}

void CHierarchyView::LoadObject()
{
	const map<int, vector<CGameObject*>>& temp = CObjectMgr::GetInstance()->GetObjects();
	for (size_t i = 0; i < temp.size(); ++i)
	{
		for (auto&j : temp.find(i)->second)
		{
			if (j->GetParentObject() != nullptr)
			{
				CString parentName = j->GetParentObject()->GetObjectName().c_str();
				CString childName = j->GetObjectName().c_str();
				auto iter_find = find_if(m_objectlist.begin(), m_objectlist.end(),
					[&](std::pair<HTREEITEM,CGameObject*>item)
					{
						CString itemName = m_Hierarchy.GetItemText(item.first);
						if (itemName == parentName)
							return true;
						return false;
					}
				);
				HTREEITEM parentItem = (*iter_find).first;

				HTREEITEM childItem = m_Hierarchy.InsertItem(childName, 0, 0, parentItem, TVI_LAST);
				m_objectlist.insert({ childItem ,j});
			}
			else
			{
				CString objectName = j->GetObjectName().c_str();
				//계층은 0이다.
				HTREEITEM item = m_Hierarchy.InsertItem(objectName, 0, 0, TVI_ROOT, TVI_LAST);
				m_objectlist.insert({ item,j});

			}
		}
	}

}

void CHierarchyView::Clear()
{
	m_objectlist.clear();
	m_CurClicked = nullptr;
	m_PreClicked = nullptr;

	m_Hierarchy.DeleteAllItems();
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
	cout << m_objectlist.size() << endl;

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
				//이전 클릭된 객체가 자식을 가지고 있을경우
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
		CCameraMgr::GetInstance()->SetCameraPosition(m_objectlist.find(hItem)->second->GetComponent<CTransform>()->GetPosition());
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		//툴뷰 가져오기
		CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK_MSG(pMyToolView, L"Hierarchy tool view nullptr");
		pMyToolView->Invalidate(FALSE);

		CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
		NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");

		pInspectView->UpdateInfo();
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
	wstring name = L"GameObject" + to_wstring(CObjectMgr::GetInstance()->GetObjectCount());

	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	pGameObject->SetObjectName(name);

	//트랜스폼 컴포넌트
	CTransform* pTransform = new CTransform;
	pTransform->Initialize(pGameObject);
	pTransform->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	pGameObject->AddComponent(pTransform);

	AddObject(pGameObject);
	CObjectMgr::GetInstance()->AddObject(pGameObject);
	
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
				m_objectlist.erase(hChildItem);
				hChildItem = hNextItem;
			}
		}
		//해당 객체는 제거 메시지를 보낸다. 부모와 자식이 있을경우 오브젝트 매니저에서처리.
		m_objectlist.find(selItem)->second->SetObjectDestroy(true);
		m_objectlist.erase(selItem);
		m_Hierarchy.DeleteItem(selItem);

		

		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		//툴뷰 가져오기
		CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK_MSG(pMyToolView, L"Hierarchy tool view nullptr");
		pMyToolView->Invalidate(FALSE);

		CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
		NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");

		if (m_Hierarchy.GetCount() == 0)
		{
			pInspectView->SetObject(nullptr);
			pInspectView->UpdateInfo();
		}
	}
}
