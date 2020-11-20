#include "RenderContext.h"
#include "ObjParse.h"

RenderContext::RenderContext()
{
	m_rasterizer = std::make_shared<Rasterizer>();
	m_material = std::make_shared<Material>();
	m_material->vertexShader = ShaderStruct::VS;
	m_material->fragmentShader = ShaderStruct::LightFS;

	m_transform = IdentityMatrix4x4<float>();
}

RenderContext::~RenderContext()
{
	delete[] m_tempVertex;
	delete[] m_triangles;
	m_vertexData = nullptr;
	m_rasterizer = nullptr;
	m_material = nullptr;
}

void RenderContext::setViewport(int width, int height, const Viewport& view)
{
	m_width = width;
	m_height = height;

	m_rasterizer->setViewport(view);
}

void RenderContext::initMeshInfo()
{
#pragma region Panel
	float w2 = 1;
	float h2 = 1;
	float d2 = 1;

	//Vertex
	Vertex v[24];
	v[0] = Vertex(-w2, -h2, -d2, 1, 0, 0, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 1, 1, 0, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(+w2, +h2, -d2, 0, 1, 1, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[3] = Vertex(+w2, -h2, -d2, 1, 0, 1, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	v[4] = Vertex(-w2, -h2, +d2, 0, 1, 1, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 0, 1, 0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 1, 0, 0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[7] = Vertex(-w2, +h2, +d2, 0, 0, 1, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	v[8] = Vertex(-w2, +h2, -d2, 1, 1, 0, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[9] = Vertex(-w2, +h2, +d2, 0, 0, 1, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+w2, +h2, +d2, 1, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+w2, +h2, -d2, 0, 1, 1, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	v[12] = Vertex(-w2, -h2, -d2, 1, 0, 0, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 1, 0, 1, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 0, 1, 0, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-w2, -h2, +d2, 0, 1, 1, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	v[16] = Vertex(-w2, -h2, +d2, 0, 1, 1, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, +h2, +d2, 0, 0, 1, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, +h2, -d2, 1, 1, 0, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[19] = Vertex(-w2, -h2, -d2, 1, 0, 0, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	v[20] = Vertex(+w2, -h2, -d2, 1, 0, 1, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[21] = Vertex(+w2, +h2, -d2, 0, 1, 1, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[22] = Vertex(+w2, +h2, +d2, 1, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[23] = Vertex(+w2, -h2, +d2, 0, 1, 0, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	//Index
	//逆时针
	size_t i[36];
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;
#pragma endregion

	std::vector<Vertex> vertices;
	std::vector<int> indexs;

	string objPath("D:\\Projects\\SimpleRender\\model\\african_head\\african_head.obj");
	loadObj(objPath, vertices, indexs);

	BufferDesc vertexDesc;
	/*vertexDesc.stride = sizeof(Vertex);
	vertexDesc.numOfEle = 24;
	vertexDesc.data = v;
	vertexDesc.bufferSize = sizeof(Vertex) * 24;*/
	vertexDesc.stride = sizeof(Vertex);
	vertexDesc.numOfEle = vertices.size();
	vertexDesc.data = vertices.data();
	vertexDesc.bufferSize = sizeof(Vertex) * vertices.size();

	BufferDesc indexDesc;
	/*indexDesc.stride = sizeof(size_t);
	indexDesc.numOfEle = 36;
	indexDesc.data = i;
	indexDesc.bufferSize = sizeof(size_t) * 36;*/
	indexDesc.stride = sizeof(size_t);
	indexDesc.numOfEle = indexs.size();
	indexDesc.data = indexs.data();
	indexDesc.bufferSize = sizeof(size_t) * indexs.size();

	m_vertexBuffer = createBuffer(vertexDesc);
	setVertexBuffer(m_vertexBuffer);

	m_indexBuffer = createBuffer(indexDesc);
	setIndexBuffer(m_indexBuffer);
}

std::shared_ptr<Buffer> RenderContext::createBuffer(const BufferDesc& desc)
{
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(desc);
	return buffer;
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

Matrix4x4f RenderContext::getTransform()
{
	m_transform[0][3] = m_offsetX;
	m_transform[1][3] = m_offsetY;
	m_transform[2][3] = m_offsetZ;

	float cosX = cos(m_eulerX), sinX = sin(m_eulerX);
	float cosY = cos(m_eulerY), sinY = sin(m_eulerY);
	float cosZ = cos(m_eulerZ), sinZ = sin(m_eulerZ);
	m_transform[0][0] = cosZ * cosY * m_scaleX;
	m_transform[0][1] = cosZ * sinY * sinX - sinZ*cosX;
	m_transform[0][2] = sinZ * sinX + cosZ * sinY * cosX;
	m_transform[1][0] = sinZ * cosY;
	m_transform[1][1] = (cosZ * cosX + sinZ * sinY * sinX) * m_scaleY;
	m_transform[1][2] = sinZ * sinY * cosX - cosZ * sinX;
	m_transform[2][0] = -sinY;
	m_transform[2][1] = cosY * sinX;
	m_transform[2][2] = cosY * cosX * m_scaleZ;

	return m_transform;
}

void RenderContext::draw(BYTE* buffer, float* depthBuffer)
{
	if ((m_vertexBuffer == nullptr)||(m_indexBuffer == nullptr))
	{
		throw std::exception("Error:vertex buffer or index buffer is nullptr");
		return;
	}

	if (m_material == nullptr)
	{
		throw std::exception("Error:m_material is nullptr");
		return;
	}

	if (m_material->vertexShader == NULL || m_material->fragmentShader == NULL)
	{
		throw std::exception("Error:m_material's vertexShader or fragmentShader is nullptr");
		return;
	}

	size_t vertexNum = m_vertexBuffer->getNumOfEle();
	//顶点着色
	for (size_t i = 0; i < vertexNum; ++i)
	{
		ShaderStruct::VS(m_vertexData[i], m_tempVertex[i]);
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
		m_rasterizer->rasterize(m_triangles[i], fragments, pixels, depthBuffer);
		//m_rasterizer->rasterizeBorder(triangles[i], fragments, pixels);

		size_t fragmentSize = fragments.size();
		for (size_t j = 0; j < fragmentSize; ++j)
		{
			ShaderStruct::LightFS(fragments[j], fragmentOut);
			outputToRenderTarget(pixels[j], *fragmentOut, buffer);
		}

		fragments.clear();
		pixels.clear();
	}
	delete fragmentOut;
}