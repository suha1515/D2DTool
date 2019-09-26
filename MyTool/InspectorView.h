#pragma once


// CInspectorView 대화 상자입니다.

class CInspectorView : public CDialogEx
{
	DECLARE_DYNAMIC(CInspectorView)

public:
	CInspectorView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInspectorView();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSPECTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
