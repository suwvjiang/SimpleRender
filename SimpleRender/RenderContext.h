#pragma once
#include <vector>
#include "Buffer.h"
#include "Rasterizer.h"
#include "Clipper.h"

class RenderContext
{
public:
	RenderContext();
	~RenderContext();

	void setViewport(int width, int height, const Viewport& view);
	Viewport getViewport() const;

	void setVertexBuffer(std::shared_ptr<Buffer> vBuffer);
	void setIndexBuffer(std::shared_ptr<Buffer> iBuffer);

	void setVertexShader(VertexShader vs);
	void setFragmentShader(FragmentShader fs);

	void clearDepthBuffer();
	void draw(BYTE* buffer);

private:

	int m_width, m_height;
	Viewport m_viewport;
	VertexShader m_vertexShader;
	FragmentShader m_fragmentShader;

	std::shared_ptr<Rasterizer> m_rasterizer;
	std::shared_ptr<Buffer> m_vertexBuffer;
	std::shared_ptr<Buffer> m_indexBuffer;
	float* m_depthBuffer;
	size_t m_dBufferSize;

	Fragment* m_tempVertex;
	Vertex* m_vertexData;
	Triangle* m_triangles;

	inline void outputToRenderTarget(const Vec2i& pixel, const Vec3f& color, BYTE* buffer)
	{
		if (NULL == buffer) return;
		if (pixel.x < 0 || pixel.y < 0) return;
		if (pixel.y * m_width * 3 + pixel.x * 3 + 3 > m_width * m_height* PIX_BITS / 8) return;

		buffer[pixel.y * m_width * 3 + pixel.x * 3 + 1] = color.g*255;
		buffer[pixel.y * m_width * 3 + pixel.x * 3 + 2] = color.r*255;
		buffer[pixel.y * m_width * 3 + pixel.x * 3 + 3] = color.b*255;
	}
};

