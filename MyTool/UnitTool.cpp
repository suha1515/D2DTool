// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


#include "MainFrm.h"
#include "MyToolView.h"

//툴뷰 화면 갱신
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

	//툴뷰 가져오기
	CMyToolView* pMyToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK_MSG(pMyToolView, L"Hierarchy tool view nullptr");

	CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
	NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");


	//하이라키뷰에서 선택된 오브젝트를 가져온다.
	CGameObject* pObject = pInspectView->m_HierarchyView.m_CurClicked;
	if (pObject != nullptr)
	{
		UpdateData(TRUE);
		//오브젝트이름
		CString objectName = pObject->GetObjectName().c_str();

		auto iter_find = m_Units.find(m_CopyName.operator LPCWSTR());
		//이미 리스트에 오브젝트가 있으므로 넘긴다
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
	//컴포넌트 정보를 넣기위해 어떤 컴포넌트가 있는지 알려준다.
	if (nullptr != pTransform)
		compInfo._Transform = 1;
	if (nullptr != pTexture)
		compInfo._Texture = 1;
	if (nullptr != pBoxCollider)
		compInfo._BoxCol = 1;
	if (nullptr != pAnimator)
		compInfo._Animator = 1;
	
	copyObject.compInfo = compInfo;

	//오브젝트 레이어정보
	objInfo._ObjectLayer = pObject->GetObjectLayer();
	//오브젝트 계층정보
	objInfo._ObjectLevel = pObject->GetObjectLevel();
	//오브젝트 이름
	lstrcpy(objInfo._ObjectName, pObject->GetObjectName().c_str());
	//오브젝트 태그
	lstrcpy(objInfo._ObjectTag, pObject->GetObjectTag().c_str());
	//부모 오브젝트이름
	if (pObject->GetParentObject() != nullptr)
		lstrcpy(objInfo._ParentObject, pObject->GetParentObject()->GetObjectName().c_str());
	else
		lstrcpy(objInfo._ParentObject, L"");

	copyObject.objInfo = objInfo;

	//트랜스폼 컴포넌트 정보 쓰기
	if (nullptr != pTransform)										//null이 아니면 정보를 쓴다.
	{
		transInfo._ObjectPos = pTransform->GetLocalPosition();			//위치
		transInfo._ObjectRotation = pTransform->GetRotation();		//회전
		transInfo._ObjectScale = pTransform->GetScale();			//크기
		copyObject.transformInfo = transInfo;
	}
	//텍스처 컴포넌트 정보 쓰기
	if (nullptr != pTexture)
	{
		lstrcpy(textureInfo._TextrueName, pTexture->GetTexName().c_str());		//텍스처이름
		textureInfo._TextureSize = pTexture->GetTexSize();						//텍스처크기
		textureInfo._TexturPos[0] = pTexture->GetTexPos()[0];					//텍스처 좌표
		textureInfo._TexturPos[1] = pTexture->GetTexPos()[1];
		textureInfo._TexturPos[2] = pTexture->GetTexPos()[2];
		textureInfo._TexturPos[3] = pTexture->GetTexPos()[3];
		copyObject.textureInfo = textureInfo;
	}
	//박스 콜라이더 정보 쓰기
	if (nullptr != pBoxCollider)
	{
		boxcolInfo._BoxHeight = pBoxCollider->GetBoxHeight();				//박스높이
		boxcolInfo._BoxWidth = pBoxCollider->GetBoxWidth();					//박스너비
		boxcolInfo._BoxOffsetX = pBoxCollider->GetBoxOffsetX();				//오프셋X
		boxcolInfo._BoxOffsetY = pBoxCollider->GetBoxoffsetY();				//오프셋Y
		boxcolInfo._colType = pBoxCollider->GetCollideType();				//콜라이더 타입
		copyObject.boxcolInfo = boxcolInfo;
	}
	//애니메이터 정보 쓰기
	if (nullptr != pAnimator)
	{
		lstrcpy(animInfo._AnimationName, pAnimator->GetAnimationName().c_str());
		copyObject.animInfo = animInfo;
	}
	//스크립트 정보 쓰기
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
	//저장된 유닛들의 정보를 저장한다.
	//나중에 빠르게 불러올 수 있다.


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString m_strPath;
	CFile file;
	CFileException ex;
	CFileDialog dlg(FALSE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("DataFile(*.dat)|*.dat|"), NULL);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\ObjectList");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

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

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\ObjectList");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if (dlg.DoModal() == IDOK)
	{
		CObjectMgr::GetInstance()->ClearCopy();
		m_ObjectList.ResetContent();
		//파일 이름 얻기
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
	
		MessageBox(L"오브젝트파일을 성공적으로 불러옴", L"Success");
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

		//툴뷰 가져오기
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
