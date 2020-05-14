#pragma once
#include "MathUtils.h"

//边界权重
struct EdgeEquation
{
	int i, j, k, value;
	EdgeEquation() {};
	EdgeEquation(const Vec2i& p0, const Vec2i& p1, const Vec2i& p)
	{
		i = p0.y - p1.y;
		j = p1.x - p0.x;
		k = Cross(p1, p0);
		value = p.x * i + p.y * j - k;
	}

	void incrementX(const int& step = 1)
	{
		value += step * i;
	}

	void incrementY(const int& step = 1)
	{
		value += step * j;
	}
};
//重心坐标系
struct EdgeEquationSet
{
	EdgeEquation e0, e1, e2;
	EdgeEquationSet() {};
	EdgeEquationSet(const Vec2i& v0, const Vec2i& v1, const Vec2i& v2, const Vec2i& p)
	{
		e0 = EdgeEquation(v1, v2, p);
		e1 = EdgeEquation(v2, v0, p);
		e2 = EdgeEquation(v0, v1, p);
	}

	void incrementX(const int& step = 1)
	{
		e0.incrementX(step);
		e1.incrementX(step);
		e2.incrementX(step);
	}

	void incrementY(const int& step = 1)
	{
		e0.incrementY(step);
		e1.incrementY(step);
		e2.incrementY(step);
	}

	bool evaluate()
	{
		return e0.value >= 0 && e1.value >= 0 && e2.value >= 0;
	}
};
//顶点数据
struct Vertex
{
	Vec3f m_pos;
	Vec3f m_normal;
	Vec2f m_uv;

	Vertex() {};
	Vertex(const Vec3f& pos, const Vec3f& normal, const Vec2f uv)
		:m_pos(pos),m_normal(normal),m_uv(uv)
	{
	}
};
//片段数据
struct Fragment
{
	Vec4f m_pos;
	Vec4f m_normal;
	Vec4f m_uv;
};
//三角形
struct Triangle
{
	Fragment vertex[3];
};