#pragma once
class CPuzzleScripts;
class CSTage1Event
{
public:
	CSTage1Event();
	~CSTage1Event();
public:
	void Initialize();
	void Update();

private:
	map<string, vector<CPuzzleScripts*>>	m_mapPuzzle;
	map<string, map<string,CGameObject*>>	m_PuzzlesObject;

	bool									m_Puzzle1=false;
};

