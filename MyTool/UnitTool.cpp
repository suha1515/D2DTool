// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


#include "MainFrm.h"
#include "MyToolView.h"

//���� ȭ�� ����
#include "MainFrm.h"
#include "MyToolView.h"
#include "InspectView.h"
#include "HierarchyView.h"

#include "GameObject.h"
#include "Scripts.h"
#include "TextureRenderer.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Animator.h"

IMPLEMENT_DYNAMIC(CUnitTool, CDialogEx)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UNITTOOL, pParent)
	, m_Search(_T(""))
	, m_name(_T(""))
	, m_CopyName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Search);
	DDX_Text(pDX, IDC_EDIT2, m_name);
	DDX_Control(pDX, IDC_LIST1, m_ObjectList);
	DDX_Text(pDX, IDC_EDIT3, m_CopyName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialogEx)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedAddObject)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnBnClickedSaveList)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnBnClickedLoadObject)
	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnBnClickedToHierarchy)
	ON_BN_CLICKED(IDC_BUTTON3, &CUnitTool::OnBnClickedRemove)
END_MESSAGE_MAP()



void CUnitTool::OnBnClickedAddObject()
{
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	//���� ��������
	CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_MSG(pMyToolView, L"Hierarchy tool view nullptr");

	CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
	NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");


	//���̶�Ű�信�� ���õ� ������Ʈ�� �����´�.
	CGameObject* pObject = pInspectView->m_HierarchyView.m_CurClicked;
	if (pObject != nullptr)
	{
		UpdateData(TRUE);
		//������Ʈ�̸�
		CString objectName = pObject->GetObjectName().c_str();

		auto iter_find = m_Units.find(m_CopyName.operator LPCWSTR());
		//�̹� ����Ʈ�� ������Ʈ�� �����Ƿ� �ѱ��
		if (m_Units.end() != iter_find)
			return;

		OBJ_COPY copy;
		copy = MakeCopy(pObject);
		
		m_Units.insert({ m_CopyName.operator LPCWSTR(),copy });
		m_ObjectList.AddString(m_CopyName);
		
		UpdateData(FALSE);
		
	}

}

OBJ_COPY CUnitTool::MakeCopy(CGameObject * pObject)
{
	OBJ_COPY copyObject;
	TCHAR	tmp[MAX_STR] = L"";
	OBJ_INFO		objInfo;
	TRANSFORM_INFO  transInfo;
	TEXTURE_INFO	textureInfo;
	BOXCOL_INFO		boxcolInfo;
	ANIM_INFO		animInfo;
	SCRIPT_INFO		scriptInfo;
	COMP_INFO compInfo;
	CTransform* pTransform = pObject->GetComponent<CTransform>();
	CTextureRenderer* pTexture = pObject->GetComponent<CTextureRenderer>();
	CBoxCollider* pBoxCollider = pObject->GetComponent<CBoxCollider>();
	CAnimator*  pAnimator = pObject->GetComponent<CAnimator>();
	int scriptSize = pObject->GetScripts().size();
	//������Ʈ ������ �ֱ����� � ������Ʈ�� �ִ��� �˷��ش�.
	if (nullptr != pTransform)
		compInfo._Transform = 1;
	if (nullptr != pTexture)
		compInfo._Texture = 1;
	if (nullptr != pBoxCollider)
		compInfo._BoxCol = 1;
	if (nullptr != pAnimator)
		compInfo._Animator = 1;
	
	copyObject.compInfo = compInfo;

	//������Ʈ ���̾�����
	objInfo._ObjectLayer = pObject->GetObjectLayer();
	//������Ʈ ��������
	objInfo._ObjectLevel = pObject->GetObjectLevel();
	//������Ʈ �̸�
	lstrcpy(objInfo._ObjectName, pObject->GetObjectName().c_str());
	//������Ʈ �±�
	lstrcpy(objInfo._ObjectTag, pObject->GetObjectTag().c_str());
	//�θ� ������Ʈ�̸�
	if (pObject->GetParentObject() != nullptr)
		lstrcpy(objInfo._ParentObject, pObject->GetParentObject()->GetObjectName().c_str());
	else
		lstrcpy(objInfo._ParentObject, L"");

	copyObject.objInfo = objInfo;

	//Ʈ������ ������Ʈ ���� ����
	if (nullptr != pTransform)										//null�� �ƴϸ� ������ ����.
	{
		transInfo._ObjectPos = pTransform->GetLocalPosition();			//��ġ
		transInfo._ObjectRotation = pTransform->GetRotation();		//ȸ��
		transInfo._ObjectScale = pTransform->GetScale();			//ũ��
		copyObject.transformInfo = transInfo;
	}
	//�ؽ�ó ������Ʈ ���� ����
	if (nullptr != pTexture)
	{
		lstrcpy(textureInfo._TextrueName, pTexture->GetTexName().c_str());		//�ؽ�ó�̸�
		textureInfo._TextureSize = pTexture->GetTexSize();						//�ؽ�óũ��
		textureInfo._TexturPos[0] = pTexture->GetTexPos()[0];					//�ؽ�ó ��ǥ
		textureInfo._TexturPos[1] = pTexture->GetTexPos()[1];
		textureInfo._TexturPos[2] = pTexture->GetTexPos()[2];
		textureInfo._TexturPos[3] = pTexture->GetTexPos()[3];
		copyObject.textureInfo = textureInfo;
	}
	//�ڽ� �ݶ��̴� ���� ����
	if (nullptr != pBoxCollider)
	{
		boxcolInfo._BoxHeight = pBoxCollider->GetBoxHeight();				//�ڽ�����
		boxcolInfo._BoxWidth = pBoxCollider->GetBoxWidth();					//�ڽ��ʺ�
		boxcolInfo._BoxOffsetX = pBoxCollider->GetBoxOffsetX();				//������X
		boxcolInfo._BoxOffsetY = pBoxCollider->GetBoxoffsetY();				//������Y
		boxcolInfo._colType = pBoxCollider->GetCollideType();				//�ݶ��̴� Ÿ��
		copyObject.boxcolInfo = boxcolInfo;
	}
	//�ִϸ����� ���� ����
	if (nullptr != pAnimator)
	{
		lstrcpy(animInfo._AnimationName, pAnimator->GetAnimationName().c_str());
		copyObject.animInfo = animInfo;
	}
	//��ũ��Ʈ ���� ����
	if (scriptSize > 0)
	{
		for (auto&k : pObject->GetScripts())
		{
			SCRIPT_INFO		scriptInfo;
			lstrcpy(scriptInfo._ScriptName, strings::widen(k.first).c_str());
			copyObject.scriptInfo.push_back(scriptInfo);
		}
	}

	int childSize = pObject->GetChildernVector().size();
	if (childSize > 0)
	{
		for (auto&j : pObject->GetChildernVector())
		{
			OBJ_COPY childCopy;
			childCopy = MakeCopy(j);
			copyObject.childInfo.push_back(childCopy);
		}
	}
	return copyObject;
}

void CUnitTool::WriteCopy(CFile* pFile, OBJ_COPY* copy)
{
	pFile->Write(&copy->compInfo, sizeof(COMP_INFO));
	pFile->Write(&copy->objInfo, sizeof(OBJ_INFO));
	if (copy->compInfo._Transform == 1)
	{
		pFile->Write(&copy->transformInfo, sizeof(TRANSFORM_INFO));
	}
	if (copy->compInfo._Texture == 1)
	{
		pFile->Write(&copy->textureInfo, sizeof(TEXTURE_INFO));
	}
	if (copy->compInfo._BoxCol == 1)
	{
		pFile->Write(&copy->boxcolInfo, sizeof(BOXCOL_INFO));
	}
	if (copy->compInfo._Animator == 1)
	{
		pFile->Write(&copy->animInfo, sizeof(ANIM_INFO));
	}
	int scriptSize = 0;
	scriptSize = copy->scriptInfo.size();
	pFile->Write(&scriptSize, sizeof(int));
	for (auto& j : copy->scriptInfo)
		pFile->Write(&j, sizeof(SCRIPT_INFO));

	int childSize = copy->childInfo.size();
	pFile->Write(&childSize, sizeof(int));
	if (childSize > 0)
	{
		for (auto&i : copy->childInfo)
			WriteCopy(pFile, &i);
	}
}



void CUnitTool::OnBnClickedSaveList()
{
	//����� ���ֵ��� ������ �����Ѵ�.
	//���߿� ������ �ҷ��� �� �ִ�.


	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString m_strPath;
	CFile file;
	CFileException ex;
	CFileDialog dlg(FALSE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("DataFile(*.dat)|*.dat|"), NULL);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\ObjectList");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	if (dlg.DoModal() == IDOK)
	{
		m_strPath = dlg.GetPathName();
		if (m_strPath.Right(4) != ".dat")
		{
			m_strPath += ".dat";
		}
		TCHAR name[MAX_STR] = L"";
		file.Open(m_strPath, CFile::modeCreate | CFile::modeReadWrite, &ex);
		for (auto&i : m_Units)
		{
			lstrcpy(name, i.first.c_str());
			file.Write(name, sizeof(name));
			WriteCopy(&file,&i.second);
		}			
		file.Close();

	}
}


void CUnitTool::OnBnClickedLoadObject()
{
	CStdioFile	file;
	CFileException kEx;
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_FILEMUSTEXIST, _T("DataFile(*.dat)|*.dat|"), NULL);
	CString m_strPath;

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\ObjectList");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	if (dlg.DoModal() == IDOK)
	{
		CObjectMgr::GetInstance()->ClearCopy();
		m_ObjectList.ResetContent();
		//���� �̸� ���
		m_strPath = dlg.GetPathName();
	
		m_ObjectList.ResetContent();
		if (!m_Units.empty())
			m_Units.clear();
			
		CObjectMgr::GetInstance()->LoadCopyObject(m_strPath.operator LPCWSTR());

		auto& copyObjects = CObjectMgr::GetInstance()->GetCopyObject();
		for (auto& i  : copyObjects)
		{
			m_ObjectList.AddString(i.first.c_str());
			m_Units.insert({ i.first,i.second });
		}
	
		MessageBox(L"������Ʈ������ ���������� �ҷ���", L"Success");
	}
}


void CUnitTool::OnBnClickedToHierarchy()
{
	UpdateData(TRUE);
	int index = m_ObjectList.GetCurSel();
	if (index != -1)
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		//���� ��������
		CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK_MSG(pMyToolView, L"Hierarchy tool view nullptr");

		CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
		NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");

		CString name;
		m_ObjectList.GetText(index, name);

		auto iter_find = m_Units.find(name.operator LPCWSTR());
		if (m_Units.end() != iter_find)
			pInspectView->m_HierarchyView.AddCopyObject(&iter_find->second, m_name.operator LPCWSTR());
	}
	UpdateData(FALSE);
}


void CUnitTool::OnBnClickedRemove()
{
	int index = m_ObjectList.GetCurSel();
	if (index != -1)
	{
		CString name;
		m_ObjectList.GetText(index, name);

		auto iter_find = m_Units.find(name.operator LPCWSTR());
		if (m_Units.end() != iter_find)
		m_Units.erase(iter_find);
		m_ObjectList.DeleteString(index);
	}
}
