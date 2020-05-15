#pragma once
#include "MathUtils.h"
typedef enum CameraType
{
	Normal,
	Focus,
};

class Camera
{
public:
	Camera();
	~Camera();
	Camera(const float& fov, const float& aspect, const float& zNear, const float& zFar);

	Vec3f position() const;
	Vec3f lookDir() const;
	Vec3f rightDir() const;
	Vec3f upDir() const;
	Vec3f focusPos()const;

	Matrix4x4f viewMatrix() const;
	Matrix4x4f projectMatrix() const;
	Matrix4x4f viewProjMatrix() const;

	void setPos(const Vec3f& pos);
	void setFocusPos(const Vec3f& focus);
	void setLookDir(const Vec3f& lookDir);
	void setViewInfo(const float& fov, const float& aspect, const float& zNear, const float& zFar);
	void update();
private:
	CameraType m_type = Normal;

	Vec3f m_pos;
	Vec3f m_focus;
	Vec3f m_lookDir;
	Vec3f m_rightDir;
	Vec3f m_upDir;

	float m_fov;
	float m_aspect;
	float m_zNear;
	float m_zFar;

	Matrix4x4f m_view;
	Matrix4x4f m_proj;
	Matrix4x4f m_viewProj;

};

