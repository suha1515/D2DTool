#pragma once
#include "afxwin.h"


// CBoxColliderPage 대화 상자입니다.
class CGameObject;
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
public:
	// 컴포넌트 비/활성화 버튼
	CButton m_CompSwitch;
	// 컴포넌트 추가 버튼
	CButton m_CompAddBtn;
	// 컴포넌트 제거 버튼
	CButton m_CompRemoveBtn;
	// 박스크기 width 컨트롤
	CEdit m_EditWidth;
	// 박스크기 Height 컨트롤
	CEdit m_Editheight;
	// 오프셋 X 에딧컨트롤
	CEdit m_EditPosX;
	// 오프셋  Y 에딧컨트롤
	CEdit m_EditPosY;
	// 콜라이더 지정 버튼
	CButton m_SetColliderBtn;
	afx_msg void OnBnClickedAddComp();
	afx_msg void OnBnClickedRemoveComp();
	afx_msg void OnBnClickedSetCollide();
	afx_msg void OnBnClickedCompCheck();

public:
	void SetObject(CGameObject* pObject);
	void Update();

private:
	CGameObject*		m_pObject;
public:
	// 박스 가로
	float m_width;
	// 박스 세로
	float m_height;
	// 박스 위치X
	float m_PosX;
	// 박스 위치Y
	float m_PosY;
};
