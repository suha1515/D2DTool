#pragma once

#include "AnimationFrame.h"
#include "afxwin.h"

// CAnimTool 대화 상자입니다.

class CAnimTool : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimTool)

public:
	CAnimTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpentool();

public:
	CAnimationFrame			m_AnimMaker;
public:
	virtual BOOL OnInitDialog();
	// //애니메이션 리스트
	CListBox m_AnimList;
	afx_msg void OnBnClickedAnimload();

private:
	map<CString, vector<ANIM_CLIP>>		m_AnimationList;
public:
	afx_msg void OnLbnDblclkAnimlist();
};
