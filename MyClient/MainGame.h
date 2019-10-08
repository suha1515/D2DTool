#pragma once
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Update();
	void LateUpdate();
	void Render();
	void RenderLine();

private:
	HRESULT Initialize();
	void Release();

public:
	static CMainGame* Create();

private:
	CDeviceMgr*			m_pDeviceMgr;
	CTextureMgr*		m_pTextureMgr;
	CTimeMgr*			m_pTimeMgr;
	CFrameMgr*			m_pFrameMgr;
	CObjectMgr*			m_pObjectMgr;
	CAnimationMgr*		m_pAnimationMgr;
	CCameraMgr*			m_pCameraMgr;
	CScriptMgr*			m_pScriptMgr;
	CInstanceMgr*		m_pInstanceMgr;
	CCollisionMgr*		m_pCollisionMgr;
	CSceneMgr*			m_pSceneMgr;
	CShaderMgr*			m_pShaderMgr;
	CKeyMgr*			m_pKeyMgr;
};

