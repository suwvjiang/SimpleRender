#include "RenderDevice.h"
#include "Rasterizer.h"

ShaderStruct::ConstBuffer ShaderStruct::constBuffer;

RenderDevice::RenderDevice()
{
}

RenderDevice::~RenderDevice()
{
	m_context3D = nullptr;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
}

bool RenderDevice::isInited()
{
	return m_inited;
}

void RenderDevice::initDevice(HDC& hdc, int width, int height)
{
	screenHDC = hdc;

	winWidth = width;
	winHeight = height; 
	screenCenter.x = winWidth >> 1;
	screenCenter.y = winHeight >> 1;

	int cnt = winWidth * winHeight;
	for (int i = 0; i < cnt; ++i)
		depthBuffer[i] = 1.0f;

	m_viewPort.width = width;
	m_viewPort.height = height;
	m_viewPort.originX = width >> 1;
	m_viewPort.originY = height >> 1;

	m_context3D = std::make_shared<RenderContext>();
	m_context3D->setViewport(m_viewPort);

	//identity Project Matrix
	float fov = PI * 3 / 9, aspect = 1.0;
	float zNear = -1, zFar = -1000;
	m_camera.setViewInfo(fov, aspect, zNear, zFar);

	m_origin.x = width >> 1;
	m_origin.y = (height >> 1);
	m_origin.z = 0;
	m_camera.setFocusPos(m_origin);
	ShaderStruct::constBuffer.world = Matrix4x4TranslationFromVector<float>(m_origin);

	initMeshInfo();
}

void RenderDevice::releaseDevice()
{

}

void RenderDevice::clear()
{
	BitBlt(screenHDC, 0, 0, m_viewPort.width, m_viewPort.height, NULL, NULL, NULL, BLACKNESS);
}

const static float Rotate_Speed = 360.0;
//更新相机信息
void RenderDevice::update()
{
	m_angle += PI / Rotate_Speed;
	Vec3f pos(m_radius * sin(m_angle), 0, m_radius * cos(m_angle));
	pos += m_origin;
	m_camera.setPos(pos);
	m_camera.update();

	ShaderStruct::constBuffer.view_proj = m_camera.viewProjMatrix();
}
//绘制
void RenderDevice::drawcall()
{
	//drawLineBres(100, 200, 300, 400);
	//circleMidPoint(200, 200, 100);
	//ellipseMidPoint(200, 200, 100, 50);
	//midPointGravity(200, 800, 40, 40);
	//testLineClip();
	//testPolygonClip();
	//drawTrangleByHalfSpace(Vec2i(100, 100), Vec2i(300, 100), Vec2i(200, 300), Color_Red, Color_Green, Color_Blue);
	//matrixDisplay3D();
	
	m_context3D->clearDepthBuffer();
	m_context3D->setIndexBuffer(m_indexBuffer);
	m_context3D->setVertexBuffer(m_vertexBuffer);

	m_context3D->setVertexShader(ShaderStruct::VS);
	m_context3D->setFragmentShader(ShaderStruct::FS);

	m_context3D->draw(screenHDC);
}

void RenderDevice::initMeshInfo()
{
	float w2 = 75;
	float h2 = 75;
	float d2 = 75;

	//Vertex
	Vertex v[24];
	v[0] = Vertex(-w2, -h2, -d2, 1, 0, 0);
	v[1] = Vertex(-w2, +h2, -d2, 1, 1, 0);
	v[2] = Vertex(+w2, +h2, -d2, 0, 1, 1);
	v[3] = Vertex(+w2, -h2, -d2, 1, 0, 1);

	v[4] = Vertex(-w2, -h2, +d2, 0, 1, 1);
	v[5] = Vertex(+w2, -h2, +d2, 0, 1, 0);
	v[6] = Vertex(+w2, +h2, +d2, 1, 0, 0);
	v[7] = Vertex(-w2, +h2, +d2, 0, 0, 1);

	v[8] = Vertex(-w2, +h2, -d2, 1, 1, 0);
	v[9] = Vertex(-w2, +h2, +d2, 0, 0, 1);
	v[10] = Vertex(+w2, +h2, +d2, 1, 0, 0);
	v[11] = Vertex(+w2, +h2, -d2, 0, 1, 1);

	v[12] = Vertex(-w2, -h2, -d2, 1, 0, 0);
	v[13] = Vertex(+w2, -h2, -d2, 1, 0, 1);
	v[14] = Vertex(+w2, -h2, +d2, 0, 1, 0);
	v[15] = Vertex(-w2, -h2, +d2, 0, 1, 1);

	v[16] = Vertex(-w2, -h2, +d2, 0, 1, 1);
	v[17] = Vertex(-w2, +h2, +d2, 0, 0, 1);
	v[18] = Vertex(-w2, +h2, -d2, 1, 1, 0);
	v[19] = Vertex(-w2, -h2, -d2, 1, 0, 0);

	v[20] = Vertex(+w2, -h2, -d2, 1, 0, 1);
	v[21] = Vertex(+w2, +h2, -d2, 0, 1, 1);
	v[22] = Vertex(+w2, +h2, +d2, 1, 0, 0);
	v[23] = Vertex(+w2, -h2, +d2, 0, 1, 0);
	
	//Index
	//逆时针
	size_t i[36];
	i[0] = 0; i[1] = 2; i[2] = 1;
	i[3] = 0; i[4] = 3; i[5] = 2;

	i[6] = 4; i[7] = 6; i[8] = 5;
	i[9] = 4; i[10] = 7; i[11] = 6;

	i[12] = 8; i[13] = 10; i[14] = 9;
	i[15] = 8; i[16] = 11; i[17] = 10;

	i[18] = 12; i[19] = 14; i[20] = 13;
	i[21] = 12; i[22] = 15; i[23] = 14;

	i[24] = 16; i[25] = 18; i[26] = 17;
	i[27] = 16; i[28] = 19; i[29] = 18;

	i[30] = 20; i[31] = 22; i[32] = 21;
	i[33] = 20; i[34] = 23; i[35] = 22;

	BufferDesc vertexDesc;
	vertexDesc.stride = sizeof(Vertex);
	vertexDesc.numOfEle = 24;
	vertexDesc.data = v;
	vertexDesc.bufferSize = sizeof(Vertex) * 24;

	m_vertexBuffer = createBuffer(vertexDesc);

	BufferDesc indexDesc;
	indexDesc.stride = sizeof(size_t);
	indexDesc.numOfEle = 36;
	indexDesc.data = i;
	indexDesc.bufferSize = sizeof(size_t) * 36;

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
	SetPixel(screenHDC, x, y, RGB(255, 255, 255));
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
	SetPixel(screenHDC, x, y, RGB(255 * col.r, 255 * col.g, 255 * col.b));
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

#pragma region 2D Clip
//线段裁剪
void RenderDevice::lineClipCohSuth(const Vec2i& winMin, const Vec2i& winMax, Vec2i& p1, Vec2i& p2)
{
	int code1, code2;
	int done = false, plotLine = false;

	float m = 1;

	while (!done)
	{
		code1 = encode(p1, winMin, winMax);
		code2 = encode(p2, winMin, winMax);

		if (accept(code1, code2))
		{
			done = true;
			plotLine = true;
		}
		else
		{
			if (reject(code1, code2))//同侧剔除
			{
				done = true;
			}
			else
			{
				if (inside(code1))
				{
					Swap(p1, p2);
					std::swap(code1, code2);
				}

				if (p2.x != p1.x)
					m = (p2.y - p1.y) / (p2.x - p1.x);
				if (code1 & winLeftBitCode)
				{
					p1.y += (winMin.x - p1.x) * m;
					p1.x = winMin.x;
				}
				else
				{
					if (code1 & winRightBitCode)
					{
						p1.y += (winMax.x - p1.x) * m;
						p1.x = winMax.x;
					}
					else
					{
						if (code1 & winBottomBitCode)
						{
							p1.x += (winMin.y - p1.y) / m;
							p1.y = winMin.y;
						}
						else
						{
							if (code1 & winTopBitCode)
							{
								p1.x += (winMax.y - p1.y) / m;
								p1.y = winMax.y;
							}
						}
					}
				}
			}
		}
	}

	if (plotLine)
		drawLineBres(p1.x, p1.y, p2.x, p2.y);
}

//线段裁剪-梁友栋
void RenderDevice::lineClipLiangBarsk(const Vec2i& winMin, const Vec2i& winMax, Vec2i& p1, Vec2i& p2)
{
	float uIn = 0, uOut = 1, dx = p2.x - p1.x, dy;
	if (clipTest(-dx, p1.x - winMin.x, &uIn, &uOut))
	{
		if (clipTest(dx, winMax.x - p1.x, &uIn, &uOut))
		{
			dy = p2.y - p1.y;
			if (clipTest(-dy, p1.y - winMin.y, &uIn, &uOut))
			{
				if (clipTest(dy, winMax.y - p1.y, &uIn, &uOut))
				{
					if (uOut < 1.0)
					{
						p2.x = round(p1.x + uOut * dx);
						p2.y = round(p1.y + uOut * dy);
					}
					if (uIn > .0)
					{
						p1.x = round(p1.x + uIn * dx);
						p1.y = round(p1.y + uIn * dy);
					}

					drawLineBres(p1.x, p1.y, p2.x, p2.y);
				}
			}
		}
	}
}

void RenderDevice::testLineClip()
{
	Vec2i winMin, winMax;
	winMin.x = 0; winMin.y = 0;
	winMax.x = 100; winMax.y = 100;
	drawClipArea(winMin, winMax);

	Vec2i p1, p2;
	p1.x = -50; p1.y = -50;
	p2.x = 120, p2.y = 120;
	//lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
}

const int nClip = 4;

//是否在边界区域内
int RenderDevice::checkPtInside(Vec2i& p, Boundary edeg, const Vec2i& winMin, const Vec2i& winMax)
{
	switch (edeg)
	{
	case Left:
		if (p.x < winMin.x)return (false);
		break;
	case Right:
		if (p.x > winMax.x)return (false);
		break;
	case Bottom:
		if (p.y < winMin.y)return (false);
		break;
	case Top:
		if (p.y > winMax.y)return (false);
		break;
	}
	return (true);
}

//是否穿过边界
int RenderDevice::checkCross(Vec2i& p1, Vec2i& p2, Boundary edeg, const Vec2i& winMin, const Vec2i& winMax)
{
	if (checkPtInside(p1, edeg, winMin, winMax) == checkPtInside(p2, edeg, winMin, winMax))
		return false;
	else
		return true;
}

//获取边界交点
Vec2i RenderDevice::intersect(Vec2i& p1, Vec2i& p2, Boundary edeg, const Vec2i& winMin, const Vec2i& winMax)
{
	Vec2i iPt;
	float m = 1.0f;

	if (p1.x != p2.x) m = float(p2.y - p1.y) / (p2.x - p1.x);
	switch (edeg)
	{
	case Left:
		iPt.x = winMin.x;
		iPt.y = p2.y + (winMin.x - p2.x) * m;
		break;
	case Right:
		iPt.x = winMax.x;
		iPt.y = p2.y + (winMax.x - p2.x) * m;
		break;
	case Bottom:
		iPt.y = winMin.y;
		if (p1.x != p2.x) iPt.x = p2.x + (winMin.y - p2.y) / m;
		else iPt.x = p2.x;
		break;
	case Top:
		iPt.y = winMax.y;
		if (p1.x != p2.x)iPt.x = p2.x + (winMax.y - p2.y) / m;
		else iPt.x = p2.x;
		break;
	default:
		break;
	}
	return iPt;
}

void RenderDevice::clipPoint(Vec2i& p, Boundary edeg, const Vec2i& winMin, const Vec2i& winMax, Vec2i* pOut, int* cnt, Vec2i* first[], Vec2i* last)
{
	Vec2i iPt;
	if (!first[edeg])
	{
		first[edeg] = new Vec2i(p.x, p.y);
	}
	else
	{
		if (checkCross(p, last[edeg], edeg, winMin, winMax))//与边界相交
		{
			iPt = intersect(p, last[edeg], edeg, winMin, winMax);
			if (edeg < Top)
				clipPoint(iPt, Boundary(edeg + 1), winMin, winMax, pOut, cnt, first, last);
			else
			{
				pOut[*cnt] = iPt;
				(*cnt)++;
			}
		}
	}

	last[edeg] = p;

	if (checkPtInside(p, edeg, winMin, winMax))//边界以内的点
	{
		if (edeg < Top)
			clipPoint(p, Boundary(edeg + 1), winMin, winMax, pOut, cnt, first, last);
		else
		{
			pOut[*cnt] = p;
			(*cnt)++;
		}
	}
}

void RenderDevice::closeClip(const Vec2i& winMin, const Vec2i& winMax, Vec2i* pOut, int* cnt, Vec2i* first[], Vec2i* last)
{
	Vec2i pt;
	int edge;
	for (edge = Left; edge <= Top; ++edge)
	{
		if (checkCross(last[edge], *first[edge], Boundary(edge), winMin, winMax))
		{
			pt = intersect(last[edge], *first[edge], Boundary(edge), winMin, winMax);
			if (edge < Top)
				clipPoint(pt, Boundary(edge + 1), winMin, winMax, pOut, cnt, first, last);
			else
			{
				pOut[*cnt] = pt;
				(*cnt)++;
			}
		}
	}
}

int RenderDevice::polygonClipSuthHodg(const Vec2i& winMin, const Vec2i& winMax, int n, Vec2i* pIn, Vec2i* pOut)
{
	Vec2i* first[nClip] = { 0, 0, 0, 0 };
	Vec2i last[nClip];
	int k, cnt = 0;

	for (k = 0; k < n; ++k)
		clipPoint(pIn[k], Left, winMin, winMax, pOut, &cnt, first, last);

	closeClip(winMin, winMax, pOut, &cnt, first, last);
	return cnt;
}

void RenderDevice::testPolygonClip()
{
	Vec2i winMin, winMax;
	winMin.x = 300; winMin.y = 200;
	winMax.x = 500; winMax.y = 400;
	drawClipArea(winMin, winMax);

	int nVerts = 3;
	Vec2i p1(250.0, 250.0), p2(550.0, 250.0), p3(400.0, 450.0);
	Vec2i verts[3] = { p1, p2, p3 };

	drawTrangleBorder(nVerts, verts);

	Vec2i outVerts[6];
	int outCnt = polygonClipSuthHodg(winMin, winMax, nVerts, verts, outVerts);
	drawPolygon(outCnt, outVerts);
}

#pragma endregion

void RenderDevice::drawClipArea(const Vec2i& winMin, const Vec2i& winMax)
{
	drawLineBres(winMin.x, winMin.y, winMax.x, winMin.y);
	drawLineBres(winMin.x, winMin.y, winMin.x, winMax.y);
	drawLineBres(winMin.x, winMax.y, winMax.x, winMax.y);
	drawLineBres(winMax.x, winMin.y, winMax.x, winMax.y);
}

//转成屏幕坐标
Vec2i RenderDevice::NDCToScreenSpace(const Vec4f pos, const int& width, const int& height, const Vec2i& center)
{
	Vec2i screenPos;
	screenPos.x = pos.x / pos.w * width + center.x;
	screenPos.y = pos.y / pos.w * height + center.y;
	return screenPos;
}

//
void RenderDevice::rasterizeTrangle(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2, const Vec3f& col0, const Vec3f& col1, const Vec3f& col2)
{
	float inv_camera_z[3];
	inv_camera_z[0] = 1 / v0.w;
	inv_camera_z[1] = 1 / v1.w;
	inv_camera_z[2] = 1 / v2.w;

	Vec2i p0, p1, p2, p;
	p0 = NDCToScreenSpace(v0, winWidth, winHeight, screenCenter);
	p1 = NDCToScreenSpace(v1, winWidth, winHeight, screenCenter);
	p2 = NDCToScreenSpace(v2, winWidth, winHeight, screenCenter);

	drawLineBres(p0.x, p0.y, p1.x, p1.y);
	drawLineBres(p0.x, p0.y, p2.x, p2.y);
	drawLineBres(p1.x, p1.y, p2.x, p2.y);

	int minX, minY, maxX, maxY;
	minX = max(min(min(p0.x, p1.x), p2.x), 0);
	minY = max(min(min(p0.y, p1.y), p2.y), 0);
	maxX = min(max(max(p0.x, p1.x), p2.x), winWidth);
	maxY = min(max(max(p0.y, p1.y), p2.y), winHeight);
	if (minX > maxX)
		return;
	if (minY > maxY)
		return;
	
	EdgeEquation area(p0, p1, p2);
	p.x = minX;
	p.y = minY;
	EdgeEquationSet tempY(p0, p1, p2, p);
	EdgeEquationSet tempX;

	int i, j, index;
	float z0 = v0.z * inv_camera_z[0], z1 = v1.z * inv_camera_z[1], z2 = v2.z* inv_camera_z[2];
	z0 = z0 * 0.5 + 0.5;
	z1 = z1 * 0.5 + 0.5;
	z2 = z2 * 0.5 + 0.5;
	float param0, param1, param2, depth;
	for (j = minY; j < maxY; ++j)
	{
		tempX = tempY;
		for (i = minX; i < maxX; ++i)
		{
			index = j * winWidth + i;
			if (tempX.evaluate())
			{
				param0 = float(tempX.e0.value) / area.value;
				param1 = float(tempX.e1.value) / area.value;
				param2 = float(tempX.e2.value) / area.value;

				depth = 1/(param0 / z0 + param1 / z1 + param2 / z2);
				Vec3f col = col0 * param0 + col1 * param1 + col2 * param2;
				if (depth < depthBuffer[index])
				{
					depthBuffer[index] = depth;
					drawColorPixel(i, j, col);
				}
				else
					std::cout << "ztest failed" << std::endl;
			}

			tempX.incrementX();
		}

		tempY.incrementY();
	}

}

void RenderDevice::matrixDisplay3D()
{
	int cnt = winWidth * winHeight;
	for (int i = 0; i < cnt; ++i)
		depthBuffer[i] = 1.0f;

	drawLineBres(0, 300, 800, 300);
	drawLineBres(400, 0, 400, 600);

	m_angle += PI / Rotate_Speed;

	int nVerts = 5;
	Vec4f p1(0, -50.0, 0), p2(0, 50.0, 100), p3(100.0, 50.0, 0), p4(.0, 50.0, -100.0), p5(-100.0, 50.0, 0);
	Vec4f verts[5] = { p1, p2, p3, p4, p5 };

	Vec3f c1(1, 0, 0), c2(0, 1, 0), c3(1, 1, 0), c4(0, 0, 1), c5(1, 0, 1);
	Vec3f colors[5] = { c1, c2, c3, c4, c5 };

	Vec3f origin(400, 300, 0);
	Matrix4x4f modelMatrix = Matrix4x4TranslationFromVector<float>(origin);
	TransformVectors(nVerts, verts, modelMatrix);

	//identity Camera Matrix
	Vec3f cameraPos(300 * sin(m_angle), 0, 300 * cos(m_angle));
	cameraPos += origin;
	m_camera.setPos(cameraPos);
	m_camera.setFocusPos(origin);
	m_camera.update();
	
	TransformVectors(nVerts, verts, m_camera.viewProjMatrix());

	//到此为止，为Clipp Space
	//唯有齐次以后，方为NDC Space
	int k;
	for (k = 2; k < nVerts; ++k)
	{
		rasterizeTrangle(verts[0], verts[k], verts[k - 1], colors[0], colors[k], colors[k - 1]);
	}
	rasterizeTrangle(verts[0], verts[1], verts[nVerts - 1], colors[0], colors[1], colors[nVerts - 1]);
}