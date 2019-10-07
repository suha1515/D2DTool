#pragma once

#include"HierarchyView.h"
#include "afxwin.h"

#include "ComponentSheet.h"

// CInspectView 폼 뷰입니다.

class CGameObject;
class CInspectView : public CFormView
{
	DECLARE_DYNCREATE(CInspectView)

protected:
	CInspectView();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHierarchy();

public:
	//클릭한 오브젝트 지정.
	void SetObject(CGameObject* object);
public:
	//에딧 박스들 정보 갱신 오브젝트 통해서.
	void UpdateInfo();
	//오브젝트 정보 에딧박스에 있는 내용으로 갱신
	void UpdateObject();

public:
	CHierarchyView		m_HierarchyView;
	virtual void OnInitialUpdate();
	// 인스펙터 오브젝트 이름
	CEdit m_ObjectName;
	// 인스펙터 포지션 x,y,z 값
	float m_PosX, m_PosY, m_PosZ;
	// 인스펙터 로테이션 x,y,z값
	float m_RotX, m_RotY, m_RotZ;
	// 인스펙터 스케일 x,y,z 값
	float m_ScaleX, m_ScaleY, m_ScaleZ;

private:
	CGameObject*		m_ClickedObject;

	//컴포넌트 프로퍼티 시트
	CComponentSheet*	m_pComponentSheet;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// 오브젝트 태그
	CString m_ObjectTag;
	// 오브젝트 레이어
	CComboBox m_ObjectLayer;
	// //디버그 모드
	CButton m_Debug;
	afx_msg void OnBnClickedCheck1();
};


