
// MyTool.h : MyTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMyToolApp:
// �� Ŭ������ ������ ���ؼ��� MyTool.cpp�� �����Ͻʽÿ�.
//

class CMyToolView;
class CMainFrame;
class CMyToolApp : public CWinAppEx
{
public:
	CMyToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);


	//������ �Ŵ��� , ����������
private:
	CMainFrame*		m_pMain;
	CFrameMgr*		m_pFrameMgr;
	CMyToolView*	m_pToolView;
};

extern CMyToolApp theApp;
