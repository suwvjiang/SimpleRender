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
	bool m_rotateEnable = false;
	bool m_rotateCamera = false;

	int winWidth, winHeight;

	BYTE* m_fragmentBuff = NULL;

	std::shared_ptr<RenderContext> m_context3D;
	std::shared_ptr<Buffer> m_vertexBuffer;
	std::shared_ptr<Buffer> m_indexBuffer;
	std::shared_ptr<Camera> m_camera;

	Viewport m_viewPort;
	float m_angle = 0;
	float m_cameraAngle = 0;
	float m_radius = 3;
	float m_scale = 1;
	Vec3f m_origin;

public:
	RenderDevice();
	~RenderDevice();

	BYTE* getFragmentBuffer() 
	{
		return m_fragmentBuff;
	}

	bool isInited();
	void initDevice(int width, int height);
	void releaseDevice();
	void clear();
	void update();
	void drawcall();

	void initMeshInfo();

	void switchRotateEnable() { m_rotateEnable = !m_rotateEnable; }
	void switchCameraRotate() { m_rotateCamera = !m_rotateCamera; }
	void sliderCamera(float delta) { m_radius += delta; }
	void scaleModel(float scale) { m_scale += scale; }

	bool getCameraRotate() { return m_rotateCamera; }

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

	void drawViewportArea(const Vec2i& winMin, const Vec2i& winMax);

private:
	inline void drawPixel(int x, int y, const Vec3f& color)
	{
		if (NULL == m_fragmentBuff) return;
		if (x < 0 || y < 0) return;
		if (y * winWidth * 3 + x * 3 + 3 > winWidth * winHeight* PIX_BITS / 8) return;

		m_fragmentBuff[y * winWidth * 3 + x * 3 + 1] = color.g;
		m_fragmentBuff[y * winWidth * 3 + x * 3 + 2] = color.r;
		m_fragmentBuff[y * winWidth * 3 + x * 3 + 3] = color.b;
	}
};

