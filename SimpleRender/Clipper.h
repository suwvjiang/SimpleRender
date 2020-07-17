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
const int ViewportFarBitCode = 0x10;
const int ViewportNearBitCode = 0x20;

typedef enum Boundary
{
	Left,
	Right,
	Bottom,
	Top,
	Far,
	Near,
};

const int ClipEdgeMax = 6;

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

	if (p < .0)//���ȡ���ֵ
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
		if (p > .0)//����ȡ��Сֵ
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
		else//��߽�ƽ��
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
	if (vert.x > vert.w)
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
//���Ѷ�
//�ο�http://web.cs.wpi.edu/~emmanuel/courses/cs4731/A14/slides/lecture23.pdf
inline bool clipLineByLiangBarskIn3D(Vec4f& v0, Vec4f& v1)
{
	float uIn = 0, uOut = 1, delta;
	float p0[6] = { v0.w + v0.x, v0.w - v0.x, v0.w + v0.y, v0.w - v0.y, v0.w + v0.z, v0.w - v0.z };//left:(x<-w)---->(w+x)<0, right:(w<x)--->(w-x)<0
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

	Vec4f deltaV = v1 - v0;
	if (code1 != ViewportInsideBitCode)
		v1 = v0 + deltaV * uOut;
	if (code0 != ViewportInsideBitCode)
		v0 += deltaV * uIn;
	return true;
}

//�ж��Ƿ��ڱ߽���
inline bool checkVertexInside(const Fragment& vert, Boundary edge)
{
	switch (edge)
	{
	case Left:
		if(vert.pos.w + vert.pos.x < 0)
			return false;
		break;
	case Right:
		if (vert.pos.w - vert.pos.x < 0)
			return false;
		break;
	case Bottom:
		if (vert.pos.w + vert.pos.y < 0)
			return false;
		break;
	case Top:
		if (vert.pos.w - vert.pos.y < 0)
			return false;
		break;
	case Far:
		if (vert.pos.w + vert.pos.z < 0)
			return false;
		break;
	case Near:
		if (vert.pos.w - vert.pos.z < 0)
			return false;
		break;
	}
	return true;
}
//�߶��Ƿ񴩹��߽�
inline bool checkVertexCross(const Fragment& vert0, const Fragment& vert1, Boundary edge)
{
	return checkVertexInside(vert0, edge) ^ checkVertexInside(vert1, edge);
}
//��ȡ�����λ��
inline float getIntersect(const Fragment& vert0, const Fragment& vert1, Boundary edge)
{
	float p0, p1;
	switch (edge)
	{
	case Left:
		p0 = vert0.pos.w + vert0.pos.x;
		p1 = vert1.pos.w + vert1.pos.x;
		break;
	case Right:
		p0 = vert0.pos.w - vert0.pos.x;
		p1 = vert1.pos.w - vert1.pos.x;
		break;
	case Bottom:
		p0 = vert0.pos.w + vert0.pos.y;
		p1 = vert1.pos.w + vert1.pos.y;
		break;
	case Top:
		p0 = vert0.pos.w - vert0.pos.y;
		p1 = vert1.pos.w - vert1.pos.y;
		break;
	case Far:
		p0 = vert0.pos.w + vert0.pos.z;
		p1 = vert1.pos.w + vert1.pos.z;
		break;
	case Near:
		p0 = vert0.pos.w - vert0.pos.z;
		p1 = vert1.pos.w - vert1.pos.z;
		break;
	}
	return p0 / (p0 - p1);
}
//���ý���Ķ�������
inline void setIntersectFragment(const Fragment& vert0, const Fragment& vert1, float t, Fragment& dest)
{
	dest.pos = Lerp(vert0.pos, vert1.pos, t);
	dest.color = Lerp(vert0.color, vert1.color, t);
	dest.uv = Lerp(vert0.uv, vert1.uv, t);
}

//�����߲ü�
//�˴��㷨�����⡣��p��λ��right��top�ı߽罻��ʱ��
//checkVertexCrossʱ�����ж�p����߽��ཻ���ҽ�����ȻΪp��,
//������p�����checkVertexCross���ж�����ӵ�����б�
//���������ڽ�������checkVertexInside�Ҳ�ж�ͨ������һ����ӵ�����б��С�
inline void clipVertex(const Fragment& vert, Boundary edge, std::vector<Fragment>& outFrags, std::vector<Fragment*>& first, std::vector<Fragment>& last)
{
	Fragment frag;
	if (!first[edge])
		first[edge] = new Fragment(vert);
	else
	{
		if (checkVertexCross(vert, last[edge], edge))
		{
			float t = getIntersect(vert, last[edge], edge);
			setIntersectFragment(vert, last[edge], t, frag);

			if (edge < Near)
				clipVertex(frag, Boundary(edge + 1), outFrags, first, last);
			else
				outFrags.emplace_back(frag);
		}
	}

	last[edge] = vert;

	if (checkVertexInside(vert, edge))
	{
		if (edge < Near)
			clipVertex(vert, Boundary(edge + 1), outFrags, first, last);
		else
			outFrags.emplace_back(vert);
	}
}
//�����չ�
inline void closeClip(std::vector<Fragment>& outFrags, std::vector<Fragment*>& first, std::vector<Fragment>& last)
{
	Fragment frag;
	for (int edge = Left; edge <= Near; ++edge)
	{
		if (checkVertexCross(last[edge], *first[edge], Boundary(edge)))
		{
			float t = getIntersect(last[edge], *first[edge], Boundary(edge));
			setIntersectFragment(last[edge], *first[edge], t, frag);

			if (edge < Near)
				clipVertex(frag, Boundary(edge + 1), outFrags, first, last);
			else
				outFrags.emplace_back(frag);
		}
	}
}
//�ݹ�֮���������ظ���ӽ���֮��
inline void clipTriangleBySuthHodgIn3D(const Triangle& triangle, std::vector<Triangle>& destTriangles)
{
	std::vector<Fragment> outFrags;
	std::vector<Fragment*> first(ClipEdgeMax, 0);
	std::vector<Fragment> lastFrags(ClipEdgeMax);

	int k = 0;
	for (k = 0; k < 3; ++k)
	{
		clipVertex(triangle.vertex[k], Left, outFrags, first, lastFrags);
	}

	closeClip(outFrags, first, lastFrags);

	if (outFrags.size() == 0)
		return;

	//�ع�������
	size_t triNum = outFrags.size() - 2;
	for (size_t i = 0; i < triNum; ++i)
	{
		size_t j = (i + 1) % outFrags.size();
		size_t k = (i + 2) % outFrags.size();

		Triangle reTri;
		reTri.vertex[0] = outFrags[0];
		reTri.vertex[1] = outFrags[j];
		reTri.vertex[2] = outFrags[k];

		destTriangles.emplace_back(reTri);
	}
}
//ѭ��֮�������Ա��⾭���߽罻����߶α��ظ����
inline void clipTriangleByClassicIn3D(const Triangle& triangle, std::vector<Triangle>& destTriangles)
{
	size_t num;
	bool code0, code1;

	std::vector<Fragment> outFrags(triangle.vertex, triangle.vertex + 3);
	std::vector<Fragment> inFrags;
	for (int edge = Left; edge <= Near; ++edge)
	{
		inFrags = outFrags;
		outFrags.clear();

		num = inFrags.size();
		for (size_t i = 0; i < num; ++i)
		{
			size_t k = (i + 1) % num;

			code0 = checkVertexInside(inFrags[i], Boundary(edge));
			code1 = checkVertexInside(inFrags[k], Boundary(edge));

			if (code0 && code1)
				outFrags.emplace_back(inFrags[k]);
			else if (!code0 && code1)
			{
				float t = getIntersect(inFrags[i], inFrags[k], Boundary(edge));
				Fragment newFrag;
				setIntersectFragment(inFrags[i], inFrags[k], t, newFrag);
				outFrags.emplace_back(newFrag);

				outFrags.emplace_back(inFrags[k]);
			}
			else if (code0 && !code1)
			{
				float t = getIntersect(inFrags[i], inFrags[k], Boundary(edge));
				Fragment newFrag;
				setIntersectFragment(inFrags[i], inFrags[k], t, newFrag);
				outFrags.emplace_back(newFrag);
			}
		}
	}

	if (outFrags.size() == 0)
		return;

	//�ع�������
	size_t triNum = outFrags.size() - 2;
	for (size_t i = 0; i < triNum; ++i)
	{
		size_t j = (i + 1) % outFrags.size();
		size_t k = (i + 2) % outFrags.size();

		Triangle reTri;
		reTri.vertex[0] = outFrags[0];
		reTri.vertex[1] = outFrags[j];
		reTri.vertex[2] = outFrags[k];

		destTriangles.emplace_back(reTri);
	}
}

inline void CVVClip(Triangle** tris, size_t& cnt)
{
	std::vector<Triangle> clippedTris;

	int code0, code1, code2, codeAnd, codeOr;
	Triangle currTri;
	for (size_t i = 0; i < cnt; ++i)
	{
		currTri = (*tris)[i];
		
		code0 = encode(currTri.vertex[0].pos);
		code1 = encode(currTri.vertex[1].pos);
		code2 = encode(currTri.vertex[2].pos);

		codeAnd = code0 & code1 & code2;
		codeOr = code0 | code1 | code2;

		//���������⣬����������
		if (codeAnd != ViewportInsideBitCode)
			continue;

		if (codeOr == ViewportInsideBitCode)
		{
			clippedTris.emplace_back(currTri);
			continue;
		}

		//clipTriangleBySuthHodgIn3D(currTri, clippedTris);
		clipTriangleByClassicIn3D(currTri, clippedTris);
	}

	size_t newCnt = clippedTris.size();
	if (newCnt > cnt)
	{
		delete[](*tris);
		(*tris) = new Triangle[newCnt];
	}
	cnt = newCnt;
	std::memcpy((*tris), clippedTris.data(), cnt * sizeof(Triangle));
}
#pragma endregion