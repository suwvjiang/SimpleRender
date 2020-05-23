#pragma once
#include "MathUtils.h"
/************************************************************************/
/* (x1+(x2-x1)u)/(w1+(w2-w1)u) = 1 ------> u = (x1-h1)/((x1-h1)-(x2-h2))*/
/* (x1+(x2-x1)u)/(w1+(w2-w1)u) = -1 -----> u = (x1+h1)/((x1+h1)-(x2+h2))*/
/************************************************************************/

const int ViewportInsideBitCode = 0x0;
const int ViewportLeftBitCode = 0x1;
const int ViewportRightBitCode = 0x2;
const int ViewportBottomBitCode = 0x4;
const int ViewportTopBitCode = 0x8;
const int ViewportNearBitCode = 0x10;
const int ViewportFarBitCode = 0x20;

inline bool inside(int code) {
	return code == ViewportInsideBitCode;
}
inline bool reject(int code0, int code1)
{
	return (code0 & code1) != ViewportInsideBitCode;
}
inline bool accept(int code0, int code1)
{
	return (code0 | code1) == ViewportInsideBitCode;
}

#pragma region 2D Clip
inline int encode(const Vec2i& pt, const Vec2i& winMin, const Vec2i& winMax)
{
	int code = 0x00;
	if (pt.x < winMin.x)
		code = code | ViewportLeftBitCode;
	if (pt.x > winMax.x)
		code |= ViewportRightBitCode;
	if (pt.y < winMin.y)
		code |= ViewportBottomBitCode;
	if (pt.y > winMax.y)
		code |= ViewportTopBitCode;
	return code;
}

inline int clipTest(float p, float q, float* uIn, float* uOut)
{
	float r;
	int returnValue = true;

	if (p < .0)//入边取最大值
	{
		r = q / p;
		if (r > * uOut)
			returnValue = false;
		else
		{
			if (r > * uIn)
				*uIn = r;
		}
	}
	else
	{
		if (p > .0)//出边取最小值
		{
			r = q / p;
			if (r < *uIn)
				returnValue = false;
			else
			{
				if (r < *uOut)
					*uOut = r;
			}
		}
		else//与边界平行
			if (q < .0)
				returnValue = false;
	}
	return returnValue;
}
#pragma endregion

#pragma region 3D Clip
inline int encode(const Vec4f& vert)
{
	int code = 0x0;
	if (vert.x < -vert.w)
		code |= ViewportLeftBitCode;
	if (vert.w > vert.w)
		code |= ViewportRightBitCode;
	if (vert.y < -vert.w)
		code |= ViewportBottomBitCode;
	if (vert.y > vert.w)
		code |= ViewportTopBitCode;
	if (vert.z < -vert.w)
		code |= ViewportFarBitCode;
	if (vert.z > vert.w)
		code |= ViewportNearBitCode;
	return code;
}

inline bool clipLineByCohSuthIn3D(Vec4f& v0, Vec4f& v1)
{
	int code0, code1;
	float delta = 1;
	bool done = false;

	while (!done)
	{
		code0 = encode(v0);
		code1 = encode(v1);
		if (reject(code0, code1))
			return false;

		if (accept(code0, code1))
			return true;

		if (inside(code0))
		{
			Swap(v0, v1);
			std::swap(code0, code1);
		}

		if (code0 & ViewportLeftBitCode)
		{
			delta = (v0.x + v0.w) / (v0.x - v1.x + v0.w - v1.w);
		}
		else
		{
			if (code0 & ViewportRightBitCode)
			{
				delta = (v0.x - v0.w) / (v0.x - v1.x - v0.w + v1.w);
			}
			else
			{
				if (code0 & ViewportBottomBitCode)
				{
					delta = (v0.y + v0.w) / (v0.y - v1.y + v0.w - v1.w);
				}
				else
				{
					if (code0 & ViewportTopBitCode)
					{
						delta = (v0.y - v0.w) / (v0.y - v1.y - v0.w + v1.w);
					}
					else
					{
						if (code0 & ViewportFarBitCode)
						{
							delta = (v0.z + v0.w) / (v0.z - v1.z + v0.w - v1.w);
						}
						else
						{
							if (code0 & ViewportNearBitCode)
							{
								delta = (v0.z - v0.w) / (v0.z - v1.z - v0.w + v1.w);
							}
						}
					}
				}
			}
		}

		v0 += (v1 - v0) * delta;
	}
}
//梁友栋
inline bool clipLineByLiangBarskIn3D(Vec4f& v0, Vec4f& v1)
{
	float uIn = 0, uOut = 1, delta;
	float p0[6] = { v0.w + v0.x, v0.w - v0.x, v0.w + v0.y, v0.w - v0.y, v0.w + v0.z, v0.w - v0.z };
	float p1[6] = { v1.w + v1.x, v1.w - v1.x, v1.w + v1.y, v1.w - v1.y, v1.w + v1.z, v1.w - v1.z };

	int code0 = encode(v0);
	int code1 = encode(v1);

	if (accept(code0, code1))
		return true;
	if (reject(code0, code1))
		return false;

	for (int i = 0; i < 6; ++i)
	{
		if (p1[i] < 0)
		{
			delta = p0[i] / (p0[i] - p1[i]);
			uOut = min(delta, uOut);
		}
		else if (p0[i] < 0)
		{
			delta = p0[i] / (p0[i] - p1[i]);
			uIn = max(delta, uIn);
		}

		if (uIn > uOut)
			return false;
	}

	if (code1 != ViewportInsideBitCode)
		v1 = v0 + (v1 - v0) * uOut;
	if (code0 != ViewportInsideBitCode)
		v0 += (v1 - v0) * uIn;
	return true;
}

#pragma endregion