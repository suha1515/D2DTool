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
	afx_msg void OnLbnDblclkAnimlist();
	afx_msg void OnBnClickedAddAnim();
	afx_msg void OnBnClickedAnimload();
public:
	CAnimationFrame			m_AnimMaker;
public:
	virtual BOOL OnInitDialog();
	// �ִϸ��̼� ����Ʈ
	CListBox m_AnimList;
	// Ŭ�� ����Ʈ
	CListBox m_ClipList;


private:
	map<CString, vector<ANIM_CLIP>>				m_AnimationList;
	//map<CString, map<CString, vector<ANIM_CLIP>>>	m_AnimationList;

public:
	// �ִϸ��̼� ���� �̸�
	CString m_AnimName;
	//Ŭ�� ����Ʈ Ŭ������
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
