// AnimationFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "AnimationFrame.h"
#include "afxdialogex.h"

#include "Camera.h"


// CAnimationFrame 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationFrame, CDialogEx)

CAnimationFrame::CAnimationFrame(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANIMFRAME, pParent)
	, m_PositionX(0)
	, m_PositionZ(0)
	, m_PositionY(0)
	, m_RotationX(0)
	, m_RotationY(0)
	, m_RotationZ(0)
	, m_ScaleX(0)
	, m_ScaleY(0)
	, m_ScaleZ(0)
	, m_PlayTime(0)
	, m_ClipName(_T(""))
	,m_pDeviceMgr(CDeviceMgr::GetInstance()),m_pTextureMgr(CTextureMgr::GetInstance())
	,m_pVB(nullptr),m_pIB(nullptr)
	,m_Cam(nullptr),m_texInfo(nullptr)
	, m_TextureName(_T(""))
	, m_TextureSizeX(0)
	, m_TextureSizeY(0)
{

}

CAnimationFrame::~CAnimationFrame()
{
	if (m_Cam != nullptr)
		delete m_Cam;
	if (m_pIB != nullptr)
		m_pIB->Release();
	if (m_pVB != nullptr)
		m_pVB->Release();
}

void CAnimationFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLIPLIST, m_ClipList);
	DDX_Text(pDX, IDC_CLIP_POS_X, m_PositionX);
	DDX_Text(pDX, IDC_CLIP_POS_Y, m_PositionY);
	DDX_Text(pDX, IDC_CLIP_POS_Z, m_PositionZ);

	DDX_Text(pDX, IDC_CLIP_ROT_X, m_RotationX);
	DDX_Text(pDX, IDC_CLIP_ROT_Y, m_RotationY);
	DDX_Text(pDX, IDC_CLIP_ROT_Z, m_RotationZ);
	DDX_Text(pDX, IDC_CLIP_SIZE_X, m_ScaleX);
	DDX_Text(pDX, IDC_CLIP_SIZE_Y, m_ScaleY);
	DDX_Text(pDX, IDC_CLIP_SIZE_Z, m_ScaleZ);
	DDX_Text(pDX, IDC_PLAY_TIME, m_PlayTime);
	DDX_Text(pDX, IDC_EDIT1, m_ClipName);
	DDX_Text(pDX, IDC_TEXTURE_NAME, m_TextureName);
	DDX_Text(pDX, IDC_TEXTURE_SIZE_X, m_TextureSizeX);
	DDX_Text(pDX, IDC_TEXTURE_SIZE_Y, m_TextureSizeY);
	DDX_Control(pDX, IDC_CHECK1, m_CheckLoop);
}


BEGIN_MESSAGE_MAP(CAnimationFrame, CDialogEx)
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimationFrame::OnBnClickedClipAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnimationFrame::OnBnClickedClipRemove)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnimationFrame::OnBnClickedAnimationSave)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimationFrame::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON5, &CAnimationFrame::OnBnClickedReWind)
	ON_BN_CLICKED(IDC_BUTTON6, &CAnimationFrame::OnBnClickedPause)
	ON_LBN_DBLCLK(IDC_CLIPLIST, &CAnimationFrame::OnLbnDblclkCliplist)
END_MESSAGE_MAP()


// CAnimationFrame 메시지 처리기입니다.


void CAnimationFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	
}

void CAnimationFrame::SetTexture(const CString& clipName, const XMFLOAT2 * tex,const XMFLOAT2* size)
{

	HRESULT hr = 0;
	m_texInfo = m_pTextureMgr->GetTexInfo(L"TILE_MAP", clipName.operator LPCWSTR());
	NULL_CHECK_MSG(m_texInfo, L"AnimFrame texinfo is nullptr");

	m_Tex[0] = tex[0];
	m_Tex[1] = tex[1];
	m_Tex[2] = tex[2];
	m_Tex[3] = tex[3];

	m_TexName = m_texInfo->textureName.c_str();
	m_Size = *size;

	UpdateData(TRUE);

	m_TextureName  = m_TexName;
	m_TextureSizeX = size->x;
	m_TextureSizeY = size->y;

	UpdateData(FALSE);



	VertexUpdate();
	Invalidate(FALSE);
}

void CAnimationFrame::Initialize()
{
	if (m_pVB != nullptr)
		m_pVB->Release();
	if (m_pIB != nullptr)
		m_pIB->Release();
	if (m_Cam == nullptr)
		m_Cam = new CCamera;

	CRect rect;
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_ANIMIMG);
	staticSize->GetClientRect(rect);

	float winX = (rect.right - rect.left);
	float winY = (rect.bottom - rect.top);

	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(2.0f, 2.0f, 1.0f));
	m_Cam->Update();

	m_Size.x = 16.f;
	m_Size.y = 16.f;

	m_Vertex[0] = Vertex(-m_Size.x*0.5f,  m_Size.y*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[0].x, m_Tex[0].y);
	m_Vertex[1] = Vertex(-m_Size.x*0.5f, -m_Size.y*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[1].x, m_Tex[1].y);
	m_Vertex[2] = Vertex( m_Size.x*0.5f,  m_Size.y*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[2].x, m_Tex[2].y);
	m_Vertex[3] = Vertex( m_Size.x*0.5f, -m_Size.y*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[3].x, m_Tex[3].y);

	m_pDeviceMgr->GetDevice()->CreateVertexBuffer(4 * sizeof(Vertex), D3DUSAGE_WRITEONLY, FVF_VERTEX, D3DPOOL_MANAGED, &m_pVB, 0);
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

void CAnimationFrame::Update()
{
	if (m_StartIndex != m_EndIndex)
	{
		XMFLOAT2 size = XMFLOAT2(m_StartIndex->second._texSizeX, m_StartIndex->second._texSizeY);
		SetTexture(m_StartIndex->second._texName, m_StartIndex->second._Tex, &size);
		if (m_time >= m_StartIndex->second._startTime)
		{
			m_StartIndex++;
			m_time -= m_time;
		}
	}
	else
	{
		m_StartIndex = m_Clips.begin();
		if (!m_CheckLoop.GetCheck())
		{
			XMFLOAT2 size = XMFLOAT2(m_StartIndex->second._texSizeX, m_StartIndex->second._texSizeY);
			SetTexture(m_StartIndex->second._texName, m_StartIndex->second._Tex, &size);
			m_IsPlaying = false;
		}
	}

	Render();
	m_time += CTimeMgr::GetInstance()->GetDeltaTime();
}

void CAnimationFrame::Render()
{
	HWND hwnd;
	hwnd = (HWND)(*GetDlgItem(IDC_ANIMIMG));

	m_pDeviceMgr->Render_Begin();

	if (m_texInfo != nullptr)
	{
		m_Cam->Update();
		m_Cam->SetTransform();

		D3DXMATRIX world = m_ScaleMat * m_Rotmat * m_TransMat;
		m_pDeviceMgr->GetDevice()->SetTransform(D3DTS_WORLD, &world);

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


void CAnimationFrame::VertexUpdate()
{
	CRect rect;
	CStatic* staticSize = (CStatic*)GetDlgItem(IDC_ANIMIMG);
	staticSize->GetClientRect(rect);

	float winX = (rect.right - rect.left);
	float winY = (rect.bottom - rect.top);

	m_Cam->Initialize(winX, winY, 0, XMFLOAT3(2.0f, 2.0f, 1.0f));
	m_Cam->Update();

	m_Vertex[0] = Vertex(-m_Size.x*0.5f,  m_Size.y*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[0].x, m_Tex[0].y);
	m_Vertex[1] = Vertex(-m_Size.x*0.5f, -m_Size.y*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[1].x, m_Tex[1].y);
	m_Vertex[2] = Vertex( m_Size.x*0.5f,  m_Size.y*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[2].x, m_Tex[2].y);
	m_Vertex[3] = Vertex( m_Size.x*0.5f, -m_Size.y*0.5f, 0.0f, 0.0f, 0.0f, 0.0f, m_Tex[3].x, m_Tex[3].y);

	Vertex* v;
	m_pVB->Lock(0, 0, (void**)&v, D3DLOCK_DISCARD);
	v[0] = m_Vertex[0];
	v[1] = m_Vertex[1];
	v[2] = m_Vertex[2];
	v[3] = m_Vertex[3];
	m_pVB->Unlock();
	WORD* i = 0;
	m_pIB->Lock(0, 0, (void**)&i, D3DLOCK_DISCARD);
	i[0] = 0, i[1] = 2, i[2] = 1;
	i[3] = 1, i[4] = 2, i[5] = 3;


	m_pIB->Unlock();
}

void CAnimationFrame::LoadAnimation(const CString& clipName, const vector<ANIM_CLIP>& clips)
{
	int index = 0;
	for (auto&i : clips)
	{
		CString name;
		name.Format(L"%s_%d", clipName, index);
		m_Clips[name] = i;
		m_ClipList.AddString(name);
		index++;
	}
}

bool CAnimationFrame::IsPlaying()
{
	return m_IsPlaying;
}


void CAnimationFrame::OnPaint()
{
	CPaintDC dc(this);

	HWND hwnd;
	hwnd = (HWND)(*GetDlgItem(IDC_ANIMIMG));

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


BOOL CAnimationFrame::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Initialize();

	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
	m_pDeviceMgr->GetDevice()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x000000ff);

	//// use alpha channel in texture for alpha 이미지에서 알파값 가져옴
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDeviceMgr->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//// set blending factors so that alpha component determines transparency
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//빛끔
	m_pDeviceMgr->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	

	m_IsPlaying = false;
	m_ReWind = false;
	
	m_time = 0.0f;
	m_EndTime = 0.0f;

	D3DXMatrixIdentity(&m_TransMat);
	D3DXMatrixIdentity(&m_Rotmat);
	D3DXMatrixIdentity(&m_ScaleMat);

	return TRUE;  
}


//클립 추가 버튼
void CAnimationFrame::OnBnClickedClipAdd()
{
	//클립정보를 종합한다.
	ANIM_CLIP clip;
	UpdateData(TRUE);
	clip._pos   = D3DXVECTOR3(m_PositionX, m_PositionY, m_PositionZ);		//애니메이션 위치
	clip._rot   = XMFLOAT3(m_RotationX, m_RotationY, m_RotationZ);			//애니메이션 회전
	clip._Scale = D3DXVECTOR3(m_ScaleX, m_ScaleY, m_ScaleZ);				//애니메이션 크기
	clip._startTime = m_PlayTime;											//애니메이션 재생시간

	clip._Tex[0] = m_Tex[0];												//애니메이션 텍스쳐
	clip._Tex[1] = m_Tex[1];
	clip._Tex[2] = m_Tex[2];
	clip._Tex[3] = m_Tex[3];

	lstrcpy(clip._texName, m_texInfo->textureName.c_str());
	clip._texSizeX = m_Size.x;
	clip._texSizeY = m_Size.y;

	CString clipName;
	int size = m_Clips.size();
	clipName.Format(L"%s_%d", m_ClipName, size);

	m_Clips.insert({ clipName,clip });
	m_ClipList.AddString(clipName);

	UpdateData(FALSE);
}


void CAnimationFrame::OnBnClickedClipRemove()
{
	int index = m_ClipList.GetCurSel();

	if (index != -1)
	{
		CString clipName;
		m_ClipList.GetText(index, clipName);

		m_ClipList.DeleteString(index);
		m_Clips.erase(clipName);
	}
}


void CAnimationFrame::OnBnClickedAnimationSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString m_strPath;
	CFile file;
	CFileException ex;
	CFileDialog dlg(FALSE, _T("*.anim"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Anim File(*.anim)|*.anim|"), NULL);

	if (dlg.DoModal() == IDOK)
	{
		m_strPath = dlg.GetPathName();
		if (m_strPath.Right(5) != ".anim")
		{
			m_strPath += ".anim";
		}
		file.Open(m_strPath, CFile::modeCreate | CFile::modeReadWrite, &ex);
		TCHAR _AnimName[256] = L"";
		lstrcpy(_AnimName, m_ClipName);
		ANIM_CLIP info;
		file.Write(&_AnimName, sizeof(_AnimName));
		for (auto& i : m_Clips)
		{
			info = i.second;
			file.Write(&info, sizeof(ANIM_CLIP));
		}
		file.Close();
		MessageBox(L"애니메이션 파일을 성공적으로 저장", L"Success");
	}
}


void CAnimationFrame::OnBnClickedPlay()
{
	if (m_Clips.empty())
	{
		::AfxMessageBox(L"애니메이션 클립이 없습니다!");
		m_IsPlaying = false;
	}
	else
	{
		if (m_IsPlaying != true)
		{
			for (auto& i : m_Clips)
				m_EndTime += i.second._startTime;
			m_StartIndex = m_Clips.begin();
			m_EndIndex = m_Clips.end();
			m_IsPlaying = true;
		}
	}
		
}


void CAnimationFrame::OnBnClickedReWind()
{
	auto& iter_begin = m_Clips.begin();
	XMFLOAT2		m_size = XMFLOAT2(iter_begin->second._texSizeX, iter_begin->second._texSizeY);
	SetTexture(iter_begin->second._texName , iter_begin->second._Tex, &m_size);

}


void CAnimationFrame::OnBnClickedPause()
{
	if (m_IsPlaying !=false)
	{
		m_IsPlaying = false;
	}
}


void CAnimationFrame::OnLbnDblclkCliplist()
{
	int index = m_ClipList.GetCurSel();

	if (index != -1)
	{
		CString clipName;
		m_ClipList.GetText(index, clipName);
		ANIM_CLIP info = m_Clips.find(clipName)->second;

		UpdateData(TRUE);

		m_PositionX = info._pos.x;
		m_PositionY = info._pos.y;
		m_PositionZ = info._pos.z;

		m_RotationX = info._rot.x;
		m_RotationY = info._rot.y;
		m_RotationZ = info._rot.z;

		m_ScaleX = info._Scale.x;
		m_ScaleY = info._Scale.y;
		m_ScaleZ = info._Scale.z;

		m_TextureName = info._texName;
		m_TextureSizeX = info._texSizeX;
		m_TextureSizeY = info._texSizeY;

		m_PlayTime = info._startTime;

		m_ClipName = clipName;
		UpdateData(FALSE);

		SetTexture(m_TextureName, info._Tex, &XMFLOAT2(m_TextureSizeX, m_TextureSizeY));
	}
}


BOOL CAnimationFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			int index = m_ClipList.GetCurSel();

			if (index != -1)
			{
				cout << "애니메이션 정보 갱신" << endl;
				//클립정보를 종합한다.
				CString clipName;
				m_ClipList.GetText(index, clipName);

				ANIM_CLIP clip;
				UpdateData(TRUE);
				clip._pos = D3DXVECTOR3(m_PositionX, m_PositionY, m_PositionZ);			//애니메이션 위치
				clip._rot = XMFLOAT3(m_RotationX, m_RotationY, m_RotationZ);			//애니메이션 회전
				clip._Scale = D3DXVECTOR3(m_ScaleX, m_ScaleY, m_ScaleZ);				//애니메이션 크기
				clip._startTime = m_PlayTime;											//애니메이션 재생시간

				clip._Tex[0] = m_Tex[0];												//애니메이션 텍스쳐
				clip._Tex[1] = m_Tex[1];
				clip._Tex[2] = m_Tex[2];
				clip._Tex[3] = m_Tex[3];

				lstrcpy(clip._texName, m_texInfo->textureName.c_str());
				clip._texSizeX = m_Size.x;
				clip._texSizeY = m_Size.y;

				m_Clips[clipName] = clip;
				UpdateData(FALSE);
			}
		}
	}

	//엔터키에 안꺼지게
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}
