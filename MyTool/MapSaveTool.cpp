// MapSaveTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MapSaveTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MyToolView.h"

#include "GameObject.h"

// CMapSaveTool 대화 상자입니다.

//컴포넌트
#include "TextureRenderer.h"
//컴포넌트
#include "Transform.h"

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

	const vector<CGameObject*>& temp = pToolView->m_GameObject;

	vector<MAP_TILE> temp2;
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
			MAP_TILE info;
			for (auto& i : temp)
			{
				info.pos = i->GetComponent<CTransform>()->GetPosition();

				CTextureRenderer *pRender = i->GetComponent<CTextureRenderer>();
				info.tex[0] = pRender->GetTexPos(0);
				info.tex[1] = pRender->GetTexPos(1);
			    info.tex[2] = pRender->GetTexPos(2);
				info.tex[3] = pRender->GetTexPos(3);

				_tcscpy(info.texture, pRender->GetTexName().c_str());
				file.Write(&info, sizeof(MAP_TILE));
				temp2.push_back(info);
			}
			file.Close();

			pToolView->m_MapName = fileName;
			m_map.insert({ fileName,temp2 });
			m_MapList.AddString(fileName);

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
			MAP_TILE info;
			for (auto& i : temp)
			{
				info.pos = i->GetComponent<CTransform>()->GetPosition();
				CTextureRenderer *pRender = i->GetComponent<CTextureRenderer>();
				info.tex[0] = pRender->GetTexPos(0);
				info.tex[1] = pRender->GetTexPos(1);
				info.tex[2] = pRender->GetTexPos(2);
				info.tex[3] = pRender->GetTexPos(3);

				_tcscpy(info.texture, pRender->GetTexName().c_str());
				file.Write(&info, sizeof(MAP_TILE));
				m_map[tileName].push_back(info);
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMyToolView* pToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
	NULL_CHECK(pToolView);

	CStdioFile	file;
	CFileException kEx;
	CFileDialog dlg(TRUE, _T("*.map"), NULL, OFN_FILEMUSTEXIST, _T("MAP Files(*.map)|*.map|"), NULL);
	CString m_strPath;

	if (dlg.DoModal() == IDOK)
	{
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

		//리스트에 이미 있을경우.
		auto iter_find = m_map.find(fileName);
		if (m_map.end() != iter_find)
		{
			MessageBox(L"해당 정보가 이미 로드되어있음", L"Stop!");
			return;
		}
		file.Open(m_strPath, CFile::modeReadWrite, &kEx);
		MAP_TILE info;

		vector<MAP_TILE> temp;

		while (file.Read(&info, sizeof(MAP_TILE)))
		{
			temp.push_back(info);
		}
		file.Close();

		//저장
		m_map.insert({ fileName, temp });

		m_MapList.AddString(fileName);
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

		//타일안에 무엇인가 있을경우..
		if (!pToolView->m_GameObject.empty())
		{
			for (auto& i : pToolView->m_GameObject)
			{
				SafeDelete(i);
			}
			pToolView->m_GameObject.clear();
		}
		for (auto& i : m_map[mapName])
		{
			/*CGameObject* pGameObject = new CGameObject;
			pGameObject->Initialize();
			pGameObject->SetPosition(i.pos);
			pGameObject->GetComponent<CTextureRenderer>()->SetTexture(i.texture);
			pGameObject->GetComponent<CTextureRenderer>()->SetVertex(16, i.tex);

			pToolView->m_GameObject.push_back(pGameObject);*/
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
