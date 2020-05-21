#include "RenderContext.h"

RenderContext::RenderContext()
{
	m_rasterizer = std::make_shared<Rasterizer>();
}

RenderContext::~RenderContext()
{
	m_rasterizer = nullptr;
}

void RenderContext::setViewport(const Viewport& view)
{
	m_viewport = view;
	m_rasterizer->setViewport(view);

	m_depthBuffer.clear();
	for (int i = view.width-1; i >= 0; --i)
	{
		for (int j = view.height-1; j >= 0; --j)
		{
			m_depthBuffer.emplace_back(-1.0);
		}
	}
}
Viewport RenderContext::getViewport() const
{
	return m_viewport;
}

void RenderContext::setVertexBuffer(std::shared_ptr<Buffer> vBuffer)
{
	m_vertexBuffer = vBuffer;
}

void RenderContext::setIndexBuffer(std::shared_ptr<Buffer> iBuffer)
{
	m_indexBuffer = iBuffer;
}

void RenderContext::setVertexShader(VertexShader vs)
{
	m_vertexShader = vs;
}
void RenderContext::setFragmentShader(FragmentShader fs)
{
	m_fragmentShader = fs;
}

void RenderContext::clearDepthBuffer()
{
	auto itBegin = m_depthBuffer.begin();
	auto itEnd = m_depthBuffer.end();
	while (itBegin != itEnd)
	{
		*itBegin = -1.0;
		++itBegin;
	}
}

void RenderContext::draw(HDC& hdc)
{
	if ((m_vertexBuffer == nullptr)||(m_indexBuffer == nullptr))
	{
		throw std::exception("Error:vertex buffer or index buffer is nullptr");
	}

	size_t vertexNum = m_vertexBuffer->getNumOfEle();
	Vertex* vertexData = static_cast<Vertex*>(m_vertexBuffer->getData());
	Fragment* processedVertex = new Fragment[vertexNum];

	//������ɫ
	for (size_t i = 0; i < vertexNum; ++i)
	{
		m_vertexShader(vertexData[i], processedVertex[i]);
	}

	//����������
	size_t triangleNum = m_indexBuffer->getNumOfEle() / 3;
	size_t* indexData = static_cast<size_t*>(m_indexBuffer->getData());
	Triangle* triangles = new Triangle[triangleNum];
	size_t begin;
	for (size_t i = 0; i < triangleNum; ++i)
	{
		begin = i * 3;
		triangles[i].vertex[0] = processedVertex[indexData[begin]];
		triangles[i].vertex[1] = processedVertex[indexData[begin + 1]];
		triangles[i].vertex[2] = processedVertex[indexData[begin + 2]];
	}
	delete[] processedVertex;

	//todo-clip

	std::vector<Fragment> fragments;
	std::vector<Vec2i> pixels;

	for (size_t i = 0; i < triangleNum; ++i)
	{
		m_rasterizer->Rasterize(triangles[i], fragments, pixels, m_depthBuffer);

		size_t fragmentSize = fragments.size();
		Vec3f* fragmentOut = new Vec3f();

		for (size_t j = 0; j < fragmentSize; ++j)
		{
			m_fragmentShader(fragments[j], fragmentOut);

			OutputToRenderTarget(pixels[j], *fragmentOut, hdc);
		}

		fragments.clear();
		pixels.clear();
		delete fragmentOut;
	}

	delete[] triangles;
}