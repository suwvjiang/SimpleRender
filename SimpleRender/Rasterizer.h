#pragma once
#include <functional>
#include "MathUtils.h"

//�߽�Ȩ��
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
//��������ϵ
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
//��������
struct Vertex
{
	Vec3f pos;
	Vec2f uv;
	Vec3f color;

	Vertex() {};
	Vertex(const Vec3f& _pos, const Vec2f& _uv, const Vec3f& _color):pos(_pos), uv(_uv), color(_color)
	{
	}
	Vertex(const float& x, const float& y, const float& z,
		const float& r, const float& g, const float& b):pos(x, y, z), color(r, g, b)
	{
	}
};
//Ƭ������
struct Fragment
{
	Vec4f pos;
	Vec2f uv;
	Vec3f color;
};
//������
struct Triangle
{
	Fragment vertex[3];
};
//��Ⱦ����
struct Viewport
{
	int originX;
	int originY;
	int width;
	int height;
};
//��ɫ��
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
	}

	inline static void FS(const Fragment& input, Vec3f* output)
	{
		(*output) = input.color;
	}
};

using VertexShader = std::function<void(const Vertex& pVertexIn, Fragment& pVertexOut)>;
using FragmentShader = std::function<void(const Fragment& pFragmentIn, Vec3f *pFragmentOut)>;
using RasterizerInterpolationFunc = std::function<void(const Fragment & frag0, const Fragment & frag1, const Fragment & frag2,
	const float& t0, const float& t1, const float& t2, Fragment & destFrag)>;
using RenderOrder = std::function<void(int x, int y, const Vec3f & col)>;

//Ƭ����ɫ��ֵ����
inline void BaseInterpolationFunc(const Fragment& frag0, const Fragment& frag1, const Fragment& frag2,
	const float& t0, const float& t1, const float& t2, Fragment& destFrag)
{
	destFrag.pos += frag0.pos * t0;
	destFrag.pos += frag1.pos * t1;
	destFrag.pos += frag2.pos * t2;

	destFrag.uv += frag0.uv * t0;
	destFrag.uv += frag1.uv * t1;
	destFrag.uv += frag2.uv * t2;

	destFrag.color += frag0.color * t0;
	destFrag.color += frag1.color * t1;
	destFrag.color += frag2.color * t2;
}

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

	void Rasterize(Triangle& triangle, std::vector<Fragment>& frag, std::vector<Vec2i>& pixels, std::vector<float>& depthBuffer)
	{
		float inv_camera_z[3];
		inv_camera_z[0] = 1 / triangle.vertex[0].pos.w;
		inv_camera_z[1] = 1 / triangle.vertex[1].pos.w;
		inv_camera_z[2] = 1 / triangle.vertex[2].pos.w;

		//���ת��
		triangle.vertex[0].pos *= inv_camera_z[0];
		triangle.vertex[1].pos *= inv_camera_z[1];
		triangle.vertex[2].pos *= inv_camera_z[2];

		Vec2i p0, p1, p2, p;
		p0 = NDCToViewport(triangle.vertex[0].pos, m_viewPort);
		p1 = NDCToViewport(triangle.vertex[1].pos, m_viewPort);
		p2 = NDCToViewport(triangle.vertex[2].pos, m_viewPort);

		int minX, minY, maxX, maxY;
		minX = max(min(min(p0.x, p1.x), p2.x), 0);
		minY = max(min(min(p0.y, p1.y), p2.y), 0);
		maxX = min(max(max(p0.x, p1.x), p2.x), m_viewPort.width);
		maxY = min(max(max(p0.y, p1.y), p2.y), m_viewPort.height);
		if (minX > maxX)
			return;
		if (minY > maxY)
			return;

		EdgeEquation area(p0, p1, p2);
		p.x = minX;
		p.y = minY;
		EdgeEquationSet tempY(p0, p1, p2, p);
		EdgeEquationSet tempX;

		int i, j;
		int index = minY * m_viewPort.width + minX;
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
					depth = triangle.vertex[0].pos.z * param0
						+ triangle.vertex[1].pos.z * param1
						+ triangle.vertex[2].pos.z * param2;
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
private:

	//ת����Ļ����
	Vec2i NDCToViewport(const Vec4f pos, const Viewport& viewport)
	{
		Vec2i screenPos;
		screenPos.x = pos.x * viewport.width + viewport.originX;
		screenPos.y = viewport.originY - pos.y * viewport.height;
		return screenPos;
	}

	RasterizerInterpolationFunc m_intFunc;
	Viewport m_viewPort;
};