// AnimationFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyTool.h"
#include "AnimationFrame.h"
#include "afxdialogex.h"


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
{

}

CAnimationFrame::~CAnimationFrame()
{
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
}


BEGIN_MESSAGE_MAP(CAnimationFrame, CDialogEx)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CAnimationFrame 메시지 처리기입니다.


void CAnimationFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	
	//포커스를 감지한다.
	switch (nState)
	{
	case WA_INACTIVE:		//포커스를 잃었을때
		break;	
	case WA_ACTIVE:			//포커스를 얻었을때 (마우스를 이용한것 제외)
		break;
	case WA_CLICKACTIVE:	//포커스를 얻었을때 (마우스를 이용하여)
		Update();
		break;
	}
}

void CAnimationFrame::Update()
{

}
