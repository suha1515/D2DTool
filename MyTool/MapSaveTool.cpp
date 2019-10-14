// MapSaveTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MapSaveTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "MyToolView.h"
#include "InspectView.h"


#include "GameObject.h"

// CMapSaveTool ��ȭ �����Դϴ�.

//������Ʈ
#include "TextureRenderer.h"
//������Ʈ
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


// CMapSaveTool �޽��� ó�����Դϴ�.


void CMapSaveTool::OnBnClickedMapSave()
{
	cout << "������!" << endl;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Map\\MapList");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

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
					//������Ʈ ������ �ֱ����� � ������Ʈ�� �ִ��� �˷��ش�.
					if (nullptr != pTransform)
						compInfo._Transform = 1;
					if (nullptr != pTexture)
						compInfo._Texture = 1;
					if (nullptr != pBoxCollider)
						compInfo._BoxCol = 1;
					if (nullptr != pAnimator)
						compInfo._Animator = 1;

					//������ ������ �̰ͺ��� �а� �ش� ������Ʈ�� �ִ��� Ȯ���Ѵ�
					file.Write(&compInfo, sizeof(compInfo));
							
					//������Ʈ ���̾�����
					objInfo._ObjectLayer = j->GetObjectLayer();
					//������Ʈ ��������
					objInfo._ObjectLevel = j->GetObjectLevel();
					//������Ʈ �̸�
					lstrcpy(objInfo._ObjectName, j->GetObjectName().c_str());
					//������Ʈ �±�
					lstrcpy(objInfo._ObjectTag, j->GetObjectTag().c_str());
					//�θ� ������Ʈ�̸�
					if (j->GetParentObject() != nullptr)
						lstrcpy(objInfo._ParentObject, j->GetParentObject()->GetObjectName().c_str());
					else
						lstrcpy(objInfo._ParentObject, L"");

					file.Write(&objInfo, sizeof(objInfo));

					//Ʈ������ ������Ʈ ���� ����
					if (nullptr != pTransform)										//null�� �ƴϸ� ������ ����.
					{
						transInfo._ObjectPos = pTransform->GetLocalPosition();			//��ġ
						transInfo._ObjectRotation = pTransform->GetRotation();		//ȸ��
						transInfo._ObjectScale = pTransform->GetScale();			//ũ��
						file.Write(&transInfo, sizeof(TRANSFORM_INFO));
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
						file.Write(&textureInfo, sizeof(TEXTURE_INFO));
					}
					//�ڽ� �ݶ��̴� ���� ����
					if (nullptr != pBoxCollider)
					{
						boxcolInfo._BoxHeight = pBoxCollider->GetBoxHeight();				//�ڽ�����
						boxcolInfo._BoxWidth = pBoxCollider->GetBoxWidth();					//�ڽ��ʺ�
						boxcolInfo._BoxOffsetX = pBoxCollider->GetBoxOffsetX();				//������X
						boxcolInfo._BoxOffsetY = pBoxCollider->GetBoxoffsetY();				//������Y
						boxcolInfo._colType = pBoxCollider->GetCollideType();				//�ڽ�Ÿ��.
						file.Write(&boxcolInfo, sizeof(BOXCOL_INFO));
					}
					//�ִϸ����� ���� ����
					if (nullptr != pAnimator)
					{
						lstrcpy(animInfo._AnimationName, pAnimator->GetAnimationName().c_str());
						file.Write(&animInfo, sizeof(ANIM_INFO));
					}

					file.Write(&scriptSize, sizeof(int));
					//��ũ��Ʈ ���� ����
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
					//������Ʈ ������ �ֱ����� � ������Ʈ�� �ִ��� �˷��ش�.
					if (nullptr != pTransform)
						compInfo._Transform = 1;
					if (nullptr != pTexture)
						compInfo._Texture = 1;
					if (nullptr != pBoxCollider)
						compInfo._BoxCol = 1;
					if (nullptr != pAnimator)
						compInfo._Animator = 1;

					//������ ������ �̰ͺ��� �а� �ش� ������Ʈ�� �ִ��� Ȯ���Ѵ�
					file.Write(&compInfo, sizeof(compInfo));

					//������Ʈ ���̾�����
					objInfo._ObjectLayer = j->GetObjectLayer();
					//������Ʈ ��������
					objInfo._ObjectLevel = j->GetObjectLevel();
					//������Ʈ �̸�
					lstrcpy(objInfo._ObjectName, j->GetObjectName().c_str());
					//������Ʈ �±�
					lstrcpy(objInfo._ObjectTag, j->GetObjectTag().c_str());
					//�θ� ������Ʈ�̸�
					if (j->GetParentObject() != nullptr)
						lstrcpy(objInfo._ParentObject, j->GetParentObject()->GetObjectName().c_str());
					else
						lstrcpy(objInfo._ParentObject, L"");
					file.Write(&objInfo, sizeof(objInfo));

					//Ʈ������ ������Ʈ ���� ����
					if (nullptr != pTransform)										//null�� �ƴϸ� ������ ����.
					{
						transInfo._ObjectPos = pTransform->GetLocalPosition();			//��ġ
						transInfo._ObjectRotation = pTransform->GetRotation();		//ȸ��
						transInfo._ObjectScale = pTransform->GetScale();			//ũ��
						file.Write(&transInfo, sizeof(TRANSFORM_INFO));
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
						file.Write(&textureInfo, sizeof(TEXTURE_INFO));
					}
					//�ڽ� �ݶ��̴� ���� ����
					if (nullptr != pBoxCollider)
					{
						boxcolInfo._BoxHeight = pBoxCollider->GetBoxHeight();				//�ڽ�����
						boxcolInfo._BoxWidth = pBoxCollider->GetBoxWidth();					//�ڽ��ʺ�
						boxcolInfo._BoxOffsetX = pBoxCollider->GetBoxOffsetX();				//������X
						boxcolInfo._BoxOffsetY = pBoxCollider->GetBoxoffsetY();				//������Y
						boxcolInfo._colType = pBoxCollider->GetCollideType();				//�ڽ�Ÿ��.
						file.Write(&boxcolInfo, sizeof(BOXCOL_INFO));
					}
					//�ִϸ����� ���� ����
					if (nullptr != pAnimator)
					{
						lstrcpy(animInfo._AnimationName, pAnimator->GetAnimationName().c_str());
						file.Write(&animInfo, sizeof(ANIM_INFO));
					}

					file.Write(&scriptSize, sizeof(int));
					//��ũ��Ʈ ���� ����
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
			MessageBox(L"�������� ���������� ����", L"Success");
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

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Map\\MapList");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	if (dlg.DoModal() == IDOK)
	{
		//������Ʈ ��� ���ֱ� (�ҷ����� �̹Ƿ�)
		CObjectMgr::GetInstance()->Clear();
		pInspectView->m_HierarchyView.Clear();

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
		
		//�̹� �����ϴ��� Ȯ�� �̹������Ѵٸ� �ٽ� �ε常�Ѵ�.
		auto iter_find = m_map.find(fileName);
		if (iter_find != m_map.end())
		{
			MessageBox( L"�̹� �����ϹǷ� �ٽ� �ε��մϴ�", L"ReLoad");
			CObjectMgr::GetInstance()->LoadObject(m_strPath.operator LPCWSTR());
			pInspectView->m_HierarchyView.LoadObject();
			return;
		}

		CObjectMgr::GetInstance()->LoadObject(m_strPath.operator LPCWSTR());

		m_MapList.AddString(fileName);
		m_map.insert({ fileName,m_strPath });
		//���̶�Ű�� ����
		pInspectView->m_HierarchyView.LoadObject();
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

		CInspectView* pInspectView = dynamic_cast<CInspectView*>(pFrameWnd->m_MainSplitter.GetPane(0, 2));
		NULL_CHECK_MSG(pInspectView, L"InspectView tool view nullptr");

		auto iter_find = m_map.find(mapName);
		if (iter_find != m_map.end())
		{
			pInspectView->m_HierarchyView.Clear();
			wstring filePath = iter_find->second;
			wstring text = mapName + L"�� �ٽ� �ε��մϴ�";
			MessageBox(text.c_str(), L"ReLoad");
			CObjectMgr::GetInstance()->LoadObject(filePath);
			pInspectView->m_HierarchyView.LoadObject();
			return;
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

	return TRUE;  
}


void CMapSaveTool::OnBnClickedButton8()
{

	// ���� ���� Ȥ�� ���忡 ���� ��ȭ���� MFC Ŭ����
	CFileDialog Dlg(TRUE, L".txt", L"�������.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Map\\MapList");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	if (IDOK == Dlg.DoModal())
	{
		wifstream fin;
		fin.open(Dlg.GetPathName());

		if (fin.fail())
			return;

		if (!m_map.empty())		//�ҷ����µ� ���� ������Ѵ�.
			m_map.clear();		//Ŭ����

		//�� ����Ʈ�� ����.
		m_MapList.ResetContent();
		TCHAR		mapPath[MAX_STR] = L"";
		TCHAR		mapName[MAX_STR] = L"";
		while (true)
		{
			fin.getline(mapName, MAX_STR, '|');
			fin.getline(mapPath, MAX_STR);
;
			if (fin.eof())					//�ٺҷ�������� ��������
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
	// ���� ���� Ȥ�� ���忡 ���� ��ȭ���� MFC Ŭ����
	CFileDialog Dlg(FALSE, L".txt", L"�������.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Map\\MapList");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

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
