#pragma once

#include "stdfx.h"
#include "screenPt.h"
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

	void drawColorPixel(int x, int y, const color& col);
	void drawColorLine(int x0, int y0, int xEnd, int yEnd, const color& col0, const color& colEnd);

	void circlePlotPoints(int x, int y, screenPt point);
	void circleMidPoint(int x, int y, int radius);
	void ellipsePlotPoints(int xCenter, int yCenter, int x, int y);
	void ellipseMidPoint(int xCenter, int yCenter, int xRadius, int yRadius);
	void midPointGravity(int vx0, int vy0, int x0, int y0);

	void drawTopFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2);
	void drawBottomFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2);
	void drawTrangle(int x0, int y0, int x1, int y1, int x2, int y2);

	void drawClipArea(const vector2D& winMin, const vector2D& winMax);
	void drawTrangleBorder(int nVerts, vector3D* verts, color* colors);

	void matrixDisplay3D();

#pragma region 2DClip
	void lineClipCohSuth(const vector2D& winMin, const vector2D& winMax, vector2D& p1, vector2D& p2);
	void lineClipLiangBarsk(const vector2D& winMin, const vector2D& winMax, vector2D& p1, vector2D& p2);
	void testLineClip();

	int checkPtInside(vector2D& p, Boundary edeg, const vector2D& winMin, const vector2D& winMax);
	int checkCross(vector2D& p1, vector2D& p2, Boundary edeg, const vector2D& winMin, const vector2D& winMax);
	vector2D intersect(vector2D& p1, vector2D& p2, Boundary edeg, const vector2D& winMin, const vector2D& winMax);
	void clipPoint(vector2D& p, Boundary edeg, const vector2D& winMin, const vector2D& winMax, vector2D* pOut, int* cnt, vector2D* first[], vector2D* last);
	void closeClip(const vector2D& winMin, const vector2D& winMax, vector2D* pOut, int* cnt, vector2D* first[], vector2D* last);
	int polygonClipSuthHodg(const vector2D& winMin, const vector2D& winMax, int n, vector2D* pIn, vector2D* pOut);
	void testPolygonClip();
#pragma endregion
};

