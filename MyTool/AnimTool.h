#pragma once

#include "AnimationFrame.h"
#include "afxwin.h"

// CAnimTool ��ȭ �����Դϴ�.

class CAnimTool : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimTool)

public:
	CAnimTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpentool();

public:
	CAnimationFrame			m_AnimMaker;
public:
	virtual BOOL OnInitDialog();
	// //�ִϸ��̼� ����Ʈ
	CListBox m_AnimList;
	afx_msg void OnBnClickedAnimload();

private:
	map<CString, vector<ANIM_CLIP>>		m_AnimationList;
public:
	afx_msg void OnLbnDblclkAnimlist();
};
