// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MapTool.h"
#include "afxdialogex.h"


//������
#include "MainFrm.h"
//�̴Ϻ�
#include "MiniView.h"
#include "MyToolView.h"

//ī�޶� Ŭ����
#include "Camera.h"

// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialogEx)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAPTOOL, pParent),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pVB(nullptr), m_pIB(nullptr), m_Cam(nullptr), m_texInfo(nullptr)
	, m_TileSetSize(_T(""))
	, m_iTileSizeX(0)
	, m_iTileSizeY(0)
{

}

CMapTool::~CMapTool()
{
	if(m_Cam != nullptr)
		delete m_Cam;
	if (m_pIB != nullptr)
		m_pIB->Release();
	if (m_pVB != nullptr)
		m_pVB->Release();

	//��Ÿ�ϼ� ���� ����
	for (auto& i : m_mapTileSetData)
	{
		SafeDelete(i.second);
	}

	m_mapTileSetData.clear();

	cout << "���� �Ҹ���" << endl;
	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();

}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TILE_NAME, m_TileSetName);
	DDX_Control(pDX, IDC_TILESET_1, m_TileSetList);
	DDX_Text(pDX, IDC_TILESETSIZE, m_TileSetSize);
	DDX_Text(pDX, IDC_EDIT1, m_iTileSizeX);
	DDX_Text(pDX, IDC_EDIT2, m_iTileSizeY);
	DDX_Control(pDX, IDC_CHECK1, m_Instacne);
	DDX_Control(pDX, IDC_CHECK4, m_Collide);
	DDX_Control(pDX, IDC_COMBO1, m_Layer);
	DDX_Control(pDX, IDC_CHECK3, m_InvertY);
}

void CMapTool::Renew(XMFLOAT2 * tex)
{
	CString tileName;
	int iIndex = m_TileSetList.GetCurSel();
	m_TileSetList.GetText(iIndex, tileName);

	HRESULT hr = 0;
	m_texInfo = m_pTextureMgr->GetTexInfo(L"TILE_MAP", tileName.operator LPCWSTR());
	NULL_CHECK_MSG(m_texInfo, L"texinfo is nullptr");

	m_Tex[0] = tex[0];
	m_Tex[1] = tex[1];
	m_Tex[2] = tex[2];
	m_Tex[3] = tex[3];

	VertexUpdate();
	Invalidate(FALSE);
}

void CMapTool::VertexUpdate()
{
	if (m_Cam == nullptr)
		m_Cam = new CCamera;

	CRect rect;
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_TILE_VIEW);
	staticSize->GetClientRect(rect);

	int winX = (rect.right - rect.left);
	int winY = (rect.bottom - rect.top);

	m_Cam->Initialize(winX, winY, 0.0f, XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_Cam->Update();

	m_Vertex[0] = Vertex(-winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[0].x, m_Tex[0].y);
	m_Vertex[1] = Vertex(-winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[1].x, m_Tex[1].y);
	m_Vertex[2] = Vertex(winX*0.5f, winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[2].x, m_Tex[2].y);
	m_Vertex[3] = Vertex(winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[3].x, m_Tex[3].y);

	Vertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	v[0] = m_Vertex[0];
	v[1] = m_Vertex[1];
	v[2] = m_Vertex[2];
	v[3] = m_Vertex[3];

	m_pVB->Unlock();

	WORD* i = 0;
	m_pIB->Lock(0, 0, (void**)&i, 0);
	i[0] = 0, i[1] = 2, i[2] = 1;
	i[3] = 1, i[4] = 2, i[5] = 3;


	m_pIB->Unlock();
}

const XMFLOAT2 * CMapTool::GetTexPos()
{
	if (m_texInfo == nullptr)
		return nullptr;
	return m_Tex;
}

const CString & CMapTool::GetTileName()
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_TileName;
}

const XMFLOAT2 & CMapTool::GetTileSize()
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	m_fTileSize = XMFLOAT2((float)m_iTileSizeX,(float)m_iTileSizeY);
	return m_fTileSize;
}

void CMapTool::OnDraw(CDC * pDC)
{
}


BEGIN_MESSAGE_MAP(CMapTool, CDialogEx)
	ON_BN_CLICKED(IDC_FILE_OPEN, &CMapTool::OnBnClickedTileSetOpen)
	ON_BN_CLICKED(IDC_FILE_OPEN2, &CMapTool::OnBnClickedTileSetRemove)
	ON_BN_CLICKED(IDC_FILE_OPEN4, &CMapTool::OnBnClickedTileListSave)
	ON_BN_CLICKED(IDC_FILE_OPEN3, &CMapTool::OnBnClickedTileListLoad)
	ON_BN_CLICKED(IDC_MAP_LOAD, &CMapTool::OnBnClickedMapLoad)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_TILESET_1, &CMapTool::OnLbnDblclkTileList)
	ON_WM_DROPFILES()
	ON_STN_CLICKED(IDC_TILESETSIZE, &CMapTool::OnStnClickedTilesetsize)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_CHECK1, &CMapTool::OnBnClickedInstance)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnBnClickedTileSetOpen()
{

	static TCHAR BASED_CODE szFilter[] = _T("�̹�������(*.BMP,*.GIF,*JPG,*PNG)|*.BMP;,*.GIF;,*.JPG;,*PNG;*.bmp;*.jpg;*.gif;*.png|�������(*.*)|*.*||");
	CFileDialog dlg(true, _T("*.jpg"), _T("imamge"), OFN_HIDEREADONLY, szFilter);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Texture");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		TCHAR  pathDirectory[MAX_STR] = L"";
		TCHAR  pathRelative[MAX_STR] = L"";
		TCHAR  filePath[MAX_STR] = L"";
		TCHAR *fileName;

		lstrcpy(pathDirectory, (LPWSTR)(LPCTSTR)pathName);
		lstrcpy(filePath, (LPWSTR)(LPCTSTR)dlg.GetFileName());

		GetCurrentDirectory(sizeof(pathDirectory), pathDirectory);
		PathRelativePathTo(pathRelative, pathDirectory, FILE_ATTRIBUTE_DIRECTORY, (LPWSTR)(LPCTSTR)pathName, FILE_ATTRIBUTE_NORMAL);

		//Ȯ���� ����
		PathRemoveExtension(filePath);

		//���� �̸���
		fileName = PathFindFileName(filePath);

		CString mapName;
		CString tileName;
		m_TileSetName.GetWindowTextW(mapName);

		if (mapName == L"")
			tileName = fileName;
		else
			tileName = mapName;

		auto iter_find = m_mapTileSetData.find(tileName);
		if (m_mapTileSetData.end() != iter_find)
		{
			MessageBox(L"�ߺ��� ���� �ֽ��ϴ�.");
			return;
		}

		m_TileSetList.AddString(tileName);

		MAP_INFO* mapInfo = new MAP_INFO;
		mapInfo->s_fileName = fileName;
		mapInfo->s_filePath = pathRelative;
		mapInfo->i_Index = m_TileSetList.FindString(-1, tileName);

		m_mapTileSetData.insert({ tileName,mapInfo });

		HRESULT hr = 0;
		//�ؽ��� �Ŵ����� �Է�
		hr = m_pTextureMgr->LoadTexture(pathRelative, L"TILE_MAP", (LPWSTR)(LPCTSTR)tileName);
		FAILED_CHECK_MSG(hr, L"Adding TileMap Failed");
	}
}


void CMapTool::OnBnClickedTileSetRemove()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iIndex = m_TileSetList.GetCurSel();

	if (-1 == iIndex)
	{
		MessageBox(L"Wrong list Index");
		return;
	}

	CString tileName;
	m_TileSetList.GetText(iIndex, tileName);

	auto iter_find = m_mapTileSetData.find(tileName);

	if (m_mapTileSetData.end() == iter_find)
	{
		MessageBox(L"Wrong MapName");
		return;
	}

	m_TileSetList.DeleteString(iIndex);
	SafeDelete(iter_find->second);
	m_mapTileSetData.erase(iter_find);

	HRESULT hr = 0;
	hr = m_pTextureMgr->RemoveTexture(L"TILE_MAP", (LPWSTR)(LPCTSTR)tileName);
	FAILED_CHECK_MSG(hr, L"Remove TileMap Failed");

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	if (tileName == m_TileName)
	{
		m_texInfo = nullptr;
		m_TileName = L"";
		pMiniView->Clear();
	}
}


void CMapTool::OnBnClickedTileListSave()
{
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
	lstrcat(szCurrentPath, L"\\Texture\\map");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	if (dlg.DoModal() == IDOK)
	{
		m_strPath = dlg.GetPathName();
		if (m_strPath.Right(4) != ".dat")
		{
			m_strPath += ".dat";
		}
		file.Open(m_strPath, CFile::modeCreate | CFile::modeReadWrite, &ex);
		MAP_SAVE info;
		for (auto& i : m_mapTileSetData)
		{
			_tcscpy_s(info.objectkey, i.first);
			_tcscpy_s(info.fileName, i.second->s_fileName.c_str());
			_tcscpy_s(info.filePath, i.second->s_filePath.c_str());
			info.Index = i.second->i_Index;
			file.Write(&info, sizeof(MAP_SAVE));
		}
		file.Close();
		MessageBox(L"�������� ���������� ����", L"Success");
	}
}


void CMapTool::OnBnClickedTileListLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CStdioFile	file;
	CFileException kEx;
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_FILEMUSTEXIST, _T("DAT Files(*.dat)|*.dat|"), NULL);
	
	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Texture\\map");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

	CString m_strPath;

	if (dlg.DoModal() == IDOK)
	{
		if (!m_mapTileSetData.empty())
		{
			for (auto& i : m_mapTileSetData)
				SafeDelete(i.second);
			m_mapTileSetData.clear();
		}

		//���� �̸� ���
		m_strPath = dlg.GetPathName();
		file.Open(m_strPath, CFile::modeReadWrite, &kEx);
		MAP_SAVE info;

		while (file.Read(&info, sizeof(MAP_SAVE)))
		{
			MAP_INFO* tile = new MAP_INFO;
			tile->s_fileName = info.fileName;
			tile->s_filePath = info.filePath;
			tile->i_Index = info.Index;

			m_mapTileSetData.insert({ info.fileName,tile });

			m_TileSetList.InsertString(tile->i_Index, info.fileName);

			HRESULT hr = 0;
			//�ؽ��� �Ŵ����� �Է�
			hr = m_pTextureMgr->LoadTexture(tile->s_filePath, L"TILE_MAP", tile->s_fileName);
			FAILED_CHECK_MSG(hr, L"Adding TileMap Failed");
		}
		file.Close();
		MessageBox(L"�������� ���������� �ҷ���", L"Success");
	}
}


void CMapTool::OnBnClickedMapLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_SaveTool.ShowWindow(SW_SHOW);
}


BOOL CMapTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if (nullptr == m_SaveTool.GetSafeHwnd())
		m_SaveTool.Create(IDD_SAVETOOL);

	m_bIsInstance = false;

	RECT rc = {};
	GetClientRect(&rc);
	int winX = rc.right - rc.left;
	int winY = rc.bottom - rc.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_Cam->Initialize(winX, winY, 0.0f, XMFLOAT3(1.0f, 1.0f, 1.0f));

	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);

	//// use alpha channel in texture for alpha �̹������� ���İ� ������
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//// set blending factors so that alpha component determines transparency
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//����
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	m_pDeviceMgr->GetDevice()->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, FVF_VERTEX, D3DPOOL_MANAGED, &m_pVB, 0);
	m_pDeviceMgr->GetDevice()->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);


	m_Layer.AddString(L"LAYER_0");
	m_Layer.AddString(L"LAYER_1");
	m_Layer.AddString(L"LAYER_2");
	m_Layer.AddString(L"LAYER_3");
	m_Layer.AddString(L"LAYER_4");
	m_Layer.AddString(L"LAYER_5");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CMapTool::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	HWND hwnd;
	hwnd = (HWND)(*GetDlgItem(IDC_TILE_VIEW));

	m_pDeviceMgr->Render_Begin();
	if (m_texInfo != nullptr)
	{
		m_Cam->Update();
		m_Cam->SetTransform();

		m_pDeviceMgr->GetDevice()->SetStreamSource(0, m_pVB, 0, sizeof(Vertex));
		m_pDeviceMgr->GetDevice()->SetIndices(m_pIB);
		m_pDeviceMgr->GetDevice()->SetFVF(FVF_VERTEX);
		m_pDeviceMgr->GetDevice()->SetTexture(0, m_texInfo->pTexture);

		m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pDeviceMgr->GetDevice()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0,
			0,
			6,
			0,
			2
		);
		m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
	m_pDeviceMgr->Render_End(hwnd);
}
void CMapTool::OnLbnDblclkTileList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
	NULL_CHECK(pMiniView);

	CString tileName;
	int iIndex = m_TileSetList.GetCurSel();
	m_TileSetList.GetText(iIndex, tileName);

	if (tileName != m_TileName)
	{
		m_texInfo = nullptr;
	}

	pMiniView->Initialize(tileName);
	m_TileName = tileName;

	//Ÿ��ũ�� ����.
	m_texInfo = m_pTextureMgr->GetTexInfo(L"TILE_MAP", tileName.operator LPCWSTR());
	UpdateData(TRUE);

	int width = m_texInfo->tImgInfo.Width;
	int height = m_texInfo->tImgInfo.Height;
	m_TileSetSize.Format(L"%d x %d", width, height);
	
	UpdateData(FALSE);
	Invalidate(FALSE);
	

}
void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	//�ι�° ���ڰ� -1�ϰ�� �巡�� �� ��ӵ� ��ü ����� ������ ��ȯ�Ѵ�.
	int iCount = ::DragQueryFile(hDropInfo, -1, nullptr, 0);
	TCHAR szFilePath[MAX_STR] = L"";
	TCHAR szFileName[MAX_STR] = L"";
	TCHAR* fileName;
	for (int i = 0; i < iCount; ++i)
	{
		::DragQueryFile(hDropInfo, i, szFilePath, MAX_STR);
		//Ȯ���� ����
		lstrcpy(szFileName, szFilePath);
		PathRemoveExtension(szFileName);
		//������� �����̸���
		fileName = PathFindFileName(szFileName);

		//���� ���� �ʿ� �̹� �ҷ��������. ��ŵ
		auto iter_find = m_mapTileSetData.find(fileName);
		if (m_mapTileSetData.end() != iter_find)
			continue;
		else
		{
			WCHAR  pathDirectory[MAX_STR] = L"";
			WCHAR  pathRelative[MAX_STR] = L"";
			lstrcpy(pathDirectory, szFilePath);

			GetCurrentDirectory(sizeof(pathDirectory), pathDirectory);
			PathRelativePathTo(pathRelative, pathDirectory, FILE_ATTRIBUTE_DIRECTORY, szFilePath, FILE_ATTRIBUTE_NORMAL);

			//����Ʈ�� �߰�
			m_TileSetList.AddString(fileName);

			//�����̳ʿ� �߰�.
			MAP_INFO* mapInfo = new MAP_INFO;
			mapInfo->s_fileName = fileName;
			mapInfo->s_filePath = pathRelative;
			mapInfo->i_Index = m_TileSetList.FindString(-1, fileName);

			m_mapTileSetData.insert({ fileName,mapInfo });

			HRESULT hr = 0;
			//�ؽ��� �Ŵ����� �Է�
			hr = m_pTextureMgr->LoadTexture(pathRelative, L"TILE_MAP", (LPWSTR)(LPCTSTR)fileName);
			FAILED_CHECK_MSG(hr, L"Adding TileMap Failed");
		}
	}
	CDC* pDC = m_TileSetList.GetDC();
	CSize stringpixel;
	int iCX = 0;

	CString strFileName = L"";

	for (int i = 0; i < m_TileSetList.GetCount(); ++i)
	{
		m_TileSetList.GetText(i, strFileName);

		//GetTextExtend : ���ڷ� �Ѱܹ��� ���ڿ��� cx,cy �ȼ� ����� ��ȯ�Ѵ�.
		stringpixel = pDC->GetTextExtent(strFileName);

		if (iCX < stringpixel.cx)
			iCX = stringpixel.cx;
	}

	//GetHorizontalExtend <- ���� ����Ʈ�ڽ��� ������ �մ� ���� ��ũ�� �ִ� ������ �� ������ Ȯ���϶�.
	//�� ���Լ��� ���� ��ũ�� ������ ����� ��ȯ�Ѵ�.
	if (iCX > m_TileSetList.GetHorizontalExtent())
	{
		m_TileSetList.SetHorizontalExtent(iCX);
	}
	ReleaseDC(pDC);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CMapTool::OnStnClickedTilesetsize()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


BOOL CMapTool::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//����Ű ��������!
		if (pMsg->wParam == VK_RETURN)
		{
			CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
			NULL_CHECK_MSG(pFrameWnd,L"Map Tool pFrameWnd nullptr");

			CMiniView* pMiniView = dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
			NULL_CHECK_MSG(pMiniView,L"Map Tool pMiniView nullptr");

			cout << "Ÿ��ũ�� ����" << endl;
			UpdateData(TRUE);
			pMiniView->SetTileSize(m_iTileSizeX, m_iTileSizeY);
			UpdateData(FALSE);
		}
	}

	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMapTool::OnBnClickedInstance()
{
	if (m_Instacne.GetCheck() == 0)
	{
		m_bIsInstance = false;
	}
	else
	{
		m_bIsInstance = true;
	}
}
