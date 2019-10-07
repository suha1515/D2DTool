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
						transInfo._ObjectPos = pTransform->GetPosition();			//��ġ
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
						transInfo._ObjectPos = pTransform->GetPosition();			//��ġ
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

		file.Open(m_strPath, CFile::modeReadWrite, &kEx);

		TCHAR	tmp[MAX_STR] = L"";
		OBJ_INFO		objInfo;						//������Ʈ����
		TRANSFORM_INFO  transInfo;						//Ʈ����������
		TEXTURE_INFO	textureInfo;					//�ؽ�ó����
		BOXCOL_INFO		boxcolInfo;						//�ݶ��̴�����
		ANIM_INFO		animInfo;						//�ִϸ���������
		SCRIPT_INFO		scriptInfo;						//��ũ��Ʈ����.
		COMP_INFO compInfo;
		
		int sizeScript=0;
		while (file.Read(&compInfo, sizeof(COMP_INFO)))
		{
			//������Ʈ ����
			CGameObject* pGameObject = new CGameObject;
			pGameObject->Initialize();
			//========================================������Ʈ ���� �ҷ�����===========================================
			file.Read(&objInfo, sizeof(OBJ_INFO));
			wstring strInfo;
			pGameObject->SetObjectName(objInfo._ObjectName);		//������Ʈ �̸�����
			pGameObject->SetObjectTag(objInfo._ObjectTag);			//������Ʈ �±׼���
			pGameObject->SetObjectLayer(objInfo._ObjectLayer);		//������Ʈ ���̾��
			
			if (lstrcmp(objInfo._ParentObject, L""))				//�ҷ��� �ڷ��� �θ� ������Ʈ �̸��� �ִٸ�
			{
				CGameObject* pObject = CObjectMgr::GetInstance()->FindObjectWithName(objInfo._ParentObject); //�θ� ������Ʈ�� ã�Ƽ� �ִ´�. 0������ �θ�� ������ �����Ƿ�.. ������ �ٲ��� ������ ���⼭ ������ �������̴�.
				pGameObject->SetParentObject(pObject);					//ã�Ƽ� �ش� ������Ʈ�� �θ� ������Ʈ�� ����Ѵ�
				pObject->GetChildernVector().push_back(pGameObject);	//������ �θ� ���Ϳ��� �ڽ��� �ִ´�.
				pGameObject->SetObjectLevel(pObject->GetLevel()+1);		//�θ��� �������� 1�������̴�.
			}															//�ƴ϶�� �׳� �Ѿ��.
			//==========================================================================================================	

			//========================================Ʈ������ ���� �ҷ�����===========================================
			if (compInfo._Transform == 1)
			{
				file.Read(&transInfo, sizeof(TRANSFORM_INFO));
				CTransform*	pTransform = new CTransform;
				pTransform->Initialize(pGameObject);					//������Ʈ ��ü ������Ʈ ����.
				pTransform->SetPosition(transInfo._ObjectPos);			//Ʈ������ ��ġ ����.
				pTransform->SetRotation(transInfo._ObjectRotation);		//Ʈ������ ȸ�� ����.
				pTransform->SetScaling(transInfo._ObjectScale);			//Ʈ������ ũ�� ����.

				pGameObject->AddComponent(pTransform);					//������Ʈ ������Ʈ ����.
			}
			//==========================================================================================================	

			//========================================�ؽ��� ���� �ҷ�����==============================================
			if (compInfo._Texture == 1)
			{
				file.Read(&textureInfo, sizeof(TEXTURE_INFO));
				CTextureRenderer*	pTexture = new CTextureRenderer;	
				pTexture->Initialize(pGameObject);						//�ؽ��� ������Ʈ ��ü ������Ʈ ����.
				pTexture->SetTexture(textureInfo._TextrueName);			//�ؽ��� �̸� ����.

				XMFLOAT2 tex[4];										//�ؽ��� ��ġ
				tex[0] = textureInfo._TexturPos[0];
				tex[1] = textureInfo._TexturPos[1];
				tex[2] = textureInfo._TexturPos[2];
				tex[3] = textureInfo._TexturPos[3];

				pTexture->SetVertex(textureInfo._TextureSize, tex);		//�ؽ�ó ũ��� ��ġ�� �����Ѵ�.
				pGameObject->AddComponent(pTexture);					//������Ʈ�� �ؽ��� ������Ʈ ����.
			}
			//==========================================================================================================	


			//========================================�ݶ��̴� ���� �ҷ�����==============================================
			if (compInfo._BoxCol == 1)
			{
				file.Read(&boxcolInfo, sizeof(BOXCOL_INFO));
				CBoxCollider* pBoxCollider = new CBoxCollider;
				pBoxCollider->Initialize(pGameObject);					//�ڽ� �ݶ��̴� ������Ʈ ������Ʈ ����.
				pBoxCollider->SetBoxSize(boxcolInfo._BoxWidth, boxcolInfo._BoxHeight);	//�ڽ��ݶ��̴� �ʺ�,��������.
				pBoxCollider->SetBoxOffset(boxcolInfo._BoxOffsetX,boxcolInfo._BoxOffsetY);	//�ڽ��ݶ��̴� ����������.

				pGameObject->AddComponent(pBoxCollider);				//�ڽ� �ݶ��̴� ����.
			}
			//==========================================================================================================	

			//========================================�ִϸ����� ���� �ҷ�����==============================================
			if (compInfo._Animator == 1)
			{
				file.Read(&animInfo, sizeof(ANIM_INFO));
				CAnimator*	pAnimator = new CAnimator;
				pAnimator->Initialize(pGameObject);					//�ִϸ����� ������Ʈ ����.
				pAnimator->LoadClips(animInfo._AnimationName);		//�ִϸ��̼� ����.

				pGameObject->AddComponent(pAnimator);
			}
			//==========================================================================================================	
	

			//========================================��ũ��Ʈ ���� �ҷ�����==============================================

			file.Read(&sizeScript, sizeof(int));
			for (int i = 0; i < sizeScript; ++i)
			{
				file.Read(tmp, sizeof(tmp));
				CScriptMgr::GetInstance()->LoadScripts(tmp, pGameObject);
			}
			//==========================================================================================================	

			//�ϼ��� ������Ʈ�� �߰�

			CObjectMgr::GetInstance()->AddObject(pGameObject);
		}
		file.Close();
		//���̶�Ű�� ����
		pInspectView->m_HierarchyView.LoadObject();
		MessageBox(L"�������� ���������� �ҷ���", L"Success");
	}
}


void CMapSaveTool::OnLbnDblclkMaplist()
{
	//// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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

	//	//Ÿ�Ͼȿ� �����ΰ� �������..
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


void CMapSaveTool::OnBnClickedButton8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
