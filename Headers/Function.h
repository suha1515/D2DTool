#pragma once

#ifndef __FUNCTION_H__

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDelete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

namespace strings {
	static std::wstring widen(const std::string& s)
	{
		std::vector<wchar_t> buf(MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, 0, 0));
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, &buf[0], buf.size());
		return std::wstring(&buf[0]);
	}
}

inline float GetAngle(const D3DXVECTOR3& pStart, const D3DXVECTOR3& pEnd)
{
	float dx = pEnd.x - pStart.x;
	float dy = pEnd.y - pStart.y;

	return D3DXToDegree(atan2f(dy, dx));
}

//라인과 라인충돌 함수 마지막 두인자는 라인과 라인의 교차점
inline bool LineDection(const LINE& _L1, const LINE& _L2, float* x=nullptr, float* y=nullptr)
{
	float x1 = _L1.startPoint.x;
	float y1 = _L1.startPoint.y;

	float x2 = _L1.endPoint.x;
	float y2 = _L1.endPoint.y;

	float x3 = _L2.startPoint.x;
	float y3 = _L2.startPoint.y;

	float x4 = _L2.endPoint.x;
	float y4 = _L2.endPoint.y;


	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		float intersectionX = x1 + (uA *(x2 - x1));
		float intersectionY = y1 + (uA * (y2 - y1));
		if(x!=nullptr)
		*x = intersectionX;
		if(y!=nullptr)
		*y = intersectionY;
		return true;
	}
	return false;
}

inline bool LineRect(const LINE& _L1, const D3DXVECTOR3* pSquare,COLLIDE_TYPE coltype ,float* interX=nullptr,float* interY=nullptr)
{
	LINE line1 = _L1;
	LINE line2;
	bool left;
	bool right;
	bool top;
	bool bottom;

	bool leftDiagnol;
	bool rightDiagnol;
	if (coltype == NORMAL)
	{
		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[1];
		bool left = LineDection(line1, line2, interX, interY);

		line2.startPoint = pSquare[2];
		line2.endPoint = pSquare[3];
		bool right = LineDection(line1, line2, interX, interY);

		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[2];
		bool top = LineDection(line1, line2, interX, interY);

		line2.startPoint = pSquare[1];
		line2.endPoint = pSquare[3];
		bool bottom = LineDection(line1, line2, interX, interY);

	}
	else if (coltype == LEFT_TOP)
	{
		//좌측선
		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[1];
		bool left = LineDection(line1, line2, interX, interY);

		//상단선
		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[2];
		bool right = LineDection(line1, line2, interX, interY);

		//좌측상단 대각선
		line2.startPoint = pSquare[2];
		line2.endPoint = pSquare[1];
		bool leftDiagnol = LineDection(line1, line2, interX, interY);

	}
	//좌측하단 대각선
	else if (coltype == LEFT_BOTTOM)
	{
		//좌측선
		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[1];
		bool left = LineDection(line1, line2, interX, interY);

		//상단선
		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[2];
		bool right = LineDection(line1, line2, interX, interY);

		//좌측상단 대각선
		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[3];
		bool leftDiagnol = LineDection(line1, line2, interX, interY);
	}
	else if (coltype == RIGHT_TOP)
	{
		//우측선
		line2.startPoint = pSquare[2];
		line2.endPoint = pSquare[3];
		bool left = LineDection(line1, line2, interX, interY);

		//상단선
		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[2];
		bool right = LineDection(line1, line2, interX, interY);

		//우측상단 대각선
		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[3];
		bool rightDiagnol = LineDection(line1, line2, interX, interY);
	}
	else if (coltype == RIGHT_BOTTOM)
	{
		//우측선
		line2.startPoint = pSquare[2];
		line2.endPoint = pSquare[3];
		bool left = LineDection(line1, line2, interX, interY);

		//하단선
		line2.startPoint = pSquare[0];
		line2.endPoint = pSquare[3];
		bool right = LineDection(line1, line2, interX, interY);

		//좌측상단 대각선
		line2.startPoint = pSquare[1];
		line2.endPoint = pSquare[2];
		bool rightDiagnol = LineDection(line1, line2, interX, interY);
	}
	
	if (left || right || top || bottom||leftDiagnol||rightDiagnol)
		return true;
	
	return false;
}

inline D3DXVECTOR3 GetReflectVector(const D3DXVECTOR3* srcDir, const D3DXVECTOR3* normal)
{
	D3DXVECTOR3 reflect = (*srcDir) - (2*D3DXVec3Dot(srcDir, normal))*(*normal);
	return reflect;
}
#define __FUNCTION_H__
#endif