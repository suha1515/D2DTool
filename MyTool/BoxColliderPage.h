#pragma once


// CBoxColliderPage ��ȭ �����Դϴ�.

class CBoxColliderPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBoxColliderPage)

public:
	CBoxColliderPage();
	virtual ~CBoxColliderPage();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOXCOLLIDERPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
