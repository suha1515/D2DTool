#pragma once


// CInspectorView ��ȭ �����Դϴ�.

class CInspectorView : public CDialogEx
{
	DECLARE_DYNAMIC(CInspectorView)

public:
	CInspectorView(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInspectorView();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSPECTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
