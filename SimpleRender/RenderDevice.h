#pragma once

#include "stdfx.h"
#include "MathUtils.h"
#include "Camera.h"
#include "RenderContext.h"
#include "Buffer.h"

class RenderDevice
{
private:
	bool m_inited = false;

	Vec2i screenCenter;
	int winWidth, winHeight;
	float depthBuffer[160000];

	HDC screenHDC = NULL;

	std::shared_ptr<RenderContext> m_context3D;
	std::shared_ptr<Buffer> m_vertexBuffer;
	std::shared_ptr<Buffer> m_indexBuffer;
	std::shared_ptr<Camera> m_camera;

	Viewport m_viewPort;
	float m_angle = 0;
	float m_radius = 100;
	Vec3f m_origin;

public:
	RenderDevice();
	~RenderDevice();

	bool isInited();
	void initDevice(HDC& hdc, int width, int height);
	void releaseDevice();
	void clear();
	void update();
	void drawcall();

	void initMeshInfo();

	std::shared_ptr<Buffer> createBuffer(const BufferDesc& desc);

#pragma region Raster Function
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
	void drawTrangleBorder(int nVerts, Vec2i* verts);
	void drawPolygon(int nVerts, Vec2i* verts);
	
	void drawColorPixel(int x, int y, const Vec3f& col);
	void drawColorLine(int x0, int y0, int xEnd, int yEnd, const Vec3f& col0, const Vec3f& colEnd);
	void drawTrangleByHalfSpace(const Vec2i& v0, const Vec2i& v1, const Vec2i& v2, const Vec3f& col0, const Vec3f& col1, const Vec3f& col2);
	void drawColorPolygon(int nVerts, Vec2i* verts, Vec3f* colors);
#pragma endregion

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

	void drawClipArea(const Vec2i& winMin, const Vec2i& winMax);
	Vec2i NDCToScreenSpace(const Vec4f pos, const int& width, const int& height, const Vec2i& center);
	void rasterizeTrangle(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2, const Vec3f& col0, const Vec3f& col1, const Vec3f& col2);
	void matrixDisplay3D();
};

