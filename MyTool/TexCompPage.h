#pragma once
#include "afxwin.h"


// CTexCompPage 대화 상자입니다.
class CGameObject;
class CTexCompPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTexCompPage)

public:
	CTexCompPage();
	virtual ~CTexCompPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXCOMPPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


public:
	void SetObject(CGameObject* pObject);
	void Update();

private:
	CGameObject*		m_pObject;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnBnClickedChangeTex();

	// 텍스쳐 사이즈에 맞춘 정점 X 크기.
	int m_sizeX;
	int m_sizeY;
	//텍스처 바꾸기 버튼
	CButton m_ChangeBtn;

	// 텍스처 크기 지정 컨트롤
	CEdit m_sizeXEdit;
	CEdit m_sizeYEdit;
	// 컴포넌트 비/활성화
	CButton m_CompCheckBox;
	afx_msg void OnBnClickedCheckcomponent();

	CButton m_ComponentAdd;
	CButton m_ComponentRemove;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddComp();
	afx_msg void OnBnClickedRemoveComp();
};
