#pragma once
class CObserver
{
public:
	CObserver();
	~CObserver();

public:
	virtual void Update(int iMessage) = 0;
};

