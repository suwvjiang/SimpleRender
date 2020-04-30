#pragma once

#include "stdfx.h"
#include "MathUtils.h"

typedef enum Boundary
{
	Left,
	Right,
	Bottom,
	Top,
};

class RenderDevice
{
private:
	int winWidth;
	int winHeight;
	HDC screenHDC;

	float angle = 0;

public:
	RenderDevice();
	~RenderDevice();

	void initRender(HDC hdc, int width, int height);
	void releaseRender();
	void clear();
	void drawcall();

	void drawPixel(int x, int y);
	void drawLineDDA(int x0, int y0, int xEnd, int yEnd);
	void drawLineBres(int x0, int y0, int xEnd, int yEnd);

	void circlePlotPoints(int x, int y, Vec2i& point);
	void circleMidPoint(int x, int y, int radius);
	void ellipsePlotPoints(int xCenter, int yCenter, int x, int y);
	void ellipseMidPoint(int xCenter, int yCenter, int xRadius, int yRadius);
	void midPointGravity(int vx0, int vy0, int x0, int y0);

	void drawTopFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2);
	void drawBottomFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2);
	void drawTrangle(int x0, int y0, int x1, int y1, int x2, int y2);

	void drawTrangleByHalfSpace(Vec2i& v0, Vec2i& v1, Vec2i& v2, const Vec3f& col0, const Vec3f& col1, const Vec3f& col2);

	void drawColorPixel(int x, int y, const Vec3f& col);
	void drawColorLine(int x0, int y0, int xEnd, int yEnd, const Vec3f& col0, const Vec3f& colEnd);

	void drawClipArea(const Vec2i& winMin, const Vec2i& winMax);
	void drawTrangleBorder(int nVerts, Vec4f* verts, Vec3f* colors);

	void matrixDisplay3D();

#pragma region 2DClip
	void lineClipCohSuth(const Vec2i& winMin, const Vec2i& winMax, Vec2i& p1, Vec2i& p2);
	void lineClipLiangBarsk(const Vec2i& winMin, const Vec2i& winMax, Vec2i& p1, Vec2i& p2);
	void testLineClip();

	int checkPtInside(Vec2i& p, Boundary edeg, const Vec2i& winMin, const Vec2i& winMax);
	int checkCross(Vec2i& p1, Vec2i& p2, Boundary edeg, const Vec2i& winMin, const Vec2i& winMax);
	Vec2i intersect(Vec2i& p1, Vec2i& p2, Boundary edeg, const Vec2i& winMin, const Vec2i& winMax);
	void clipPoint(Vec2i& p, Boundary edeg, const Vec2i& winMin, const Vec2i& winMax, Vec2i* pOut, int* cnt, Vec2i* first[], Vec2i* last);
	void closeClip(const Vec2i& winMin, const Vec2i& winMax, Vec2i* pOut, int* cnt, Vec2i* first[], Vec2i* last);
	int polygonClipSuthHodg(const Vec2i& winMin, const Vec2i& winMax, int n, Vec2i* pIn, Vec2i* pOut);
	void testPolygonClip();
#pragma endregion
};

