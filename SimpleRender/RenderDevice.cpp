#include "RenderDevice.h"
#include "Rasterizer.h"
#include "ObjParse.h"

ShaderStruct::ConstBuffer ShaderStruct::constBuffer;

RenderDevice::RenderDevice()
{
}

RenderDevice::~RenderDevice()
{
	m_context3D = nullptr;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_camera = nullptr;
}

bool RenderDevice::isInited()
{
	return m_inited;
}

void RenderDevice::initDevice(int width, int height)
{
	m_fragmentBuff = new BYTE[width * height * PIX_BITS / 8];
	memset(m_fragmentBuff, 45, width * height * PIX_BITS / 8);

	winWidth = width;
	winHeight = height; 

	m_viewPort.width = width-50;
	m_viewPort.height = height-50;
	m_viewPort.originX = width >> 1;
	m_viewPort.originY = height >> 1;

	m_context3D = std::make_shared<RenderContext>();
	m_context3D->setViewport(width, height, m_viewPort);

	m_radius = 6;
	m_camera = std::make_shared<Camera>();
	//identity Project Matrix
	float fov = PI/2, aspect = 1.0;
	float zNear = -1, zFar = -1000;
	m_camera->setViewInfo(fov, aspect, zNear, zFar);

	m_origin.x = width >> 1;
	m_origin.y = (height >> 1);
	m_origin.z = 0;
	m_camera->setFocusPos(m_origin);

	ShaderStruct::constBuffer.world = Matrix4x4TranslationFromVector<float>(m_origin-Vec3f(0, 1, 0));

	initMeshInfo();
}

void RenderDevice::releaseDevice()
{

}

void RenderDevice::clear()
{
	memset(m_fragmentBuff, 45, winWidth * winHeight * PIX_BITS / 8);
}

const static float Rotate_Speed = 360.0;
//更新相机信息
void RenderDevice::update()
{
	m_angle += PI / Rotate_Speed;
	Vec3f pos(m_radius * sin(m_angle), 0, m_radius * cos(m_angle));
	pos += m_origin;
	m_camera->setPos(pos);
	m_camera->update();

	ShaderStruct::constBuffer.view_proj = m_camera->viewProjMatrix();
}
//绘制
void RenderDevice::drawcall()
{
	Vec2i viewportMin = Vec2i(m_viewPort.originX - (m_viewPort.width >> 1), m_viewPort.originY - (m_viewPort.height >> 1));
	Vec2i viewportMax = Vec2i(m_viewPort.originX + (m_viewPort.width >> 1), m_viewPort.originY + (m_viewPort.height >> 1));
	drawViewportArea(viewportMin, viewportMax);
	
	m_context3D->clearDepthBuffer();
	m_context3D->setIndexBuffer(m_indexBuffer);
	m_context3D->setVertexBuffer(m_vertexBuffer);

	m_context3D->setVertexShader(ShaderStruct::VS);
	m_context3D->setFragmentShader(ShaderStruct::FS);

	m_context3D->draw(m_fragmentBuff);
}

void RenderDevice::initMeshInfo()
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

	string objPath("D:\\Projects\\SimpleRender\\model\\teapot.obj");
	loadObj(objPath, vertices, indexs);

	BufferDesc vertexDesc;
	/*vertexDesc.stride = sizeof(Vertex);
	vertexDesc.numOfEle = 24;
	vertexDesc.data = v;
	vertexDesc.bufferSize = sizeof(Vertex) * 24; */
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
	m_indexBuffer = createBuffer(indexDesc);
}

std::shared_ptr<Buffer> RenderDevice::createBuffer(const BufferDesc& desc)
{
	std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(desc);
	return buffer;
}

#pragma region Raster Function
void RenderDevice::drawPixel(int x, int y)
{
	//SetPixel(mScreenHDC, x, y, RGB(255, 255, 255));
}

void RenderDevice::drawLineDDA(int x0, int y0, int xEnd, int yEnd)
{
	float dx = xEnd - x0;
	float dy = yEnd - y0;
	int steps, k;

	float xIncrement, yIncrement, x = x0, y = y0;

	if (abs(dx) > abs(dy))
	{
		steps = dx;
	}
	else
	{
		steps = dy;
	}

	xIncrement = dx / steps;
	yIncrement = dy / steps;

	drawPixel(round(x), round(y));
	for (k = 0; k < steps; ++k)
	{
		x = x0 + xIncrement * k;
		y = y0 + yIncrement * k;
		drawPixel(round(x), round(y));
	}
}

void RenderDevice::drawLineBres(int x0, int y0, int xEnd, int yEnd)
{
	int dx = abs(xEnd - x0);
	int dy = abs(yEnd - y0);
	int dx2 = dx + dx;
	int dy2 = dy + dy;
	int x, y, stepX, stepY;

	x = x0;
	y = y0;
	stepX = Sign(xEnd - x0);
	stepY = Sign(yEnd - y0);

	drawPixel(x, y);
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
			drawPixel(x, y);
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
			drawPixel(x, y);
		}
	}
}

//中点法画圆
void RenderDevice::circlePlotPoints(int x, int y, Vec2i& point)
{
	drawPixel(x + point.x, y + point.y);
	drawPixel(x + point.x, y - point.y);
	drawPixel(x - point.x, y + point.y);
	drawPixel(x - point.x, y - point.y);
	drawPixel(x + point.y, y + point.x);
	drawPixel(x + point.y, y - point.x);
	drawPixel(x - point.y, y + point.x);
	drawPixel(x - point.y, y - point.x);
}
void RenderDevice::circleMidPoint(int x, int y, int radius)
{
	Vec2i circPt(0, radius);
	int p = 1 - radius;
	circlePlotPoints(x, y, circPt);

	while (circPt.x < circPt.y)
	{
		circPt.x++;

		if (p < 0)
		{
			p += 2 * circPt.x + 1;
		}
		else
		{
			circPt.y--;
			p += 2 * circPt.x - 2 * circPt.y + 1;
		}

		circlePlotPoints(x, y, circPt);
	}
}

//中点画椭圆
void RenderDevice::ellipsePlotPoints(int xCenter, int yCenter, int x, int y)
{
	drawPixel(xCenter + x, yCenter + y);
	drawPixel(xCenter + x, yCenter - y);
	drawPixel(xCenter - x, yCenter + y);
	drawPixel(xCenter - x, yCenter - y);
}
void RenderDevice::ellipseMidPoint(int xCenter, int yCenter, int xRadius, int yRadius)
{
	const int Rx2 = xRadius * xRadius;
	const int Ry2 = yRadius * yRadius;
	const int tRx2 = 2 * Rx2;
	const int tRy2 = 2 * Ry2;

	int p = 0;
	int x = 0;
	int y = yRadius;
	int px = 0;
	int py = tRx2 * y;

	ellipsePlotPoints(xCenter, yCenter, x, y);

	p = round(Ry2 - (Rx2 * yRadius) + (0.25f * Rx2));
	while (px < py)
	{
		x++;
		px += tRy2;
		if (p < 0)
		{
			p += Ry2 + px;
		}
		else
		{
			y--;
			py -= tRx2;
			p += Ry2 + px - py;
		}
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}

	p = round(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
	while (y > 0)
	{
		y--;
		py -= tRx2;
		if (p > 0)
		{
			p += Rx2 - py;
		}
		else
		{
			x++;
			px += tRy2;
			p += Rx2 - py + px;
		}
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
}

//中点抛物线
void RenderDevice::midPointGravity(int vx0, int vy0, int x0, int y0)
{
	const int gravity = 980;
	const int vxy = vx0 * vy0;
	const int gx = gravity * (x0 - 0.5f);
	const int vxt = vx0 * vx0;
	const int xEnd = (vxy / gravity) + x0;

	int x = x0, y = y0;
	int xg = x * gravity;
	drawPixel(x, y);
	drawPixel(xEnd + xEnd - x, y);

	int temp = round((vy0 * vy0 - vxt) / (2 * gravity)) + y0;
	int p = vxt + 0.125 * gravity - 0.5 * vxy;
	while (y < temp)
	{
		y++;
		if (p > 0)
		{
			x++;
			xg += gravity;
			p += vxt + xg - gx - gravity - vxy;
		}
		else
		{
			p += vxt;
		}

		drawPixel(x, y);
		drawPixel(xEnd + xEnd - x, y);
	}

	p = (0.5f + y - y0) * vxt - vxy * (x + 1 - x0) + 0.5f * gravity * (x + 1 - x0) * (x + 1 - x0);
	while (x < xEnd)
	{
		x++;
		xg += gravity;
		if (p < 0)
		{
			y++;
			p += xg - gx - vxy + vxt;
		}
		else
		{
			p += xg - gx - vxy;
		}
		drawPixel(x, y);
		drawPixel(xEnd + xEnd - x, y);
	}
}

//下平底三角形
void RenderDevice::drawBottomFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	if (y0 != y1)
	{
		std::cout << "this trangle is not bottom flat" << std::endl;
		return;
	}

	float deltax = x2 - x0;
	float deltay = y2 - y0;
	float deltaL = deltax / deltay;
	deltax = x2 - x1;
	deltay = y2 - y1;
	float deltaR = deltax / deltay;
	int k;

	for (k = y0; k < y2; ++k)
	{
		int xl = round((k - y0) * deltaL + x0);
		int xr = round((k - y1) * deltaR + x1);
		drawLineBres(xl, k, xr, k);
	}
}

//上平底三角色
void RenderDevice::drawTopFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	if (y0 != y1)
	{
		std::cout << "this trangle is not top flat" << std::endl;
		return;
	}

	float deltax = x2 - x0;
	float deltay = y2 - y0;
	float deltaL = deltax / deltay;
	deltax = x2 - x1;
	deltay = y2 - y1;
	float deltaR = deltax / deltay;
	int k;

	for (k = y2; k < y0; ++k)
	{
		int xl = round((k - y0) * deltaL + x0);
		int xr = round((k - y1) * deltaR + x1);
		drawLineBres(xl, k, xr, k);
	}
}

//绘制任意三角形
void RenderDevice::drawTrangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	//todo y2>y0>y1
	if (y0 < y1)
	{
		std::swap(y1, y0);
		std::swap(x1, x0);
	}
	if (y2 < y1)
	{
		std::swap(y2, y1);
		std::swap(x2, x1);
	}
	if (y2 < y0)
	{
		std::swap(y2, y0);
		std::swap(x2, x0);
	}

	if (y0 == y1)//下平底
		drawBottomFlatTrangle(x0, y0, x1, y1, x2, y2);
	else if (y0 == y2)//上平底
		drawTopFlatTrangle(x0, y0, x2, y2, x1, y1);
	else
	{
		float deltaX = x2 - x1;
		float dletaY = y2 - y1;
		float delta = deltaX / dletaY;
		int xAdd = round((y0 - y1) * delta + x1);
		int yAdd = y0;
		if (xAdd < x0)
		{
			std::swap(xAdd, x0);
			std::swap(yAdd, y0);
		}

		drawBottomFlatTrangle(x0, y0, xAdd, yAdd, x2, y2);
		drawTopFlatTrangle(x0, y0, xAdd, yAdd, x1, y1);
	}
}

//绘制边框
void RenderDevice::drawTrangleBorder(int nVerts, Vec2i* verts)
{
	for (int k = 1; k < nVerts; ++k)
	{
		if (k == 1)
		{
			drawLineBres(verts[0].x, verts[0].y, verts[1].x, verts[1].y);
			drawLineBres(verts[nVerts - 1].x, verts[nVerts - 1].y, verts[1].x, verts[1].y);
		}
		else
		{
			drawLineBres(verts[0].x, verts[0].y, verts[k].x, verts[k].y);
			drawLineBres(verts[k - 1].x, verts[k - 1].y, verts[k].x, verts[k].y);
		}
	}
}

//绘制多边形
void RenderDevice::drawPolygon(int nVerts, Vec2i* verts)
{
	if (nVerts < 3)
		return;

	Vec2i p0, p1, p2;
	p0 = verts[0];
	for (int k = 2; k < nVerts; ++k)
	{
		p1 = verts[k - 1];
		p2 = verts[k];

		drawTrangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
	}
}

//绘制彩点
void RenderDevice::drawColorPixel(int x, int y, const Vec3f& col)
{
	//SetPixel(screenHDC, x, y, RGB(255 * col.r, 255 * col.g, 255 * col.b));
}

//绘制彩线
void RenderDevice::drawColorLine(int x0, int y0, int xEnd, int yEnd, const Vec3f& col0, const Vec3f& colEnd)
{
	float dx = xEnd - x0;
	float dy = yEnd - y0;
	int steps, k;

	float xIncrement, yIncrement, x = x0, y = y0;

	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);

	xIncrement = dx / steps;
	yIncrement = dy / steps;

	drawColorPixel(round(x), round(y), col0);
	for (k = 0; k < steps; ++k)
	{
		x = x0 + xIncrement * k;
		y = y0 + yIncrement * k;
		drawColorPixel(round(x), round(y), Lerp(col0, colEnd, float(k) / steps));
	}
}

//
void RenderDevice::drawTrangleByHalfSpace(const Vec2i& v0, const Vec2i& v1, const Vec2i& v2, const Vec3f& col0, const Vec3f& col1, const Vec3f& col2)
{
	int minX, minY, maxX, maxY;
	minX = max(min(min(v0.x, v1.x), v2.x), 0);
	minY = max(min(min(v0.y, v1.y), v2.y), 0);
	maxX = min(max(max(v0.x, v1.x), v2.x), winWidth);
	maxY = min(max(max(v0.y, v1.y), v2.y), winHeight);

	EdgeEquation total(v0, v1, v2);

	Vec2i pt(minX, minY);
	EdgeEquationSet tempY(v0, v1, v2, pt);
	EdgeEquationSet tempX;
	for (int y = minY; y < maxY; ++y)
	{
		tempX = tempY;
		for (int x = minX; x < maxX; ++x)
		{
			if (tempX.evaluate())
			{
				//注意取点的对边面积
				float a0 = float(tempX.e0.value) / total.value;
				float a1 = float(tempX.e1.value) / total.value;
				float a2 = float(tempX.e2.value) / total.value;

				Vec3f col = col0 * a0 + col1 * a1 + col2 * a2;
				drawColorPixel(x, y, col);
			}
			tempX.incrementX();
		}
		tempY.incrementY();
	}
}

void RenderDevice::drawColorPolygon(int nVerts, Vec2i* verts, Vec3f* colors)
{
	if (nVerts < 3)
		return;

	Vec2i p0, p1, p2;
	p0 = verts[0];
	for (int k = 2; k < nVerts; ++k)
	{
		p1 = verts[k - 1];
		p2 = verts[k];

		drawTrangleByHalfSpace(p0, p1, p2, colors[0], colors[k-1], colors[k]);
	}
}
#pragma endregion

void RenderDevice::drawViewportArea(const Vec2i& winMin, const Vec2i& winMax)
{
	drawLineBres(winMin.x, winMin.y, winMax.x, winMin.y);
	drawLineBres(winMin.x, winMin.y, winMin.x, winMax.y);
	drawLineBres(winMin.x, winMax.y, winMax.x, winMax.y);
	drawLineBres(winMax.x, winMin.y, winMax.x, winMax.y);
}
