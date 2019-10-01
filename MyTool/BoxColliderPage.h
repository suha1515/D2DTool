#pragma once


// CBoxColliderPage 대화 상자입니다.

class CBoxColliderPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBoxColliderPage)

public:
	CBoxColliderPage();
	virtual ~CBoxColliderPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOXCOLLIDERPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
