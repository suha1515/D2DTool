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

bool FindIter(CLIP_INFO info,int index)
{
	if (info._index == index)
		return true;
	return false;
}


void CAnimationFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	
}

void CAnimationFrame::UpDateInfo(ANIM_CLIP & clipinfo)
{
	UpdateData(TRUE);

	//툴에서 클립의 첫번째 클립의 정보로 채운다.
	vector<CLIP_INFO>::iterator iter_begin = clipinfo._clips.begin();
	m_ClipName = clipinfo._clipName;
	wcout << m_ClipName.operator LPCWSTR() << endl;
	
	if (!clipinfo._clips.empty())
	{
		//위치 정보
		m_PositionX		= iter_begin->_pos.x;
		m_PositionY		= iter_begin->_pos.y;
		m_PositionZ		= iter_begin->_pos.z;

		//회전 정보
		m_RotationX		= iter_begin->_rot.x;
		m_RotationY		= iter_begin->_rot.y;
		m_RotationZ		= iter_begin->_rot.z;

		//크기 정보
		m_ScaleX		= iter_begin->_Scale.x;
		m_ScaleY		= iter_begin->_Scale.y;
		m_ScaleZ		= iter_begin->_Scale.z;

		//텍스처 정보.
		m_TexName		= iter_begin->_texName;
		m_TextureSizeX  = iter_begin->_texSizeX;
		m_TextureSizeY  = iter_begin->_texSizeY;

		//재생 시간
		m_PlayTime		= iter_begin->_startTime;

		//이미지 정보 갱신 
		XMFLOAT2 size = XMFLOAT2(m_TextureSizeX, m_TextureSizeY);
		SetTexture(m_TexName, iter_begin->_Tex, &size);

		//총 시간
		float total=0.0f;
		for (auto& i : clipinfo._clips)
		{
			total += i._startTime;
		}
			

		m_EndTime = total;
	}
	//만약 클립내용이 하나도 없을경우.
	else
	{
		//위치 정보
		m_PositionX = 0.0f;
		m_PositionY = 0.0f;
		m_PositionZ = 0.0f;

		//회전 정보
		m_RotationX = 0.0f;
		m_RotationY = 0.0f;
		m_RotationZ = 0.0f;

		//크기 정보
		m_ScaleX = 0.0f;
		m_ScaleY = 0.0f;
		m_ScaleZ = 0.0f;

		//텍스처 정보.
		m_TexName = L"";
		m_TextureSizeX = 0.0f;
		m_TextureSizeY = 0.0f;

		//재생 시간
		m_PlayTime = 0.0f;

		cout << "클립이 비었습니다" << endl;
	}
	UpdateData(FALSE);
}

void CAnimationFrame::SetTexture(const CString& clipName, const XMFLOAT2 * tex,const XMFLOAT2* size)
{
	HRESULT hr = 0;
	m_texInfo = m_pTextureMgr->GetTexInfo(L"TILE_MAP", clipName.operator LPCWSTR());
	NULL_CHECK_MSG_RETURN(m_texInfo, L"AnimFrame 텍스처 정보가 없습니다");

	m_Tex[0] = tex[0];
	m_Tex[1] = tex[1];
	m_Tex[2] = tex[2];
	m_Tex[3] = tex[3];

	m_TexName = m_texInfo->textureName.c_str();
	m_Size = *size;

	m_TextureName  = m_TexName;
	m_TextureSizeX = size->x;
	m_TextureSizeY = size->y;

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
	//벡터 시작 - 끝 0,1,2,3,4  시작 0 끝 5

	if (m_startIndex < m_endIndex)
	{
		XMFLOAT2 size = XMFLOAT2(m_Clips[m_startIndex]._texSizeX, m_Clips[m_startIndex]._texSizeY);
		SetTexture(m_Clips[m_startIndex]._texName, m_Clips[m_startIndex]._Tex, &size);
		if (m_time >= m_Clips[m_startIndex]._startTime)
		{
			m_startIndex++;
			m_time -= m_time;
		}
	}
	else
	{ 
		m_startIndex = 0;
		if (!m_CheckLoop.GetCheck())
		{
			XMFLOAT2 size = XMFLOAT2(m_Clips[m_startIndex]._texSizeX, m_Clips[m_startIndex]._texSizeY);
			SetTexture(m_Clips[m_startIndex]._texName, m_Clips[m_startIndex]._Tex, &size);
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

void CAnimationFrame::Renew()
{
	//리스트 박스 갱신
	m_ClipList.ResetContent();
	//인덱스 갱신
	int index = 0;
	wcout << m_ClipName.operator LPCWSTR() << endl;
	for (auto& i  : m_Clips)
	{
		i._index = index;
		index++;

		//리스트 박스 원소추가
		CString clipName;
		clipName.Format(L"%s_%d", m_ClipName, index);
		m_ClipList.AddString(clipName);
	}
}

void CAnimationFrame::LoadClip(ANIM_CLIP& clipinfo)
{
	if (m_Clip != nullptr)
		m_Clip = nullptr;

	m_Clip  = &clipinfo;
	m_Clips = m_Clip->_clips;

	UpDateInfo(clipinfo);

	Renew();
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
	CLIP_INFO clip;
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

	//리스트 항목을 선택했을경우. ( 중간삽입 항목아이템 바로아래 , 그리고 나머지 아이템도 갱신해야함)
	int index = m_ClipList.GetCurSel();
	if (index != -1)
	{
		auto iter_find = find_if(m_Clips.begin(), m_Clips.end(), 
			[&index](CLIP_INFO info)
			{
			if (info._index == index)
				return true;
			return false;
			}
		);
		if (m_Clips.end() == iter_find)
		{
			MessageBox(L"리스트 항목을 찾을수 없습니다", L"ANIMFRAME");
			return;
		}
		iter_find++;
		m_Clips.insert(iter_find, clip);
	}
	//인덱스가 마지막이거나//선택하지 않은 경우는 새로 생성한다. 바로 끝자락에
	else if (index == m_ClipList.GetCount()-1||index == -1)
	{
		m_Clips.push_back(clip);
	}
	Renew();
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
		
		//해당 인덱스에 근거해 벡터를 지운다.
		auto iter_find = find_if(m_Clips.begin(), m_Clips.end(),
			[&index](CLIP_INFO info)
		{
			if (info._index == index)
				return true;
			return false;
		}
		);

		m_Clips.erase(iter_find);
		//중간 원소를 지웠을수 있으므로 다시 리스트 갱신
		Renew();
	}
}


void CAnimationFrame::OnBnClickedAnimationSave()
{
	//따로 클립별 저장이아니다

	//수정한 클립들을 저장시켜준다.
	m_Clip->_clips = m_Clips;
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
			m_startIndex = 0;
			m_endIndex = m_Clips.size();
			m_IsPlaying = true;
		}
	}
		
}


void CAnimationFrame::OnBnClickedReWind()
{
	/*auto& iter_begin = m_Clips.begin();
	XMFLOAT2		m_size = XMFLOAT2(iter_begin->second._texSizeX, iter_begin->second._texSizeY);
	SetTexture(iter_begin->second._texName , iter_begin->second._Tex, &m_size);
*/
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
		auto iter_find = find_if(m_Clips.begin(), m_Clips.end(),
			[&index](CLIP_INFO info)
		{
			if (info._index == index)
				return true;
			return false;
		}
		);

		if (iter_find == m_Clips.end())
			return;
		
		CLIP_INFO info = *iter_find;
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

				CLIP_INFO clip;
				UpdateData(TRUE);
				clip._pos = D3DXVECTOR3(m_PositionX, m_PositionY, m_PositionZ);			//애니메이션 위치
				clip._rot = XMFLOAT3(m_RotationX, m_RotationY, m_RotationZ);			//애니메이션 회전
				clip._Scale = D3DXVECTOR3(m_ScaleX, m_ScaleY, m_ScaleZ);				//애니메이션 크기
				clip._startTime = m_PlayTime;											//애니메이션 재생시간

				clip._Tex[0] = m_Tex[0];												//애니메이션 텍스쳐
				clip._Tex[1] = m_Tex[1];
				clip._Tex[2] = m_Tex[2];
				clip._Tex[3] = m_Tex[3];

				clip._texSizeX = m_Size.x;
				clip._texSizeY = m_Size.y;

				m_Clips[index] = clip;

				UpdateData(FALSE);
			}
		}
	}

	//엔터키에 안꺼지게
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}
