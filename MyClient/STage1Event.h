#pragma once
class CPuzzleScripts;
class CSTage1Event
{
	enum STATE{NONE,PUZZLE1,PUZZLE2,CAMERA1};
public:
	CSTage1Event();
	~CSTage1Event();
public:
	void Initialize();
	void Update();

public:
	void EventState();

private:
	map<string, vector<CPuzzleScripts*>>	m_mapPuzzle;
	map<string, map<string,CGameObject*>>	m_PuzzlesObject;

	//퍼즐 1에관한변수들
	bool									m_Puzzle1=false;
	bool									m_Puzzle1ObjFade = false;

	float									m_fAlphaValue;

	STATE									m_CurState;
	STATE									m_PreState;
};

