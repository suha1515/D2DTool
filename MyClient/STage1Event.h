#pragma once
class CPuzzleScripts;
class CCameraEvent;
class CSTage1Event
{
	enum STATE{NONE,PUZZLE1,PUZZLE2,CAMERA1};
public:
	CSTage1Event();
	~CSTage1Event();
public:
	void Initialize();
	void Update();

	void CameraEventUpdate();
public:
	void EventState();

private:
	map<string, CCameraEvent*>					m_CameraEvents;
	map<string, map<string, CPuzzleScripts*>>	m_mapPuzzle;
	map<string, map<string,CGameObject*>>	m_PuzzlesObject;

	//���� 1�����Ѻ�����
	bool									m_Puzzle1=false;
	bool									m_Puzzle1ObjFade = false;

	float									m_fAlphaValue;

	STATE									m_CurState;
	STATE									m_PreState;

	//����2�����Ѻ�����..

	bool									m_Puzzle2PointOn = false;
	bool									m_Puzzle2SpawnOn = false;
	bool									m_Puzzle2Clear = false;
	bool									m_Puzzle2WallFade = false;

	float									m_fAlpahValue2;

	//����3�� ���� ������..
	bool									m_Puzzle3Clear = false;
	bool									m_Puzzle3WallFde = false;
	float									m_fAlphaValue3;

	//����4������ ������..
	bool									m_Puzzle4Clear = false;
};

