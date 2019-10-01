#pragma once
#include "afxwin.h"


// CAnimationFrame 대화 상자입니다.
class CGameObject;
class CAnimationFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationFrame)

public:
	CAnimationFrame(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationFrame();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMFRAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// //애니메이션 클립 리스트
	CListBox m_ClipList;
	// 애니메이션 위치
	float m_PositionX;
	float m_PositionY;
	float m_PositionZ;

	// //애니메이션 회전
	float m_RotationX;
	float m_RotationY;
	float m_RotationZ;
	// //애니메이션 크기
	float m_ScaleX;
	float m_ScaleY;
	float m_ScaleZ;
	// //애니메이션 재생 시간
	float m_PlayTime;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);


public:
	void				Update();
private:
	CGameObject*		m_ClickedObject;

};
