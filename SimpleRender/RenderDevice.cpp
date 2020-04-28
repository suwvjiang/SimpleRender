#include "RenderDevice.h"

RenderDevice::RenderDevice()
{

}

RenderDevice::~RenderDevice()
{

}

void RenderDevice::initRender(HDC hdc, int width, int height)
{
	screenHDC = hdc;
	winWidth = width;
	winHeight = height;
}

void RenderDevice::releaseRender()
{

}

void RenderDevice::clear()
{
	BitBlt(screenHDC, 0, 0, winWidth, winHeight, NULL, NULL, NULL, BLACKNESS);

}

void RenderDevice::drawcall()
{
	//drawLineBres(100, 200, 300, 400);
	//circleMidPoint(200, 200, 100);
	//ellipseMidPoint(200, 200, 100, 50);
	//midPointGravity(200, 800, 40, 40);
	//testLineClip();

	matrixDisplay3D();
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
	stepX = signPlus(xEnd - x0);
	stepY = signPlus(yEnd - y0);

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

void RenderDevice::drawColorPixel(int x, int y, const color& col)
{
	SetPixel(screenHDC, x, y, RGB(255* col.r, 255*col.g, 255*col.b));
}

void RenderDevice::drawColorLine(int x0, int y0, int xEnd, int yEnd, const color& col0, const color& colEnd)
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
		drawColorPixel(round(x), round(y), lerpColor(col0, colEnd, float(k)/steps));
	}
}

//中点法画圆
void RenderDevice::circlePlotPoints(int x, int y, screenPt point)
{
	drawPixel(x + point.getX(), y + point.getY());
	drawPixel(x + point.getX(), y - point.getY());
	drawPixel(x - point.getX(), y + point.getY());
	drawPixel(x - point.getX(), y - point.getY());
	drawPixel(x + point.getY(), y + point.getX());
	drawPixel(x + point.getY(), y - point.getX());
	drawPixel(x - point.getY(), y + point.getX());
	drawPixel(x - point.getY(), y - point.getX());
}
void RenderDevice::circleMidPoint(int x, int y, int radius)
{
	screenPt circPt;
	int p = 1 - radius;
	circPt.setCoords(0, radius);
	circlePlotPoints(x, y, circPt);

	while (circPt.getX() < circPt.getY())
	{
		circPt.incrementX();

		if (p < 0)
		{
			p += 2 * circPt.getX() + 1;
		}
		else
		{
			circPt.decrementY();
			p += 2 * circPt.getX() - 2 * circPt.getY() + 1;
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
#pragma endregion

#pragma region 2D Clip
//线段裁剪
void RenderDevice::lineClipCohSuth(const vector2D& winMin, const vector2D& winMax, vector2D& p1, vector2D& p2)
{
	int code1, code2;
	int done = false, plotLine = false;

	float m;

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
					swapPts(&p1, &p2);
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
		drawLineBres(round(p1.x), round(p1.y), round(p2.x), round(p2.y));
}

//线段裁剪-梁友栋
void RenderDevice::lineClipLiangBarsk(const vector2D& winMin, const vector2D& winMax, vector2D& p1, vector2D& p2)
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
						p2.setCoords(p1.x + uOut * dx, p1.y + uOut * dy);
					if (uIn > .0)
						p1.setCoords(p1.x + uIn * dx, p1.y + uIn * dy);

					drawLineBres(round(p1.x), round(p1.y), round(p2.x), round(p2.y));
				}
			}
		}
	}
}

void RenderDevice::testLineClip()
{
	vector2D winMin, winMax;
	winMin.x = 0; winMin.y = 0;
	winMax.x = 100; winMax.y = 100;
	drawClipArea(winMin, winMax);

	vector2D p1, p2;
	p1.x = -50; p1.y = -50;
	p2.x = 120, p2.y = 120;
	//lineClipCohSuth(winMin, winMax, p1, p2);
	lineClipLiangBarsk(winMin, winMax, p1, p2);
}

const int nClip = 4;

//是否在边界区域内
int RenderDevice::checkPtInside(vector2D& p, Boundary edeg, const vector2D& winMin, const vector2D& winMax)
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
int RenderDevice::checkCross(vector2D& p1, vector2D& p2, Boundary edeg, const vector2D& winMin, const vector2D& winMax)
{
	if (checkPtInside(p1, edeg, winMin, winMax) == checkPtInside(p2, edeg, winMin, winMax))
		return false;
	else
		return true;
}

//获取边界交点
vector2D RenderDevice::intersect(vector2D& p1, vector2D& p2, Boundary edeg, const vector2D& winMin, const vector2D& winMax)
{
	vector2D iPt;
	float m;

	if (p1.x != p2.x) m = (p2.y - p1.y) / (p2.x - p1.x);
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

void RenderDevice::clipPoint(vector2D& p, Boundary edeg, const vector2D& winMin, const vector2D& winMax, vector2D* pOut, int* cnt, vector2D* first[], vector2D* last)
{
	vector2D iPt;
	if (!first[edeg])
	{
		first[edeg] = new vector2D();
		first[edeg]->setCoords(p.x, p.y);
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

void RenderDevice::closeClip(const vector2D& winMin, const vector2D& winMax, vector2D* pOut, int* cnt, vector2D* first[], vector2D* last)
{
	vector2D pt;
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

int RenderDevice::polygonClipSuthHodg(const vector2D& winMin, const vector2D& winMax, int n, vector2D* pIn, vector2D* pOut)
{
	vector2D* first[nClip] = { 0, 0, 0, 0 };
	vector2D last[nClip];
	int k, cnt = 0;

	for (k = 0; k < n; ++k)
		clipPoint(pIn[k], Left, winMin, winMax, pOut, &cnt, first, last);

	closeClip(winMin, winMax, pOut, &cnt, first, last);
	return cnt;
}

void RenderDevice::testPolygonClip()
{
	vector2D winMin, winMax;
	winMin.x = 0; winMin.y = 0;
	winMax.x = 100; winMax.y = 100;
	drawClipArea(winMin, winMax);

	int nVerts = 3;
	vector2D p1, p2, p3;
	p1.setCoords(-25.0, 10.0);
	p2.setCoords(125.0, 10.0);
	p3.setCoords(50.0, 125.0);
	vector2D verts[3] = { p1, p2, p3 };

	//glColor3f(1.0f, 0.0f, 0.0f);
	//triangle2D(verts, nVerts);

	vector2D outVerts[6];
	int outCnt = polygonClipSuthHodg(winMin, winMax, nVerts, verts, outVerts);
	//glColor3f(0.0f, 1.0f, 0.0f);
	//triangle2D(outVerts, outCnt);
}

#pragma endregion

void RenderDevice::drawClipArea(const vector2D& winMin, const vector2D& winMax)
{
	drawLineBres(round(winMin.x), round(winMin.y), round(winMax.x), round(winMin.y));
	drawLineBres(round(winMin.x), round(winMin.y), round(winMin.x), round(winMax.y));
	drawLineBres(round(winMin.x), round(winMax.y), round(winMax.x), round(winMax.y));
	drawLineBres(round(winMax.x), round(winMin.y), round(winMax.x), round(winMax.y));
}

void RenderDevice::drawTrangleBorder(int nVerts, vector3D* verts, color* colors)
{
	for (int k = 1; k < nVerts; ++k)
	{
		if (k == 1)
		{
			drawColorLine(round(verts[nVerts - 1].x),	round(verts[nVerts - 1].y),		round(verts[1].x), round(verts[1].y), colors[nVerts - 1], colors[1] );
			drawColorLine(round(verts[0].x),				round(verts[0].y),				round(verts[1].x), round(verts[1].y), colors[0], colors[1]);
		}
		else
		{
			drawColorLine(round(verts[0].x), round(verts[0].y), round(verts[k].x), round(verts[k].y), colors[0], colors[k]);
			drawColorLine(round(verts[k - 1].x), round(verts[k - 1].y), round(verts[k].x), round(verts[k].y), colors[k - 1], colors[k]);
		}
	}
}

const static float Rotate_Speed = 1800.0;
void RenderDevice::matrixDisplay3D()
{
	drawLineBres(0, 300, 800, 300);
	drawLineBres(400, 0, 400, 600);

	angle += PI / Rotate_Speed;

	int nVerts = 5;
	vector3D p1, p2, p3, p4, p5;
	p1.setCoords(0, 100.0, 0);
	p2.setCoords(100, -50.0, 100);
	p3.setCoords(100.0, -50.0, -100);
	p4.setCoords(-100.0, -50.0, -100);
	p5.setCoords(-100.0, -50.0, 100);
	vector3D verts[5] = { p1, p2, p3, p4, p5 };

	color c1, c2, c3, c4, c5;
	c1.setColor(1, 0, 0);
	c2.setColor(0, 1, 0);
	c3.setColor(1, 1, 0);
	c4.setColor(0, 0, 1);
	c5.setColor(1, 0, 1);
	color colors[5] = { c1, c2, c3, c4, c5 };

	vector3D viewOrigin;
	viewOrigin.setCoords(0, 0, 0);
	vector3D viewLookAt;
	viewLookAt.setCoords(50, 0, 0);

	Matrix4x4 matComposite;
	matrix4x4SetIdentity(matComposite);
	rotate3D(viewOrigin, viewLookAt, PI, matComposite);

	//identity Camera Matrix
	vector3D cameraPos, cameraLookAt, cameraUpDir;
	cameraPos.setCoords(200 * sin(angle), 0, 200 * cos(angle));
	cameraLookAt.setCoords(0, 0, 0);
	cameraUpDir.setCoords(0, 1, 0);
	Matrix4x4 cameraMatrix;
	generateCameraModel(cameraMatrix, cameraPos, cameraLookAt, cameraUpDir);
	matrix4x4PreMultiply(cameraMatrix, matComposite);

	//identity Project Matrix
	float fov = PI * 3 / 9, aspect = 4 / 3;
	float zNear = -100, zFar = -300;
	Matrix4x4 projectMatrix;
	generateProjectModel(projectMatrix, fov, aspect, zNear, zFar);
	matrix4x4PreMultiply(projectMatrix, matComposite);
	//mvp transform
	transformVerts3D(nVerts, verts, matComposite);
	//homoneous transformation
	int k;
	for (k = 0; k < nVerts; ++k)
	{
		verts[k].homogeneous();
	}

	//identity Screen Matrix
	vector3D screenCenter;
	screenCenter.setCoords(400, 300, 0);
	Matrix4x4 screenMatrix;
	generateScreenModel(screenMatrix, screenCenter, 800, 600);

	matrix4x4SetIdentity(matComposite);
	matrix4x4PreMultiply(screenMatrix, matComposite);
	transformVerts3D(nVerts, verts, matComposite);

	drawTrangleBorder(nVerts, verts, colors);
}