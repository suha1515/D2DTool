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
						transInfo._ObjectPos = pTransform->GetPosition();			//위치
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

	}
	else
	{
		int iIndex = m_MapList.GetCurSel();
		CString tileName;
		m_MapList.GetText(iIndex, tileName);

		auto iter_find = m_map.find(tileName);
		if (m_map.end() != iter_find)
		{
			(*iter_find).second.clear();

			CString path = L"../Texture/mapData/";
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
						transInfo._ObjectPos = pTransform->GetPosition();			//위치
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

		file.Open(m_strPath, CFile::modeReadWrite, &kEx);

		TCHAR	tmp[MAX_STR] = L"";
		OBJ_INFO		objInfo;						//오브젝트정보
		TRANSFORM_INFO  transInfo;						//트랜스폼정보
		TEXTURE_INFO	textureInfo;					//텍스처정보
		BOXCOL_INFO		boxcolInfo;						//콜라이더정보
		ANIM_INFO		animInfo;						//애니메이터정보
		SCRIPT_INFO		scriptInfo;						//스크립트정보.
		COMP_INFO compInfo;
		
		int sizeScript=0;
		while (file.Read(&compInfo, sizeof(COMP_INFO)))
		{
			//오브젝트 생성
			CGameObject* pGameObject = new CGameObject;
			pGameObject->Initialize();
			//========================================오브젝트 정보 불러오기===========================================
			file.Read(&objInfo, sizeof(OBJ_INFO));
			wstring strInfo;
			pGameObject->SetObjectName(objInfo._ObjectName);		//오브젝트 이름설정
			pGameObject->SetObjectTag(objInfo._ObjectTag);			//오브젝트 태그설정
			pGameObject->SetObjectLayer(objInfo._ObjectLayer);		//오브젝트 레이어설정
			
			if (lstrcmp(objInfo._ParentObject, L""))				//불러온 자료중 부모 오브젝트 이름이 있다면
			{
				CGameObject* pObject = CObjectMgr::GetInstance()->FindObjectWithName(objInfo._ParentObject); //부모 오브젝트를 찾아서 넣는다. 0계층인 부모는 무조건 있으므로.. 순서만 바뀌지 않으면 여기서 문제가 없을것이다.
				pGameObject->SetParentObject(pObject);					//찾아서 해당 오브젝트의 부모 오브젝트로 등록한다
				pObject->GetChildernVector().push_back(pGameObject);	//더욱이 부모 벡터에는 자식을 넣는다.
				pGameObject->SetObjectLevel(pObject->GetLevel()+1);		//부모의 계층보다 1높을것이다.
			}															//아니라면 그냥 넘어간다.
			//==========================================================================================================	

			//========================================트랜스폼 정보 불러오기===========================================
			if (compInfo._Transform == 1)
			{
				file.Read(&transInfo, sizeof(TRANSFORM_INFO));
				CTransform*	pTransform = new CTransform;
				pTransform->Initialize(pGameObject);					//컴포넌트 주체 오브젝트 지정.
				pTransform->SetPosition(transInfo._ObjectPos);			//트랜스폼 위치 지정.
				pTransform->SetRotation(transInfo._ObjectRotation);		//트랜스폼 회전 지정.
				pTransform->SetScaling(transInfo._ObjectScale);			//트랜스폼 크기 지정.

				pGameObject->AddComponent(pTransform);					//오브젝트 컴포넌트 지정.
			}
			//==========================================================================================================	

			//========================================텍스쳐 정보 불러오기==============================================
			if (compInfo._Texture == 1)
			{
				file.Read(&textureInfo, sizeof(TEXTURE_INFO));
				CTextureRenderer*	pTexture = new CTextureRenderer;	
				pTexture->Initialize(pGameObject);						//텍스쳐 컴포넌트 주체 오브젝트 지정.
				pTexture->SetTexture(textureInfo._TextrueName);			//텍스쳐 이름 지정.

				XMFLOAT2 tex[4];										//텍스쳐 위치
				tex[0] = textureInfo._TexturPos[0];
				tex[1] = textureInfo._TexturPos[1];
				tex[2] = textureInfo._TexturPos[2];
				tex[3] = textureInfo._TexturPos[3];

				pTexture->SetVertex(textureInfo._TextureSize, tex);		//텍스처 크기와 위치를 지정한다.
				pGameObject->AddComponent(pTexture);					//오브젝트에 텍스쳐 컴포넌트 지정.
			}
			//==========================================================================================================	


			//========================================콜라이더 정보 불러오기==============================================
			if (compInfo._BoxCol == 1)
			{
				file.Read(&boxcolInfo, sizeof(BOXCOL_INFO));
				CBoxCollider* pBoxCollider = new CBoxCollider;
				pBoxCollider->Initialize(pGameObject);					//박스 콜라이더 컴포넌트 오브젝트 지정.
				pBoxCollider->SetBoxSize(boxcolInfo._BoxWidth, boxcolInfo._BoxHeight);	//박스콜라이더 너비,높이지정.
				pBoxCollider->SetBoxOffset(boxcolInfo._BoxOffsetX,boxcolInfo._BoxOffsetY);	//박스콜라이더 오프셋지정.

				pGameObject->AddComponent(pBoxCollider);				//박스 콜라이더 지정.
			}
			//==========================================================================================================	

			//========================================애니메이터 정보 불러오기==============================================
			if (compInfo._Animator == 1)
			{
				file.Read(&animInfo, sizeof(ANIM_INFO));
				CAnimator*	pAnimator = new CAnimator;
				pAnimator->Initialize(pGameObject);					//애니메이터 오브젝트 지정.
				pAnimator->LoadClips(animInfo._AnimationName);		//애니메이션 지정.

				pGameObject->AddComponent(pAnimator);
			}
			//==========================================================================================================	
	

			//========================================스크립트 정보 불러오기==============================================

			file.Read(&sizeScript, sizeof(int));
			for (int i = 0; i < sizeScript; ++i)
			{
				file.Read(tmp, sizeof(tmp));
				CScriptMgr::GetInstance()->LoadScripts(tmp, pGameObject);
			}
			//==========================================================================================================	

			//완성된 오브젝트를 추가

			CObjectMgr::GetInstance()->AddObject(pGameObject);
		}
		file.Close();
		//하이라키뷰 갱신
		pInspectView->m_HierarchyView.LoadObject();
		MessageBox(L"맵파일을 성공적으로 불러옴", L"Success");
	}
}


void CMapSaveTool::OnLbnDblclkMaplist()
{
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//int iIndex = m_MapList.GetCurSel();
	//if (iIndex != -1)
	//{
	//	CString mapName;
	//	m_MapList.GetText(iIndex, mapName);
	//
	//	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	//	NULL_CHECK(pFrameWnd);

	//	CMyToolView* pToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	//	NULL_CHECK(pToolView);

	//	//타일안에 무엇인가 있을경우..
	//	if (!pToolView->m_GameObject.empty())
	//	{
	//		for (auto& i : pToolView->m_GameObject)
	//		{
	//			SafeDelete(i);
	//		}
	//		pToolView->m_GameObject.clear();
	//	}
	//	for (auto& i : m_map[mapName])
	//	{
	//		/*CGameObject* pGameObject = new CGameObject;
	//		pGameObject->Initialize();
	//		pGameObject->SetPosition(i.pos);
	//		pGameObject->GetComponent<CTextureRenderer>()->SetTexture(i.texture);
	//		pGameObject->GetComponent<CTextureRenderer>()->SetVertex(16, i.tex);

	//		pToolView->m_GameObject.push_back(pGameObject);*/
	//	}
	//	pToolView->Invalidate(FALSE);
	//}
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapSaveTool::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
