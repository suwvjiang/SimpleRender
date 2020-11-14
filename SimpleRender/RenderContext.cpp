#include "RenderContext.h"

RenderContext::RenderContext()
{
	m_rasterizer = std::make_shared<Rasterizer>();
}

RenderContext::~RenderContext()
{
	delete[] m_tempVertex;
	delete[] m_triangles;
	delete[] m_depthBuffer;
	m_rasterizer = nullptr;
}

void RenderContext::setViewport(int width, int height, const Viewport& view)
{
	m_width = width;
	m_height = height;

	m_viewport = view;
	m_rasterizer->setViewport(view);

	m_dBufferSize = width * height;
	m_depthBuffer = new float[m_dBufferSize];
	for (int i = m_dBufferSize - 1; i >= 0; --i)
		m_depthBuffer[i] = 1.0f;
}
Viewport RenderContext::getViewport() const
{
	return m_viewport;
}

void RenderContext::setVertexBuffer(std::shared_ptr<Buffer> vBuffer)
{
	m_vertexBuffer = vBuffer;
	size_t vertexNum = m_vertexBuffer->getNumOfEle();
	m_vertexData = static_cast<Vertex*>(m_vertexBuffer->getData());
	m_tempVertex = new Fragment[vertexNum];
}

void RenderContext::setIndexBuffer(std::shared_ptr<Buffer> iBuffer)
{
	m_indexBuffer = iBuffer;

	size_t triangleNum = m_indexBuffer->getNumOfEle() / 3;
	m_triangles = new Triangle[triangleNum];
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
	for (int i = m_dBufferSize - 1; i >= 0; --i)
		m_depthBuffer[i] = 1.0f;
}

void RenderContext::draw(BYTE* buffer)
{
	if ((m_vertexBuffer == nullptr)||(m_indexBuffer == nullptr))
	{
		throw std::exception("Error:vertex buffer or index buffer is nullptr");
	}

	size_t vertexNum = m_vertexBuffer->getNumOfEle();
	//顶点着色
	for (size_t i = 0; i < vertexNum; ++i)
	{
		m_vertexShader(m_vertexData[i], m_tempVertex[i]);
	}
	
	//构建三角形
	size_t triangleNum = m_indexBuffer->getNumOfEle() / 3;
	size_t* indexData = static_cast<size_t*>(m_indexBuffer->getData());
	size_t begin;
	for (size_t i = 0; i < triangleNum; ++i)
	{
		begin = i * 3;
		m_triangles[i].vertex[0] = m_tempVertex[indexData[begin]];
		m_triangles[i].vertex[1] = m_tempVertex[indexData[begin + 1]];
		m_triangles[i].vertex[2] = m_tempVertex[indexData[begin + 2]];
	}
	//todo-clip
	CVVClip(&m_triangles, triangleNum);

	std::vector<Fragment> fragments;
	std::vector<Vec2i> pixels;

	Vec3f* fragmentOut = new Vec3f();
	for (size_t i = 0; i < triangleNum; ++i)
	{
		m_rasterizer->rasterize(m_triangles[i], fragments, pixels, m_depthBuffer);
		//m_rasterizer->rasterizeBorder(triangles[i], fragments, pixels);

		size_t fragmentSize = fragments.size();
		for (size_t j = 0; j < fragmentSize; ++j)
		{
			m_fragmentShader(fragments[j], fragmentOut);
			outputToRenderTarget(pixels[j], *fragmentOut, buffer);
		}

		fragments.clear();
		pixels.clear();
	}
	delete fragmentOut;
}