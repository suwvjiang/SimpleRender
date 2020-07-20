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

	void incrementX()
	{
		value += i;
	}

	void incrementY()
	{
		value += j;
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

	void incrementX()
	{
		e0.incrementX();
		e1.incrementX();
		e2.incrementX();
	}

	void incrementY()
	{
		e0.incrementY();
		e1.incrementY();
		e2.incrementY();
	}

	bool evaluate()
	{
		return e0.value >= 0 && e1.value >= 0 && e2.value >= 0;
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
	};

	static ConstBuffer constBuffer;

	inline static void VS(const Vertex& input, Fragment& output)
	{
		Vec4f pos(input.pos.x, input.pos.y, input.pos.z, 1);
		Vec4f worldPos = Transform(constBuffer.world, pos);
		output.pos = Transform(constBuffer.view_proj, worldPos);
		output.color = input.color;
		output.uv = input.uv;
	}

	inline static void FS(const Fragment& input, Vec3f* output)
	{
		(*output) = input.color;
	}

	inline static void TextFS(const Fragment& input, Vec3f* output)
	{
		int u = input.uv.x * 255;
		int v = input.uv.y * 255;
		u /= 32;
		v /= 32;
		(*output) = ((u + v) & 1) ? Color_White : Color_Blue;
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
	destFrag.pos += frag0.pos * t0;
	destFrag.pos += frag1.pos * t1;
	destFrag.pos += frag2.pos * t2;

	destFrag.normal += frag0.normal * t0;
	destFrag.normal += frag1.normal * t1;
	destFrag.normal += frag2.normal * t2;

	destFrag.uv += frag0.uv * t0;
	destFrag.uv += frag1.uv * t1;
	destFrag.uv += frag2.uv * t2;

	destFrag.color += frag0.color * t0;
	destFrag.color += frag1.color * t1;
	destFrag.color += frag2.color * t2;
}

static constexpr int blockSize = 16;

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

	void rasterize(Triangle& triangle, std::vector<Fragment>& frag, std::vector<Vec2i>& pixels, std::vector<float>& depthBuffer)
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
		if (area.value < 0)
			return;

		p.x = minX;
		p.y = minY;
		EdgeEquationSet tempY(p0, p1, p2, p);
		EdgeEquationSet tempX;

		int i, j;
		int index = (minY - m_viewPort.getLTY()) * m_viewPort.width + (minX - m_viewPort.getLTX());
		float z0, z1, z2;
		float param0, param1, param2, cameraZ, depth;
		for (j = minY; j < maxY; ++j)
		{
			tempX = tempY;
			for (i = minX; i < maxX; ++i)
			{
				if (tempX.evaluate())
				{
					param0 = float(tempX.e0.value) * inv_camera_z[0] / area.value;
					param1 = float(tempX.e1.value) * inv_camera_z[1] / area.value;
					param2 = float(tempX.e2.value) * inv_camera_z[2] / area.value;
					// z in viewport
					cameraZ = 1 / (param0 + param1 + param2);

					param0 *= cameraZ;
					param1 *= cameraZ;
					param2 *= cameraZ;
					//depth = z in ndc space
					depth = triangle.vertex[0].pos.z * param0;
					depth += triangle.vertex[1].pos.z * param1;
					depth += triangle.vertex[2].pos.z * param2;
					if (depth > depthBuffer[index])
					{
						depthBuffer[index] = depth;

						pixels.emplace_back(Vec2i(i, j));

						Fragment fragDest;
						BaseInterpolationFunc(triangle.vertex[0], triangle.vertex[1], triangle.vertex[2], param0, param1, param2, fragDest);
						frag.emplace_back(fragDest);
					}
				}

				index++;
				tempX.incrementX();
			}
			index += m_viewPort.width - maxX + minX;
			tempY.incrementY();
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

	RasterizerInterpolationFunc m_intFunc;
	Viewport m_viewPort;
};