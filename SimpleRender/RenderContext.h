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

	void setViewport(const Viewport& view);
	Viewport getViewport() const;

	void setVertexBuffer(std::shared_ptr<Buffer> vBuffer);
	void setIndexBuffer(std::shared_ptr<Buffer> iBuffer);

	void setVertexShader(VertexShader vs);
	void setFragmentShader(FragmentShader fs);

	void clearDepthBuffer();
	void draw(HDC& hdc);

private:
	Viewport m_viewport;
	VertexShader m_vertexShader;
	FragmentShader m_fragmentShader;

	std::shared_ptr<Rasterizer> m_rasterizer;
	std::shared_ptr<Buffer> m_vertexBuffer;
	std::shared_ptr<Buffer> m_indexBuffer;
	std::vector<float> m_depthBuffer;

	void outputToRenderTarget(const Vec2i& pixel, const Vec3f& color, HDC& target)
	{
		SetPixel(target, pixel.x, pixel.y, RGB(255 * color.r, 255 * color.g, 255 * color.b));
	}
};

