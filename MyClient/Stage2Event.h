#pragma once
class CCameraEvent;

class CGameObject;
class CStage2Event
{
public:
	CStage2Event();
	~CStage2Event();
public:
	void Initialize();
	void Update();

		
	CCameraEvent*  m_CameraEvents;
	CGameObject*  m_pPlayer;
	CGameObject*  m_pBoss;

	bool		  m_bCameraEvent=false;
	bool		  m_bEvent2 = false;


	float		  m_fTime;

};

