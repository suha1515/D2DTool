#pragma once
class CScene
{
protected:
	CScene();

public:
	virtual ~CScene();

public:
	virtual void Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render() PURE;

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;

protected:
	CDeviceMgr*			m_pDeviceMgr;
	CTextureMgr*		m_pTextureMgr;
	CTimeMgr*			m_pTimeMgr;
	CObjectMgr*			m_pObjectMgr;
	CSceneMgr*			m_pSceneMgr;
	CKeyMgr*			m_pKeyMgr;
};

