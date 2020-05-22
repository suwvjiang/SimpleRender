#pragma once
#include "MathUtils.h"

const int ViewportInsideBitCode = 0x0;
const int ViewportLeftBitCode = 0x1;
const int ViewportRightBitCode = 0x2;
const int ViewportBottomBitCode = 0x4;
const int ViewportTopBitCode = 0x8;
const int ViewportNearBitCode = 0x16;
const int ViewportFarBitCode = 0x32;

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

inline int encode(const Vec4f& vert)
{
	int code = 0x00;
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