// MapSaveTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MapSaveTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MyToolView.h"

#include "GameObject.h"

// CMapSaveTool ��ȭ �����Դϴ�.

//������Ʈ
#include "TextureRenderer.h"
//������Ʈ
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


// CMapSaveTool �޽��� ó�����Դϴ�.


void CMapSaveTool::OnBnClickedMapSave()
{
	cout << "������!" << endl;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
			//������ �̸� �������� (����Ʈ�� �߰�)
			TCHAR filePath[MAX_STR] = L"";
			TCHAR* fileName;
			lstrcpy(filePath, (LPWSTR)(LPCTSTR)dlg.GetFileName());
			
			//Ȯ���� ����
			PathRemoveExtension(filePath);
			//�����̸���.
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

			MessageBox(L"�������� ���������� ����", L"Success");
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
			MessageBox(L"�������� ���������� ����", L"Success");
		}
		else
			return;

	}
}


void CMapSaveTool::OnBnClickedMapLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		//���� �̸� ���
		m_strPath = dlg.GetPathName();
		//������ �̸� �������� (����Ʈ�� �߰�)
		WCHAR  filePath[MAX_STR] = L"";
		TCHAR* fileName;
		lstrcpy(filePath, (LPWSTR)(LPCTSTR)dlg.GetFileName());

		//Ȯ���� ����
		PathRemoveExtension(filePath);
		//�����̸���.
		fileName = PathFindFileName(filePath);

		//����Ʈ�� �̹� �������.
		auto iter_find = m_map.find(fileName);
		if (m_map.end() != iter_find)
		{
			MessageBox(L"�ش� ������ �̹� �ε�Ǿ�����", L"Stop!");
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

		//����
		m_map.insert({ fileName, temp });

		m_MapList.AddString(fileName);
		MessageBox(L"�������� ���������� �ҷ���", L"Success");
	}
}


void CMapSaveTool::OnLbnDblclkMaplist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iIndex = m_MapList.GetCurSel();
	if (iIndex != -1)
	{
		CString mapName;
		m_MapList.GetText(iIndex, mapName);
	
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK(pFrameWnd);

		CMyToolView* pToolView = dynamic_cast<CMyToolView*>(pFrameWnd->m_MainSplitter.GetPane(0, 1));
		NULL_CHECK(pToolView);

		//Ÿ�Ͼȿ� �����ΰ� �������..
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

//��������
void CMapSaveTool::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_MapList.SetCurSel(-1);
}


BOOL CMapSaveTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.



	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
