#pragma once
#include <functional>
#include "MathUtils.h"

#define PIX_BITS 24

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

	void incrementX(const int i = 1)
	{
		e0.value += e0.i * i;
		e1.value += e1.i * i;
		e2.value += e2.i * i;
	}

	void incrementY(const int j = 1)
	{
		e0.value += e0.j * j;
		e1.value += e1.j * j;
		e2.value += e2.j * j;
	}

	bool evaluate()
	{
		return e0.value <= 0 && e1.value <= 0 && e2.value <= 0;
	}
};
//顶点数据
struct Vertex
{
	Vec3f pos;
	Vec2f uv;
	Vec3f normal;
	Vec3f color;

	Vertex() {};
	Vertex(const Vec3f& _pos, const Vec3f& _normal, const Vec2f& _uv, const Vec3f& _color)
		:pos(_pos), uv(_uv), normal(_normal), color(_color)
	{
	}

	Vertex(const float& x, const float& y, const float& z,
		const float& r, const float& g, const float& b,
		const float& normalX, const float& normalY, const float& normalZ,
		const float& u, const float& v)
		:pos(x, y, z), color(r, g, b), uv(u, v), normal(normalX, normalY, normalZ)
	{
	}
};
//片段数据
struct Fragment
{
	Fragment() {};
	Fragment(const Fragment& _frag) :pos(_frag.pos),uv(_frag.uv), normal(_frag.normal), color(_frag.color) {};
	Fragment(const Vec3f& _col) :color(_col) {};

	Vec4f pos;
	Vec2f uv;
	Vec3f normal;
	Vec3f color;
};
//三角形
struct Triangle
{
	Fragment vertex[3];
};
//渲染窗口
struct Viewport
{
	int originX;
	int originY;
	int width;
	int height;

	int getLTX() { return originX - (width >> 1); }
	int getLTY() { return originY - (height >> 1); }
	int getRBX() { return originX + (width >> 1); }
	int getRBY() { return originY + (height >> 1); }
};
//着色器
struct ShaderStruct
{
	struct ConstBuffer
	{
		Matrix4x4f world;
		Matrix4x4f view_proj;
		Matrix4x4f m_v_p;
		Matrix4x4f obj2world;
		Vec3f lightDir;
	};

	static ConstBuffer constBuffer;

	inline static void VS(const Vertex& input, Fragment& output)
	{
		Vec4f pos(input.pos.x, input.pos.y, input.pos.z, 1);
		output.pos = Transform(constBuffer.m_v_p, pos);

		Matrix3x3f _objToWorld = Matrix3x3f(constBuffer.obj2world);
		output.normal = Transform(_objToWorld, input.normal);

		output.color = input.color;
		output.uv = input.uv;
	}

	inline static void FS(const Fragment& input, Vec3f* output)
	{
		(*output) = input.normal;
	}

	inline static void TextFS(const Fragment& input, Vec3f* output)
	{
		int u = input.uv.x * 255;
		int v = input.uv.y * 255;
		u /= 32;
		v /= 32;
		(*output) = ((u + v) & 1) ? Color_White : Color_Blue;
	}

	inline static void DepthFS(const Fragment& input, Vec3f* output)
	{
		float depth = -0.5f * input.pos.z + 0.5f;//0-1
		(*output) = Vec3f(depth);
	}

	inline static void LightFS(const Fragment& input, Vec3f* output)
	{
		Vec3f n = Normalize(input.normal);
		Vec3f l = Normalize(constBuffer.lightDir);

		float nl = Dot(n, l);
		nl = 0.5f * nl + 0.5f;

		(*output) = Vec3f(nl);
	}

};

using VertexShader = std::function<void(const Vertex& pVertexIn, Fragment& pVertexOut)>;
using FragmentShader = std::function<void(const Fragment& pFragmentIn, Vec3f *pFragmentOut)>;
using RasterizerInterpolationFunc = std::function<void(const Fragment & frag0, const Fragment & frag1, const Fragment & frag2,
	const float& t0, const float& t1, const float& t2, Fragment & destFrag)>;

//片段着色插值函数
inline void BaseInterpolationFunc(const Fragment& frag0, const Fragment& frag1, const Fragment& frag2,
	const float& t0, const float& t1, const float& t2, Fragment& destFrag)
{
	destFrag.pos = frag0.pos * t0;
	destFrag.pos += frag1.pos * t1;
	destFrag.pos += frag2.pos * t2;

	destFrag.normal = frag0.normal * t0;
	destFrag.normal += frag1.normal * t1;
	destFrag.normal += frag2.normal * t2;

	destFrag.uv = frag0.uv * t0;
	destFrag.uv += frag1.uv * t1;
	destFrag.uv += frag2.uv * t2;

	destFrag.color = frag0.color * t0;
	destFrag.color += frag1.color * t1;
	destFrag.color += frag2.color * t2;
}

static constexpr int Block_Size = 16;

class Rasterizer
{
public:
	Rasterizer() 
	{ 
		m_intFunc = BaseInterpolationFunc; 
	};
	~Rasterizer() {};

	void setViewport(const Viewport& viewPort)
	{
		m_viewPort = viewPort;
	}

	void rasterize(Triangle& triangle, std::vector<Fragment>& frag, std::vector<Vec2i>& pixels, float* depthBuffer)
	{
		float inv_camera_z[3];
		inv_camera_z[0] = 1 / triangle.vertex[0].pos.w;
		inv_camera_z[1] = 1 / triangle.vertex[1].pos.w;
		inv_camera_z[2] = 1 / triangle.vertex[2].pos.w;

		//齐次转换
		triangle.vertex[0].pos *= inv_camera_z[0];
		triangle.vertex[1].pos *= inv_camera_z[1];
		triangle.vertex[2].pos *= inv_camera_z[2];

		Vec2i p0, p1, p2, p;
		p0 = NDCToScreen(triangle.vertex[0].pos, m_viewPort);
		p1 = NDCToScreen(triangle.vertex[1].pos, m_viewPort);
		p2 = NDCToScreen(triangle.vertex[2].pos, m_viewPort);

		int minX, minY, maxX, maxY;
		minX = max(min(min(p0.x, p1.x), p2.x), m_viewPort.getLTX());
		minY = max(min(min(p0.y, p1.y), p2.y), m_viewPort.getLTY());
		maxX = min(max(max(p0.x, p1.x), p2.x), m_viewPort.getRBX());
		maxY = min(max(max(p0.y, p1.y), p2.y), m_viewPort.getRBY());
		if ((minX > maxX) || (minY > maxY))
			return;

		EdgeEquation area(p0, p1, p2);
		if (area.value > 0)
			return;

		auto noBlockSize = ~(Block_Size - 1);
		minX &= noBlockSize;
		minY &= noBlockSize;
		maxX &= noBlockSize;
		maxY &= noBlockSize;

		maxX += Block_Size;
		maxY += Block_Size;

		p.x = minX;
		p.y = minY;
		EdgeEquationSet tempY(p0, p1, p2, p);
		EdgeEquationSet tempX;

		// Tile Render
		int i, j;
		float z0, z1, z2;
		float param0, param1, param2, cameraZ, depth;
		EdgeEquationSet temp, lt, rt, lb, rb;
		bool ltIn, rtIn, lbIn, rbIn;
		for (j = minY; j < maxY; j+= Block_Size)
		{
			tempX = tempY;
			for (i = minX; i < maxX; i += Block_Size)
			{
				temp = tempX;
				lt = temp;
				temp.incrementX(Block_Size - 1);
				rt = temp;
				temp.incrementY(Block_Size - 1);
				rb = temp;
				temp.incrementX(1 - Block_Size);
				lb = temp;

				tempX.incrementX(Block_Size);

				ltIn = lt.evaluate();
				rtIn = rt.evaluate();
				lbIn = lb.evaluate();
				rbIn = rb.evaluate();

				if (ltIn && rbIn && rtIn && lbIn)//块在三角形内
				{
					renderInsideBlock(triangle, inv_camera_z, area.value, lt, i, j, frag, pixels, depthBuffer);
					continue;
				}
				else if (!ltIn && !rbIn && !rtIn && !lbIn)//块在三角形外
				{
					auto pointInAABB = [](const Vec2i& bMin, const Vec2i& bMax, const Vec2i& pt)
					{
						return (pt.x >= bMin.x && pt.y >= bMin.y) && (pt.x <= bMax.x && pt.y <= bMax.y);
					};
					Vec2i aabbMin(i, j);
					Vec2i aabbMax(i + Block_Size - 1, j + Block_Size - 1);
					if (pointInAABB(aabbMin, aabbMax, p0)
						|| pointInAABB(aabbMin, aabbMax, p1)
						|| pointInAABB(aabbMin, aabbMax, p2))
					{
						renderIntersetorBlock(triangle, inv_camera_z, area.value, lt, i, j, frag, pixels, depthBuffer);
						continue;
					}
					else if (BlockTriangleSegmentIntersection(aabbMin, Block_Size, p0, p1, p2))
					{
						renderIntersetorBlock(triangle, inv_camera_z, area.value, lt, i, j, frag, pixels, depthBuffer);
						continue;
					}
					continue;
				}
				renderIntersetorBlock(triangle, inv_camera_z, area.value, lt, i, j, frag, pixels, depthBuffer);
			}
			tempY.incrementY(Block_Size);
		}
	}

	//绘制边框
	void rasterizeBorder(Triangle& triangle, std::vector<Fragment>& frag, std::vector<Vec2i>& pixels)
	{
		float inv_camera_z[3];
		inv_camera_z[0] = 1 / triangle.vertex[0].pos.w;
		inv_camera_z[1] = 1 / triangle.vertex[1].pos.w;
		inv_camera_z[2] = 1 / triangle.vertex[2].pos.w;

		//齐次转换
		triangle.vertex[0].pos *= inv_camera_z[0];
		triangle.vertex[1].pos *= inv_camera_z[1];
		triangle.vertex[2].pos *= inv_camera_z[2];

		Vec2i p0, p1, p2;
		p0 = NDCToScreen(triangle.vertex[0].pos, m_viewPort);
		p1 = NDCToScreen(triangle.vertex[1].pos, m_viewPort);
		p2 = NDCToScreen(triangle.vertex[2].pos, m_viewPort);

		drawLineBres(p0, p1, frag, pixels);
		drawLineBres(p0, p2, frag, pixels);
		drawLineBres(p1, p2, frag, pixels);
	}

	void rasterizeLine(Fragment& v0, Fragment& v1, std::vector<Fragment>& frag, std::vector<Vec2i>& pixels)
	{
		v0.pos /= v0.pos.w;
		v1.pos /= v1.pos.w;

		Vec2i p0, p1;
		p0 = NDCToScreen(v0.pos, m_viewPort);
		p1 = NDCToScreen(v1.pos, m_viewPort);

		drawLineBres(p0, p1, frag, pixels);
	}
private:

	void drawLineBres(const Vec2i& v0, const Vec2i& v1, std::vector<Fragment>& frag, std::vector<Vec2i>& pixels)
	{
		int dx = abs(v1.x - v0.x);
		int dy = abs(v1.y - v0.y);
		int dx2 = dx + dx;
		int dy2 = dy + dy;
		int x, y, stepX, stepY;

		x = v0.x;
		y = v0.y;
		stepX = Sign(v1.x - v0.x);
		stepY = Sign(v1.y - v0.y);

		frag.emplace_back(Fragment(Color_White));
		pixels.emplace_back(Vec2i(x, y));
		if (dx > dy)
		{
			int p = dy2 - dx;
			int a = dy2 - dx2;
			for (int k = 0; k < dx; ++k)
			{
				x += stepX;
				if (p < 0)
				{
					p = p + dy2;
				}
				else
				{
					y += stepY;
					p = p + a;
				}

				frag.emplace_back(Fragment(Color_White));
				pixels.emplace_back(Vec2i(x, y));
			}
		}
		else
		{
			int p = dx2 - dy;
			int a = dx2 - dy2;
			for (int k = 0; k < dy; ++k)
			{
				y += stepY;
				if (p < 0)
				{
					p = p + dx2;
				}
				else
				{
					x += stepX;
					p = p + a;
				}

				frag.emplace_back(Fragment(Color_White));
				pixels.emplace_back(Vec2i(x, y));
			}
		}
	}

	//转成屏幕坐标
	Vec2i NDCToScreen(const Vec4f pos, const Viewport& viewport)
	{
		Vec2i screenPos;
		screenPos.x = pos.x * (viewport.width >> 1) + viewport.originX;
		screenPos.y = viewport.originY - pos.y * (viewport.height >> 1);
		return screenPos;
	}

	void renderInsideBlock(const Triangle& triangle, const float(&inv_camera_z)[3], const int& area, const EdgeEquationSet& set,
		const int& px, const int& py, std::vector<Fragment>& frag, std::vector<Vec2i>& pixels, float* depthBuffer)
	{
		EdgeEquationSet tempY = set;
		EdgeEquationSet tempX;

		int x_end = px + Block_Size;
		int y_end = py + Block_Size;
		int index = (py - m_viewPort.getLTY()) * m_viewPort.width + (px - m_viewPort.getLTX());
		int index_delta = m_viewPort.width - Block_Size;

		float z0, z1, z2;
		float param0, param1, param2, cameraZ, depth;
		for (int j = py; j < y_end; ++j)
		{
			tempX = tempY;
			for (int i = px; i < x_end; ++i)
			{
				param0 = inv_camera_z[0] * float(tempX.e0.value) / area;
				param1 = inv_camera_z[1] * float(tempX.e1.value) / area;
				param2 = inv_camera_z[2] * float(tempX.e2.value) / area;
				// z in viewport
				cameraZ = 1 / (param0 + param1 + param2);

				param0 *= cameraZ;
				param1 *= cameraZ;
				param2 *= cameraZ;
				//depth = z in ndc space
				depth = triangle.vertex[0].pos.z * param0;
				depth += triangle.vertex[1].pos.z * param1;
				depth += triangle.vertex[2].pos.z * param2;

				if (depth < depthBuffer[index])
				{
					depthBuffer[index] = depth;

					pixels.emplace_back(Vec2i(i, j));

					Fragment fragDest;
					BaseInterpolationFunc(triangle.vertex[0], triangle.vertex[1], triangle.vertex[2], param0, param1, param2, fragDest);
					frag.emplace_back(fragDest);
				}

				index++; 
				tempX.e0.value += tempX.e0.i;
				tempX.e1.value += tempX.e1.i;
				tempX.e2.value += tempX.e2.i;
			}

			index += index_delta;
			tempY.e0.value += tempY.e0.j;
			tempY.e1.value += tempY.e1.j;
			tempY.e2.value += tempY.e2.j;
		}
	}

	void renderIntersetorBlock(const Triangle& triangle, const float(&inv_camera_z)[3], const int& area, const EdgeEquationSet& set,
		const int& px, const int& py, std::vector<Fragment>& frag, std::vector<Vec2i>& pixels, float* depthBuffer)
	{
		EdgeEquationSet tempY = set;
		EdgeEquationSet tempX;

		int x_end = px + Block_Size;
		int y_end = py + Block_Size;
		int index = (py - m_viewPort.getLTY()) * m_viewPort.width + (px - m_viewPort.getLTX());
		int index_delta = m_viewPort.width - Block_Size;

		float z0, z1, z2;
		float param0, param1, param2, cameraZ, depth;
		for (int j = py; j < y_end; ++j)
		{
			tempX = tempY;
			for (int i = px; i < x_end; ++i)
			{
				if (tempX.e0.value <= 0 && tempX.e1.value <= 0 && tempX.e2.value <= 0)
				{
					//https://zhuanlan.zhihu.com/p/31780748
					param0 = inv_camera_z[0] * float(tempX.e0.value) / area;
					param1 = inv_camera_z[1] * float(tempX.e1.value) / area;
					param2 = inv_camera_z[2] * float(tempX.e2.value) / area;
					// z in viewport
					cameraZ = 1 / (param0 + param1 + param2);

					//depth = z in ndc space
					depth = triangle.vertex[0].pos.z * param0;
					depth += triangle.vertex[1].pos.z * param1;
					depth += triangle.vertex[2].pos.z * param2;
					depth *= cameraZ;

					if (depth < depthBuffer[index])
					{
						depthBuffer[index] = depth;

						pixels.emplace_back(Vec2i(i, j));

						Fragment fragDest;
						BaseInterpolationFunc(triangle.vertex[0], triangle.vertex[1], triangle.vertex[2], param0, param1, param2, fragDest);
						frag.emplace_back(fragDest);
					}
				}

				index++; 
				tempX.e0.value += tempX.e0.i;
				tempX.e1.value += tempX.e1.i;
				tempX.e2.value += tempX.e2.i;
			}
			index += index_delta;
			tempY.e0.value += tempY.e0.j;
			tempY.e1.value += tempY.e1.j;
			tempY.e2.value += tempY.e2.j;
		}
	}

	RasterizerInterpolationFunc m_intFunc;
	Viewport m_viewPort;
};