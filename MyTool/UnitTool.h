#pragma once
#include "afxwin.h"


// CUnitTool 대화 상자입니다.

class CUnitTool : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 유닛 툴 검색용
	CString m_Search;
	// 하이라키에 내놓을 이름
	CString m_name;
	afx_msg void OnBnClickedAddObject();

	OBJ_COPY MakeCopy(CGameObject* pObject);
	void	 WriteCopy(CFile* pFile,OBJ_COPY* copy);

public:
	//추가한 유닛을 넣을 컨테이너.
	map<wstring,OBJ_COPY>		m_Units;
	// //담을 오브젝트
	CListBox m_ObjectList;
	afx_msg void OnBnClickedSaveList();
	afx_msg void OnBnClickedLoadObject();
	// 카피된 오브젝트 이름
	CString m_CopyName;
	afx_msg void OnBnClickedToHierarchy();
	afx_msg void OnBnClickedRemove();
};
