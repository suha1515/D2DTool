#pragma once



// CInspectView �� ���Դϴ�.

class CInspectView : public CFormView
{
	DECLARE_DYNCREATE(CInspectView)

protected:
	CInspectView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CInspectView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSPECTOR };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


