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

inline float Lerp(const float& begin, const float& end, const float& _ratio)
{
	float ratio = __min(1.f, __max(0.f, _ratio));
	float temp;

	temp = begin + (end - begin)*ratio;
	return temp;
}

inline float LerpFloat(const float& begin, const float& end, const float& _ratio)
{
	float ratio = __min(1.f, __max(0.f, _ratio));
	float temp;

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

inline D3DXVECTOR3 BezierCuve4Dim(const D3DXVECTOR3& begin, const D3DXVECTOR3& end, const D3DXVECTOR3& control1, const D3DXVECTOR3& control2, const D3DXVECTOR3& control3, const float& _ratio)
{
	D3DXVECTOR3 Dim1mid1 = Lerp(begin, control1, _ratio);
	D3DXVECTOR3 Dim1mid2 = Lerp(control1,control2, _ratio);
	D3DXVECTOR3 Dim1mid3 = Lerp(control2, control3, _ratio);
	D3DXVECTOR3 Dim1mid4 = Lerp(control3, end, _ratio);

	D3DXVECTOR3 Dim2mid1 = Lerp(Dim1mid1, Dim1mid2, _ratio);
	D3DXVECTOR3 Dim2mid2 = Lerp(Dim1mid2, Dim1mid3, _ratio);
	D3DXVECTOR3 Dim2mid3 = Lerp(Dim1mid3, Dim1mid4, _ratio);

	D3DXVECTOR3 Dim3mid1 = Lerp(Dim2mid1, Dim2mid2, _ratio);
	D3DXVECTOR3 Dim3mid2 = Lerp(Dim2mid2, Dim2mid3, _ratio);
	
	D3DXVECTOR3 pos = Lerp(Dim3mid1, Dim3mid2, _ratio);
	
	return pos;
}