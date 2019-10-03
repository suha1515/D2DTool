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
	afx_msg void OnLbnDblclkAnimlist();
	afx_msg void OnBnClickedAddAnim();
	afx_msg void OnBnClickedAnimload();
public:
	CAnimationFrame			m_AnimMaker;
public:
	virtual BOOL OnInitDialog();
	// 애니메이션 리스트
	CListBox m_AnimList;
	// 클립 리스트
	CListBox m_ClipList;


private:
	map<CString, vector<ANIM_CLIP>>				m_AnimationList;
	//map<CString, map<CString, vector<ANIM_CLIP>>>	m_AnimationList;

public:
	// 애니메이션 종류 이름
	CString m_AnimName;
	//클립 리스트 클립네임
	CString m_ClipName;
	afx_msg void OnLbnDblclkCliplist();
	afx_msg void OnBnClickedClipAdd();

	afx_msg void OnBnClickedClipSave();
	afx_msg void OnBnClickedSaveAnim();
	afx_msg void OnBnClickedListLoad();

public:
	void		AnimationLoad(const wstring& filePath);
	void		AnimationSave(const wstring& filePath,const wstring& anim);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedRemoveAnim();
	afx_msg void OnBnClickedRemoveClip();
};
