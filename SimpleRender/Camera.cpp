#include "Camera.h"
Camera::Camera() :m_pos(.0), m_lookDir(0, 0, 1), m_upDir(0, 1, 0)
{
	m_view = IdentityMatrix4x4<float>();
	setViewInfo(PI / 2, 1, -1, -1000);
	update();
}

Camera::~Camera() {}

Camera::Camera(const float& fov, const float& aspect, const float& zNear, const float& zFar) :m_pos(.0), m_lookDir(0, 0, 1), m_upDir(0, 1, 0)
{
	m_view = IdentityMatrix4x4<float>();
	setViewInfo(fov, aspect, zNear, zFar);
	update();
}

void Camera::setPos(const Vec3f& pos)
{
	m_pos = pos;
}

void Camera::setLookDir(const Vec3f& lookDir)
{
	m_type = Normal;
	m_lookDir = lookDir;
}

void Camera::setFocusPos(const Vec3f& focus)
{
	m_type = Focus;
	m_focus = focus; 
}

void Camera::setViewInfo(const float& fov, const float& aspect, const float& zNear, const float& zFar)
{
	m_fov = fov;
	m_aspect = aspect;
	m_zNear = zNear;
	m_zFar = zFar;

	float cot = tan((PI - fov) * 0.5f);

	m_proj = IdentityMatrix4x4<float>();
	m_proj[0][0] = cot / aspect;
	m_proj[1][1] = cot;
	m_proj[2][2] = (m_zNear + m_zFar) / (m_zNear - m_zFar);
	m_proj[2][3] = -(2 * m_zNear * m_zFar) / (m_zNear - m_zFar);
	m_proj[3][2] = -1;
	m_proj[3][3] = 0;
}

void Camera::update()
{
	if (m_type == Focus)
		m_lookDir = m_pos - m_focus;
	m_lookDir.normalize();

	m_rightDir = Cross(m_upDir, m_lookDir);
	m_rightDir.normalize();
	m_upDir = Cross(m_lookDir, m_rightDir);
	m_upDir.normalize();

	m_view[0][0] = m_rightDir.x;
	m_view[0][1] = m_rightDir.y;
	m_view[0][2] = m_rightDir.z;
	m_view[0][3] = -Dot(m_pos, m_rightDir);

	m_view[1][0] = m_upDir.x;
	m_view[1][1] = m_upDir.y;
	m_view[1][2] = m_upDir.z;
	m_view[1][3] = -Dot(m_pos, m_upDir);

	m_view[2][0] = m_lookDir.x;
	m_view[2][1] = m_lookDir.y;
	m_view[2][2] = m_lookDir.z;
	m_view[2][3] = -Dot(m_pos, m_lookDir);

	m_view[3][3] = 1;

	m_viewProj = m_proj * m_view;
}

Vec3f Camera::position()const
{
	return m_pos;
}

Vec3f Camera::lookDir()const
{
	return m_lookDir;
}

Vec3f Camera::rightDir()const
{
	return m_rightDir;
}

Vec3f Camera::upDir()const
{
	return m_upDir;
}

Vec3f Camera::focusPos()const
{
	return m_focus;
}

Matrix4x4f Camera::viewMatrix()const
{
	return m_view;
}

Matrix4x4f Camera::projectMatrix()const
{
	return m_proj;
}

Matrix4x4f Camera::viewProjMatrix()const
{
	return m_viewProj;
}