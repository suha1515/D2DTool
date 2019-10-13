#pragma once

inline void ParabolaMoveMent(const D3DXVECTOR3* startPos, const D3DXVECTOR3* endPos, float* endTime,
									const float& maxTime, const float& maxHeight, float* fgravity,
									float* speedX,float* speedY)
{
	//끝나는 높이
	float endHeight = endPos->y - startPos->y;

	//포물선의 높이
	float fHeight = maxHeight - startPos->y;

	//중력가속도
	float fg = 2 * fabsf(maxHeight / (maxTime*maxTime));

	*speedY = sqrtf(2 * fg*fHeight);

	float a = fg;
	float b = -2 * (*speedY);
	float c = 2 * endHeight;

	*endTime = (-b + sqrtf((b*b) - (4 * a*c))) / (2 * a);

	*speedX = -(startPos->x - endPos->x) / (*endTime);
	*fgravity = fg;
}

inline D3DXVECTOR3 Lerp(const D3DXVECTOR3& begin, const D3DXVECTOR3& end,const float& _ratio)
{
	float ratio = __min(1.f, __max(0.f, _ratio));
	D3DXVECTOR3 temp;

	temp = begin + (end - begin)*ratio;
	return temp;
}

inline D3DXVECTOR3 BezierCurve(const D3DXVECTOR3& begin,const D3DXVECTOR3& end,const D3DXVECTOR3& control,const float& _ratio)
{
	D3DXVECTOR3 mid1 = Lerp(begin, control, _ratio);
	D3DXVECTOR3 mid2 = Lerp(control, end, _ratio);
	D3DXVECTOR3 pos = Lerp(mid1, mid2, _ratio);
	return pos;
}