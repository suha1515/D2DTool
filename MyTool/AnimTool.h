#pragma once


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
};
