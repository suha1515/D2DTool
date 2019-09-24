// MyForm.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MyTool.h"
#include "MyForm.h"
#include "Camera.h"

#include "MainFrm.h"
#include "MiniView.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM),
	m_pDeviceMgr(CDeviceMgr::GetInstance()),
	m_pTextureMgr(CTextureMgr::GetInstance()),
	m_pVB(nullptr),m_pIB(nullptr),m_Cam(nullptr)
{

}

CMyForm::~CMyForm()
{
	delete m_Cam;
	if (m_pIB!=nullptr&&m_pIB->Release())
		MessageBox(0, L"m_pIB Release Failed", MB_OK);
	if (m_pVB != nullptr&&m_pVB->Release())
		MessageBox(0, L"m_pVB Release Failed", MB_OK);

	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();

	//��Ÿ�ϼ� ���� ����

	for (auto& i : m_mapTileSetData)
	{
		SafeDelete(i.second);
		NULL_CHECK_MSG(i.second, L"mapTileSetData delete Failed");
	}
		
	m_mapTileSetData.clear();
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEX_VALUE, m_TexValue);
	DDX_Control(pDX, IDC_TILESET_1, m_TileSetList);
	DDX_Control(pDX, IDC_TILE_NAME, m_TileNameEdit);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnBnClickedUnitTool)
	ON_BN_CLICKED(IDC_FILE_OPEN, &CMyForm::OnBnClickedFileOpen)
	ON_STN_CLICKED(IDC_TEX_VALUE, &CMyForm::OnStnClickedTexValue)
	//ON_EN_CHANGE(IDC_EDIT1, &CMyForm::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_FILE_OPEN2, &CMyForm::OnBnClickedDeleteMap)
	ON_LBN_SELCHANGE(IDC_TILESET_1, &CMyForm::OnLbnSelchangeTileClick)
END_MESSAGE_MAP()


// CMyForm �����Դϴ�.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm �޽��� ó�����Դϴ�.


void CMyForm::OnBnClickedUnitTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_UnitTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	//���̾�α� ����
	if (nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);

	

	RECT rc = {};
	GetClientRect(&rc);
	float winX = rc.right - rc.left;
	float winY = rc.bottom - rc.top;

	if (m_Cam == nullptr)
		m_Cam = new CCamera;
	m_Cam = new CCamera;
	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));

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
}


void CMyForm::OnDraw(CDC* /*pDC*/)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	

}

void CMyForm::Renew(XMFLOAT2 * tex)
{
	//m_TexValue.SetWindowTextW(str);
	CString tileName;
	int iIndex = m_TileSetList.GetCurSel();
	m_TileSetList.GetText(iIndex, tileName);

	HRESULT hr = 0;
	m_texInfo = m_pTextureMgr->GetTexInfo(L"MAP_TILE", tileName.operator LPCWSTR());
	NULL_CHECK_MSG(m_texInfo, L"texinfo is nullptr");

	m_Tex[0] = tex[0];
	m_Tex[1] = tex[1];
	m_Tex[2] = tex[2];
	m_Tex[3] = tex[3];

	VertexUpdate();
	//��ó ��Ʈ�� ���̾�α�.
	HWND hwnd;
	hwnd = (HWND)(*GetDlgItem(IDC_TILE_VIEW));

	m_Cam->Update();
	m_Cam->SetTransform();
	m_pDeviceMgr->Render_Begin();

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
	m_pDeviceMgr->Render_End(hwnd);
}

void CMyForm::VertexUpdate()
{
	if (m_pVB != nullptr)
		m_pVB->Release();
	if (m_pIB != nullptr)
		m_pIB->Release();
	if (m_Cam == nullptr)
		m_Cam = new CCamera;

	CRect rect;
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_TILE_VIEW);
	staticSize->GetClientRect(rect);

	float winX = (rect.right - rect.left);
	float winY = (rect.bottom - rect.top);

	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(1.0f, 1.0f, 1.0f));
	m_Cam->Update();

	m_Vertex[0] = Vertex(-winX*0.5f,  winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[0].x, m_Tex[0].y);
	m_Vertex[1] = Vertex(-winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[1].x, m_Tex[1].y);
	m_Vertex[2] = Vertex( winX*0.5f,  winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[2].x, m_Tex[2].y);
	m_Vertex[3] = Vertex( winX*0.5f, -winY*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[3].x, m_Tex[3].y);

	m_pDeviceMgr->GetDevice()->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY,FVF_VERTEX, D3DPOOL_MANAGED,&m_pVB,0);
	Vertex* v;
	m_pVB->Lock(0, 0, (void**)&v, 0);

	v[0] = m_Vertex[0];
	v[1] = m_Vertex[1];
	v[2] = m_Vertex[2];
	v[3] = m_Vertex[3];

	m_pVB->Unlock();
	m_pDeviceMgr->GetDevice()->CreateIndexBuffer(6 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);

	WORD* i = 0;
	m_pIB->Lock(0, 0, (void**)&i, 0);
	i[0] = 0, i[1] = 2, i[2] = 1;
	i[3] = 1, i[4] = 2, i[5] = 3;


	m_pIB->Unlock();
}

//�� ��������
void CMyForm::OnBnClickedFileOpen()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	static TCHAR BASED_CODE szFilter[] = _T("�̹�������(*.BMP,*.GIF,*JPG,*PNG)|*.BMP;,*.GIF;,*.JPG;,*PNG;*.bmp;*.jpg;*.gif;*.png|�������(*.*)|*.*||");
	CFileDialog dlg(true, _T("*.jpg"), _T("imamge"), OFN_HIDEREADONLY, szFilter);

	if (IDOK == dlg.DoModal())
	{
		CString pathName = dlg.GetPathName();
		WCHAR  pathDirectory[MAX_STR] = L"";
		WCHAR  pathRelative[MAX_STR] = L"";
		WCHAR  fileName[MAX_STR] = L"";
		
		lstrcpy(pathDirectory, (LPWSTR)(LPCTSTR)pathName);
		lstrcpy(fileName, (LPWSTR)(LPCTSTR)dlg.GetFileName());

		GetCurrentDirectory(sizeof(pathDirectory), pathDirectory);
		PathRelativePathTo(pathRelative, pathDirectory, FILE_ATTRIBUTE_DIRECTORY, (LPWSTR)(LPCTSTR)pathName, FILE_ATTRIBUTE_NORMAL);

		CString mapName;
		CString tileName;
		m_TileNameEdit.GetWindowTextW(mapName);

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
		mapInfo->i_Index = m_TileSetList.FindString(-1,tileName);

		m_mapTileSetData.insert({ tileName,mapInfo });

		HRESULT hr = 0;
		//�ؽ��� �Ŵ����� �Է�
		hr = m_pTextureMgr->LoadTexture(pathRelative, L"TILE_MAP", (LPWSTR)(LPCTSTR)tileName);
		FAILED_CHECK_MSG(hr, L"Adding TileMap Failed");



	}
}


void CMyForm::OnStnClickedTexValue()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CMyForm::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CFormView::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//�� ����Ʈ ����.
void CMyForm::OnBnClickedDeleteMap()
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

	pMiniView->Clear();

}

//�ʸ���Ʈ Ŭ����
void CMyForm::OnLbnSelchangeTileClick()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	NULL_CHECK(pFrameWnd);

	CMiniView* pMiniView= dynamic_cast<CMiniView*>(pFrameWnd->m_SecondSplitter.GetPane(0, 0));
	NULL_CHECK(pMiniView);
	CString tileName;
	int iIndex = m_TileSetList.GetCurSel();
	m_TileSetList.GetText(iIndex, tileName);


	pMiniView->Initialize(tileName);

}
