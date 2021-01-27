#pragma once
#include <vector>
#include "Buffer.h"
#include "Rasterizer.h"
#include "Clipper.h"
#include "TGAImage.h"

struct Material
{
	VertexShader vertexShader = NULL;
	FragmentShader fragmentShader = NULL;
	std::shared_ptr<TGAImage> mainTexture = nullptr;
};

class RenderContext
{
public:
	RenderContext();
	~RenderContext();

	void setViewport(int width, int height, const Viewport& view);
	void initMeshInfo();

	std::shared_ptr<Buffer> createBuffer(const BufferDesc& desc);

	void setVertexBuffer(std::shared_ptr<Buffer> vBuffer);
	void setIndexBuffer(std::shared_ptr<Buffer> iBuffer);

	void setLocalPos(const float& x, const float& y, const float& z)
	{
		m_offsetX = x;
		m_offsetY = y;
		m_offsetZ = z;
	}
	//x-y-z
	void setLocalRotate(const float& x, const float& y, const float& z)
	{
		m_eulerX = x;
		m_eulerY = y;
		m_eulerZ = z;
	}
	void setLocalScale(const float& scale_x, const float& scale_y, const float& scale_z)
	{
		m_scaleX = scale_x;
		m_scaleY = scale_y;
		m_scaleZ = scale_z;
	}
	Matrix4x4f getTransform();

	void draw(BYTE* buffer, float* depthBuffer);

private:

	int m_width, m_height;
	float m_offsetX = 0, m_offsetY = 0, m_offsetZ = 0;
	float m_eulerX = 0, m_eulerY = 0, m_eulerZ = 0;
	float m_scaleX = 1, m_scaleY = 1, m_scaleZ = 1;

	Matrix4x4f m_transform;
	std::shared_ptr<Rasterizer> m_rasterizer;
	std::shared_ptr<Buffer> m_vertexBuffer;
	std::shared_ptr<Buffer> m_indexBuffer;
	std::shared_ptr <Material> m_material;

	Fragment* m_tempVertex;
	Vertex* m_vertexData;
	Triangle* m_triangles;

	inline void outputToRenderTarget(const int& pixel, const Vec3f& color, BYTE* buffer)
	{
		if (NULL == buffer) return;
		if (pixel < 0) return;
		if (pixel * 3 + 3 > m_width * m_height* PIX_BITS / 8) return;

		buffer[pixel * 3 + 1] = color.g*255;
		buffer[pixel * 3 + 2] = color.r*255;
		buffer[pixel * 3 + 3] = color.b*255;
	}
};

