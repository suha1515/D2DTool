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

	//���õ� �������� �������. �ڽ����� ����
	if (selItem != NULL)
	{
		CGameObject* parentObj = m_objectlist.find(selItem)->second;
		int index = parentObj->GetChildernVector().size() + 1;
		wstring objectName = parentObj->GetObjectName() + L"_child_" + to_wstring(index);
		object->SetObjectName(objectName);

		HTREEITEM item = m_Hierarchy.InsertItem(objectName.c_str(), 0, 0, selItem, TVI_LAST);
		m_objectlist.insert({ item,object });

		//�θ���.
		object->SetParentObject(parentObj);
		//�θ�ü�� ���.
		parentObj->GetChildernVector().push_back(object);
		//�θ�ü���� ���� ������ ����
		object->SetObjectLevel(parentObj->GetLevel() + 1);
		//�θ�ü �������� ��ǥ�� ����
		D3DXVECTOR3 pos =object->GetComponent<CTransform>()->GetPosition();
		D3DXMATRIX parentMat = parentObj->GetComponent<CTransform>()->GetWorldMat();
		//��������� ������� ��ġ����̹Ƿ�. ��ġȭ
		D3DXMatrixInverse(&parentMat, 0, &parentMat);

		//D3DXMatrixTranspose(&parentMat, &parentMat);
		
		D3DXVECTOR4 newPos;
		D3DXVec3Transform(&newPos, &pos, &parentMat);

		object->GetComponent<CTransform>()->SetPosition(D3DXVECTOR3(newPos.x, newPos.y, newPos.z));

		//�ڽ����� ������ 1 ��ȯ.
		return 1;
	}
	//������ ���ο� �׸� ����.
	else
	{
		CString objectName = object->GetObjectName().c_str();

		//������ 0�̴�.
		object->SetObjectLevel(0);

		HTREEITEM item = m_Hierarchy.InsertItem(objectName, 0, 0, TVI_ROOT, TVI_LAST);
		m_objectlist.insert({ item,object, });

		//��Ʈ�� ������ 0��ȯ
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

	////������Ʈ �����ֱ�
	//if (name == L"")
	//	pGameObject->SetObjectName(copy->objInfo._ObjectName);
	//else
	//	pGameObject->SetObjectName(name);		//���ο� �̸��� �־�ȴ�.
	//pGameObject->SetObjectLayer(copy->objInfo._ObjectLayer);
	//pGameObject->SetObjectLevel(copy->objInfo._ObjectLevel);
	//pGameObject->SetObjectTag(copy->objInfo._ObjectTag);

	//if (parent != nullptr)
	//{
	//	pGameObject->SetParentObject(parent);
	//	parent->GetChildernVector().push_back(pGameObject);
	//}
	////Comp��������
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

	////��ũ��Ʈ
	//if (copy->scriptInfo.size() > 0)
	//{
	//	for (auto&i : copy->scriptInfo)
	//	{
	//		CScriptMgr::GetInstance()->LoadScripts(i._ScriptName, pGameObject);
	//	}
	//}
	////�ڽ��� �������
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
				//������ 0�̴�.
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


// CHierarchyView �޽��� ó�����Դϴ�.


BOOL CHierarchyView::OnInitDialog()
{
	CDialogEx::OnInitDialog();


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
	cout << m_objectlist.size() << endl;

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
				//���� Ŭ���� ��ü�� �ڽ��� ������ �������
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
		CCameraMgr::GetInstance()->SetCameraPosition(m_objectlist.find(hItem)->second->GetComponent<CTransform>()->GetPosition());
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		//���� ��������
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
	wstring name = L"GameObject" + to_wstring(CObjectMgr::GetInstance()->GetObjectCount());

	CGameObject* pGameObject = new CGameObject;
	pGameObject->Initialize();
	pGameObject->SetObjectName(name);

	//Ʈ������ ������Ʈ
	CTransform* pTransform = new CTransform;
	pTransform->Initialize(pGameObject);
	pTransform->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	pGameObject->AddComponent(pTransform);

	AddObject(pGameObject);
	CObjectMgr::GetInstance()->AddObject(pGameObject);
	
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
				m_objectlist.erase(hChildItem);
				hChildItem = hNextItem;
			}
		}
		//�ش� ��ü�� ���� �޽����� ������. �θ�� �ڽ��� ������� ������Ʈ �Ŵ�������ó��.
		m_objectlist.find(selItem)->second->SetObjectDestroy(true);
		m_objectlist.erase(selItem);
		m_Hierarchy.DeleteItem(selItem);

		

		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		//���� ��������
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
