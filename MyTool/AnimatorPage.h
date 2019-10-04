#pragma once
#include "afxwin.h"


// CAnimatorPage 대화 상자입니다.
class CGameObject;
class CAnimatorPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAnimatorPage)

public:
	CAnimatorPage();
	virtual ~CAnimatorPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATORPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 컴포넌트 비/활성화
	CButton m_CompSwitch;
	// 애니메이션 리스트
	CComboBox m_AnimCombo;
	// 클립 콤보
	CComboBox m_ClipCombo;

public:
	void SetObject(CGameObject* pObject);
	void Update();
	// 컴포넌트 추가버튼
	CButton m_CompAdd;
	// 컴포넌트 제거 버튼
	CButton m_CompRemove;
	// 재생 버튼
	CButton m_PlayBtn;
	// 정지 버튼
	CButton m_PauseBtn;
	// 초기화 버튼
	CButton m_InitBtn;
	// //애니메이션 리스트 갱신
	CButton m_animListRenew;
private:
	CGameObject *m_pObject;
public:
	afx_msg void OnBnClickedCompOn();
	afx_msg void OnBnClickedRenewList();
	
	afx_msg void OnBnClickedAddComp();
	afx_msg void OnBnClickedRemoveComp();
	afx_msg void OnBnClickedRenewClip();
	afx_msg void OnBnClickedSetAnim();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedReWind();
	// 반복 체크박스.
	CButton m_LoopCheck;
};
