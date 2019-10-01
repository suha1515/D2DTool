#pragma once
#include "afxwin.h"


// CAnimationFrame ��ȭ �����Դϴ�.
class CGameObject;
class CAnimationFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimationFrame)

public:
	CAnimationFrame(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimationFrame();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMFRAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// //�ִϸ��̼� Ŭ�� ����Ʈ
	CListBox m_ClipList;
	// �ִϸ��̼� ��ġ
	float m_PositionX;
	float m_PositionY;
	float m_PositionZ;

	// //�ִϸ��̼� ȸ��
	float m_RotationX;
	float m_RotationY;
	float m_RotationZ;
	// //�ִϸ��̼� ũ��
	float m_ScaleX;
	float m_ScaleY;
	float m_ScaleZ;
	// //�ִϸ��̼� ��� �ð�
	float m_PlayTime;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);


public:
	void				Update();
private:
	CGameObject*		m_ClickedObject;

};
