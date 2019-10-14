// MapSaveTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MapSaveTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MyToolView.h"
#include "InspectView.h"


#include "GameObject.h"

// CMapSaveTool 대화 상자입니다.

//컴포넌트
#include "TextureRenderer.h"
//컴포넌트
#include "Transform.h"
#include "Animator.h"
#include "BoxCollider.h"
#include "Scripts.h"

IMPLEMENT_DYNAMIC(CMapSaveTool, CDialogEx)

CMapSaveTool::CMapSaveTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SAVETOOL, pParent)
{

}

CMapSaveTool::~CMapSaveTool()
{
}

void CMapSaveTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAPLIST, m_MapList);
}


BEGIN_MESSAGE_MAP(CMapSaveTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapSaveTool::OnBnClickedMapSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapSaveTool::OnBnClickedMapLoad)
	ON_LBN_DBLCLK(IDC_MAPLIST, &CMapSaveTool::OnLbnDblclkMaplist)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapSaveTool::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapSaveTool::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapSaveTool::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapSaveTool::OnBnClickedReMove)
END_MESSAGE_MAP()


// CMapSaveTool 메시지 처리기입니다.


void CMapSaveTool::OnBnClickedMapSave()
{
	cout << "맵저장!" << endl;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyToolView* pToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
	NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");

	const map<int, vector<CGameObject*>>& temp = CObjectMgr::GetInstance()->GetObjects();

	CString m_strPath;
	CFile file;
	CFileException ex;
	CFileDialog dlg(FALSE, _T("*.map"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Map Files(*.map)|*.map|"), NULL);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Map\\MapList");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	int iIndex = m_MapList.GetCurSel();
	if (-1 == iIndex)
	{
		if (dlg.DoModal() == IDOK)
		{
			//저장할 이름 가져오기 (리스트에 추가)
			TCHAR filePath[MAX_STR] = L"";
			TCHAR* fileName;
			lstrcpy(filePath, (LPWSTR)(LPCTSTR)dlg.GetFileName());
			
			//확장자 제거
			PathRemoveExtension(filePath);
			//파일이름만.
			fileName = PathFindFileName(filePath);

			m_strPath = dlg.GetPathName();
			if (m_strPath.Right(4) != ".map")
			{
				m_strPath += ".map";
			}
			file.Open(m_strPath, CFile::modeCreate | CFile::modeReadWrite, &ex);
			
		
			for (int i = 0; i < temp.size(); ++i)
			{
				auto iter = temp.find(i);
				for (auto& j : iter->second)
				{
					TCHAR	tmp[MAX_STR] = L"";
					OBJ_INFO		objInfo;
					TRANSFORM_INFO  transInfo;
					TEXTURE_INFO	textureInfo;
					BOXCOL_INFO		boxcolInfo;
					ANIM_INFO		animInfo;
					SCRIPT_INFO		scriptInfo;
					COMP_INFO compInfo;
					CTransform* pTransform = j->GetComponent<CTransform>();
					CTextureRenderer* pTexture = j->GetComponent<CTextureRenderer>();
					CBoxCollider* pBoxCollider = j->GetComponent<CBoxCollider>();
					CAnimator*  pAnimator = j->GetComponent<CAnimator>();
					int scriptSize = j->GetScripts().size();
					//컴포넌트 정보를 넣기위해 어떤 컴포넌트가 있는지 알려준다.
					if (nullptr != pTransform)
						compInfo._Transform = 1;
					if (nullptr != pTexture)
						compInfo._Texture = 1;
					if (nullptr != pBoxCollider)
						compInfo._BoxCol = 1;
					if (nullptr != pAnimator)
						compInfo._Animator = 1;

					//파일을 읽을때 이것부터 읽고 해당 컴포넌트가 있는지 확인한다
					file.Write(&compInfo, sizeof(compInfo));
							
					//오브젝트 레이어정보
					objInfo._ObjectLayer = j->GetObjectLayer();
					//오브젝트 계층정보
					objInfo._ObjectLevel = j->GetObjectLevel();
					//오브젝트 이름
					lstrcpy(objInfo._ObjectName, j->GetObjectName().c_str());
					//오브젝트 태그
					lstrcpy(objInfo._ObjectTag, j->GetObjectTag().c_str());
					//부모 오브젝트이름
					if (j->GetParentObject() != nullptr)
						lstrcpy(objInfo._ParentObject, j->GetParentObject()->GetObjectName().c_str());
					else
						lstrcpy(objInfo._ParentObject, L"");

					file.Write(&objInfo, sizeof(objInfo));

					//트랜스폼 컴포넌트 정보 쓰기
					if (nullptr != pTransform)										//null이 아니면 정보를 쓴다.
					{
						transInfo._ObjectPos = pTransform->GetLocalPosition();			//위치
						transInfo._ObjectRotation = pTransform->GetRotation();		//회전
						transInfo._ObjectScale = pTransform->GetScale();			//크기
						file.Write(&transInfo, sizeof(TRANSFORM_INFO));
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
						file.Write(&textureInfo, sizeof(TEXTURE_INFO));
					}
					//박스 콜라이더 정보 쓰기
					if (nullptr != pBoxCollider)
					{
						boxcolInfo._BoxHeight = pBoxCollider->GetBoxHeight();				//박스높이
						boxcolInfo._BoxWidth = pBoxCollider->GetBoxWidth();					//박스너비
						boxcolInfo._BoxOffsetX = pBoxCollider->GetBoxOffsetX();				//오프셋X
						boxcolInfo._BoxOffsetY = pBoxCollider->GetBoxoffsetY();				//오프셋Y
						boxcolInfo._colType = pBoxCollider->GetCollideType();				//박스타입.
						file.Write(&boxcolInfo, sizeof(BOXCOL_INFO));
					}
					//애니메이터 정보 쓰기
					if (nullptr != pAnimator)
					{
						lstrcpy(animInfo._AnimationName, pAnimator->GetAnimationName().c_str());
						file.Write(&animInfo, sizeof(ANIM_INFO));
					}

					file.Write(&scriptSize, sizeof(int));
					//스크립트 정보 쓰기
					if (scriptSize > 0)
					{
						for (auto&k : j->GetScripts())
						{
							lstrcpy(tmp, strings::widen(k.first).c_str());
							file.Write(tmp, sizeof(tmp));
						}
					}
				}
			}
			m_MapList.AddString(fileName);
			m_map.insert({ fileName,m_strPath });
			file.Close();

			MessageBox(L"맵파일을 성공적으로 저장", L"Success");
		}

	}
	else
	{
		int iIndex = m_MapList.GetCurSel();
		CString tileName;
		m_MapList.GetText(iIndex, tileName);

		auto iter_find = m_map.find(tileName);
		if (m_map.end() != iter_find)
		{

			CString path = L"../Texture/map/";
			path = path + tileName + L".map";

			file.Open(path, CFile::modeCreate | CFile::modeReadWrite, &ex);
			TCHAR	tmp[MAX_STR] = L"";
			OBJ_INFO		objInfo;
			TRANSFORM_INFO  transInfo;
			TEXTURE_INFO	textureInfo;
			BOXCOL_INFO		boxcolInfo;
			ANIM_INFO		animInfo;
			SCRIPT_INFO		scriptInfo;


			for (int i = 0; i < temp.size(); ++i)
			{
				auto iter = temp.find(i);
				for (auto& j : iter->second)
				{
					COMP_INFO compInfo;
					CTransform* pTransform = j->GetComponent<CTransform>();
					CTextureRenderer* pTexture = j->GetComponent<CTextureRenderer>();
					CBoxCollider* pBoxCollider = j->GetComponent<CBoxCollider>();
					CAnimator*  pAnimator = j->GetComponent<CAnimator>();
					int scriptSize = j->GetScripts().size();
					//컴포넌트 정보를 넣기위해 어떤 컴포넌트가 있는지 알려준다.
					if (nullptr != pTransform)
						compInfo._Transform = 1;
					if (nullptr != pTexture)
						compInfo._Texture = 1;
					if (nullptr != pBoxCollider)
						compInfo._BoxCol = 1;
					if (nullptr != pAnimator)
						compInfo._Animator = 1;

					//파일을 읽을때 이것부터 읽고 해당 컴포넌트가 있는지 확인한다
					file.Write(&compInfo, sizeof(compInfo));

					//오브젝트 레이어정보
					objInfo._ObjectLayer = j->GetObjectLayer();
					//오브젝트 계층정보
					objInfo._ObjectLevel = j->GetObjectLevel();
					//오브젝트 이름
					lstrcpy(objInfo._ObjectName, j->GetObjectName().c_str());
					//오브젝트 태그
					lstrcpy(objInfo._ObjectTag, j->GetObjectTag().c_str());
					//부모 오브젝트이름
					if (j->GetParentObject() != nullptr)
						lstrcpy(objInfo._ParentObject, j->GetParentObject()->GetObjectName().c_str());
					else
						lstrcpy(objInfo._ParentObject, L"");
					file.Write(&objInfo, sizeof(objInfo));

					//트랜스폼 컴포넌트 정보 쓰기
					if (nullptr != pTransform)										//null이 아니면 정보를 쓴다.
					{
						transInfo._ObjectPos = pTransform->GetLocalPosition();			//위치
						transInfo._ObjectRotation = pTransform->GetRotation();		//회전
						transInfo._ObjectScale = pTransform->GetScale();			//크기
						file.Write(&transInfo, sizeof(TRANSFORM_INFO));
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
						file.Write(&textureInfo, sizeof(TEXTURE_INFO));
					}
					//박스 콜라이더 정보 쓰기
					if (nullptr != pBoxCollider)
					{
						boxcolInfo._BoxHeight = pBoxCollider->GetBoxHeight();				//박스높이
						boxcolInfo._BoxWidth = pBoxCollider->GetBoxWidth();					//박스너비
						boxcolInfo._BoxOffsetX = pBoxCollider->GetBoxOffsetX();				//오프셋X
						boxcolInfo._BoxOffsetY = pBoxCollider->GetBoxoffsetY();				//오프셋Y
						boxcolInfo._colType = pBoxCollider->GetCollideType();				//박스타입.
						file.Write(&boxcolInfo, sizeof(BOXCOL_INFO));
					}
					//애니메이터 정보 쓰기
					if (nullptr != pAnimator)
					{
						lstrcpy(animInfo._AnimationName, pAnimator->GetAnimationName().c_str());
						file.Write(&animInfo, sizeof(ANIM_INFO));
					}

					file.Write(&scriptSize, sizeof(int));
					//스크립트 정보 쓰기
					if (scriptSize > 0)
					{
						for (auto&k : j->GetScripts())
						{
							lstrcpy(tmp, strings::widen(k.first).c_str());
							file.Write(tmp, sizeof(tmp));
						}
					}

				}
			}
			file.Close();
			MessageBox(L"맵파일을 성공적으로 저장", L"Success");
		}
		else
			return;
	}
}


void CMapSaveTool::OnBnClickedMapLoad()
{
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyToolView* pToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
	NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");

	CStdioFile	file;
	CFileException kEx;
	CFileDialog dlg(TRUE, _T("*.map"), NULL, OFN_FILEMUSTEXIST, _T("MAP Files(*.map)|*.map|"), NULL);
	CString m_strPath;

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Map\\MapList");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if (dlg.DoModal() == IDOK)
	{
		//오브젝트 모두 없애기 (불러오기 이므로)
		CObjectMgr::GetInstance()->Clear();
		pInspectView->m_HierarchyView.Clear();

		//파일 이름 얻기
		m_strPath = dlg.GetPathName();
		//저장할 이름 가져오기 (리스트에 추가)
		WCHAR  filePath[MAX_STR] = L"";
		TCHAR* fileName;
		lstrcpy(filePath, (LPWSTR)(LPCTSTR)dlg.GetFileName());

		//확장자 제거
		PathRemoveExtension(filePath);
		//파일이름만.
		fileName = PathFindFileName(filePath);
		
		//이미 존재하는지 확인 이미존재한다면 다시 로드만한다.
		auto iter_find = m_map.find(fileName);
		if (iter_find != m_map.end())
		{
			MessageBox( L"이미 존재하므로 다시 로드합니다", L"ReLoad");
			CObjectMgr::GetInstance()->LoadObject(m_strPath.operator LPCWSTR());
			pInspectView->m_HierarchyView.LoadObject();
			return;
		}

		CObjectMgr::GetInstance()->LoadObject(m_strPath.operator LPCWSTR());

		m_MapList.AddString(fileName);
		m_map.insert({ fileName,m_strPath });
		//하이라키뷰 갱신
		pInspectView->m_HierarchyView.LoadObject();
		MessageBox(L"맵파일을 성공적으로 불러옴", L"Success");
	}
}


void CMapSaveTool::OnLbnDblclkMaplist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iIndex = m_MapList.GetCurSel();
	if (iIndex != -1)
	{
		CString mapName;
		m_MapList.GetText(iIndex, mapName);
	
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CMyToolView* pToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pToolView);

		CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
		NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");

		auto iter_find = m_map.find(mapName);
		if (iter_find != m_map.end())
		{
			pInspectView->m_HierarchyView.Clear();
			wstring filePath = iter_find->second;
			wstring text = mapName + L"을 다시 로드합니다";
			MessageBox(text.c_str(), L"ReLoad");
			CObjectMgr::GetInstance()->LoadObject(filePath);
			pInspectView->m_HierarchyView.LoadObject();
			return;
		}
	
		pToolView->Invalidate(FALSE);
	}
}

//선택해제
void CMapSaveTool::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_MapList.SetCurSel(-1);
}


BOOL CMapSaveTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  
}


void CMapSaveTool::OnBnClickedButton8()
{

	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(TRUE, L".txt", L"제목없음.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Map\\MapList");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if (IDOK == Dlg.DoModal())
	{
		wifstream fin;
		fin.open(Dlg.GetPathName());

		if (fin.fail())
			return;

		if (!m_map.empty())		//불러오는데 맵을 비워야한다.
			m_map.clear();		//클리어

		//맵 리스트도 비운다.
		m_MapList.ResetContent();
		TCHAR		mapPath[MAX_STR] = L"";
		TCHAR		mapName[MAX_STR] = L"";
		while (true)
		{
			fin.getline(mapName, MAX_STR, '|');
			fin.getline(mapPath, MAX_STR);
;
			if (fin.eof())					//다불러왔을경우 루프종료
				break;
			m_map.insert({ mapName,mapPath });
			
			m_MapList.AddString(mapName);
		}

		fin.close();
	}
}


void CMapSaveTool::OnBnClickedButton4()
{
	TCHAR filePath[MAX_STR] = L"../Map/Maplist.txt";
	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(FALSE, L".txt", L"제목없음.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Map\\MapList");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	TCHAR mapList[MAX_STR] = L"";

	if (IDOK == Dlg.DoModal())
	{
		wofstream fout;
		fout.open(Dlg.GetPathName());

		if (fout.fail())
			return;

		for (auto& i : m_map)
		{
			wstring temp = i.first+L"|"+L"../Map/" + i.first + ".map";
			fout << temp << endl;
		}
		fout.close();

		AfxMessageBox(filePath);
	}
}


void CMapSaveTool::OnBnClickedReMove()
{
	int index = m_MapList.GetCurSel();
	if (index != -1)
	{
		CString name;
		m_MapList.GetText(index, name);
		auto iter = m_map.find(name);
		if (m_map.end() != iter)
		{
			m_map.erase(iter);
		}
		m_MapList.DeleteString(index);
	}
}
