
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
};

extern CMyToolApp theApp;
