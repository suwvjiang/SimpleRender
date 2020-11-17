#pragma once
#include "Vector.h"
#include "Matrix.h"

const double TWO_PI = 6.2831853;
const double PI = 3.1415926;

const static Vec3f Color_White(1.0);
const static Vec3f Color_Black(.0);
const static Vec3f Color_Red(1.0, 0.0, 0.0);
const static Vec3f Color_Green(0.0, 1.0, 0.0);
const static Vec3f Color_Blue(0.0, 0.0, 1.0);

inline int Sign(const int& a) { return (a > 0) - (a < 0); }

template<typename T>
inline T Lerp(const T& v1, const T& v2, const float& t) { return v1 * (1 - t) + v2 * t; }

template<typename T>
static inline void Swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

template<typename T>
Vector3<T> Transform(const Matrix3x3<T>& m, const Vector3<T>& v)
{
	Vector3<T> temp;
	temp.x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2];
	temp.y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2];
	temp.z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2];
	return temp;
}

template<typename T>
Vector4<T> Transform(const Matrix4x4<T>& m, const Vector4<T>& v)
{
	Vector4<T> temp;
	temp.x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + v.w * m[0][3];
	temp.y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + v.w * m[1][3];
	temp.z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + v.w * m[2][3];
	temp.w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + v.w * m[3][3];
	return temp;
}

template<typename T>
void TransformVectors(const int& nVerts, Vector4<T>* verts, const Matrix4x4<T>& m)
{
	for (int i = 0; i < nVerts; ++i)
	{
		verts[i] = Transform(m, verts[i]);
	}
}

#pragma region Matrix3x3
/*----------------*/
/* Matrix3x3 Math */
/*----------------*/

//位移矩阵
template<typename T>
Matrix3x3<T> Matrix3x3Translation(const float& x, const float& y)
{
	Matrix3x3<T> mat = IdentityMatrix3x3<T>();
	mat[0][2] = x;
	mat[1][2] = y;
	mat[2][2] = 1;
	return mat;
}
template<typename T>
Matrix3x3<T> Matrix3x3TranslationFromVector(const Vector3<T>& v)
{
	Matrix3x3<T> mat = IdentityMatrix3x3<T>();
	mat[0][2] = v.x;
	mat[1][2] = v.y;
	mat[2][2] = 1;
	return mat;
}

//缩放矩阵
template<typename T>
Matrix3x3<T> Matrix3x3Scaling(const float& scale_x, const float& scale_y)
{
	Matrix4x4<T> mat = ZeroMatrix3x3<T>();
	mat[0][0] = scale_x;
	mat[1][1] = scale_y;
	mat[2][2] = 1;

	return mat;
}
template<typename T>
Matrix3x3<T> Matrix3x3ScalingFromVector(const Vector3<T>& v)
{
	Matrix4x4<T> mat = IdentityMatrix3x3<T>();
	mat[0][0] = v.x;
	mat[1][1] = v.y;
	mat[2][2] = 1;
	return mat;
}

//旋转矩阵
template<typename T>
Matrix3x3<T> Matrix3x3RotationOrigin(const T& theta_radian)
{
	Matrix4x4<T> mat = IdentityMatrix3x3<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = cos_theta;
	mat[0][1] = -sin_theta;

	mat[1][0] = sin_theta;
	mat[1][1] = cos_theta;

	mat[2][2] = 1;

	return mat;
}
template<typename T>
Matrix3x3<T> Matrix3x3RotationPoint(const Vector3<T>& center, const T& theta_radian)
{
	Matrix4x4<T> mat = IdentityMatrix3x3<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = cos_theta;
	mat[0][1] = -sin_theta;
	mat[0][2] = (1 - cos_theta) * center.x + center.y * sin_theta;

	mat[1][0] = sin_theta;
	mat[1][1] = cos_theta;
	mat[1][2] = -center.x * sin_theta + (1 - cos_theta) * center.y;

	mat[2][2] = 1;

	return mat;
}
#pragma endregion

#pragma region Matrix4x4
/*----------------*/
/* Matrix4x4 Math */
/*----------------*/

//位移矩阵
template<typename T>
Matrix4x4<T> Matrix4x4Translation(const T& x, const T& y, const T& z)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();
	mat[0][3] = x;
	mat[1][3] = y;
	mat[2][3] = z;
	mat[3][3] = 1;
	return mat;
}
template<typename T>
Matrix4x4<T> Matrix4x4TranslationFromVector(const Vector3<T>& v)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();
	mat[0][3] = v.x;
	mat[1][3] = v.y;
	mat[2][3] = v.z;
	mat[3][3] = 1;
	return mat;
}

//缩放矩阵
template<typename T>
Matrix4x4<T> Matrix4x4Scaling(const float& scale_x, const float& scale_y, const float& scale_z)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();
	mat[0][0] = scale_x;
	mat[1][1] = scale_y;
	mat[2][2] = scale_z;
	mat[3][3] = 1;
	return mat;
}
template<typename T>
Matrix4x4<T> Matrix4x4ScalingFromVector(const Vector3<T>& v)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();
	mat[0][0] = v.x;
	mat[1][1] = v.y;
	mat[2][2] = v.z;
	mat[3][3] = 1;
	return mat;
}

//旋转矩阵
template<typename T>
Matrix4x4<T> Matrix4x4RotationX(const T& theta_radian)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = 1;
	mat[1][1] = cos_theta;
	mat[1][2] = -sin_theta;
	mat[2][1] = sin_theta;
	mat[2][2] = cos_theta;
	mat[3][3] = 1;
	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4RotationY(const T& theta_radian)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = cos_theta;
	mat[0][2] = sin_theta;
	mat[1][1] = 1;
	mat[2][0] = -sin_theta;
	mat[2][2] = cos_theta;
	mat[3][3] = 1;
	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4RotationZ(const T& theta_radian)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();

	T cos_theta = std::cos(theta_radian);
	T sin_theta = std::sin(theta_radian);

	mat[0][0] = cos_theta;
	mat[0][1] = -sin_theta;
	mat[1][0] = sin_theta;
	mat[1][1] = cos_theta;
	mat[2][2] = 1;
	mat[3][3] = 1;
	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4RotationAxis(const Vector3<T>& axi, const T& theta_radian)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();

	T sin_theta = std::sin(theta_radian);
	T cos_theta = std::cos(theta_radian);
	T sub_cos_theta = 1 - cos_theta;

	T x = axi.x;
	T y = axi.y;
	T z = axi.z;

	mat[0][0] = cos_theta + sub_cos_theta * x * x;
	mat[0][1] = sub_cos_theta * x * y - sin_theta * z; 
	mat[0][2] = sub_cos_theta * x * z + sin_theta * y;

	mat[1][0] = sub_cos_theta * x * y + sin_theta * z;
	mat[1][1] = cos_theta + sub_cos_theta * y * y;
	mat[1][2] = sub_cos_theta * y * z - sin_theta * x; 

	mat[2][0] = sub_cos_theta * x * z - sin_theta * y; 
	mat[2][1] = sub_cos_theta * y * z + sin_theta * x;
	mat[2][2] = cos_theta + sub_cos_theta * z * z;

	mat[3][3] = 1;

	return mat;
}
#pragma endregion

#pragma region 3D View
template<typename T>
Matrix4x4<T> Matrix4x4Camera(const Vector3<T>& eyePos, const Vector3<T>& focusPos, const Vector3<T>& upDir = Vector3<T>(0, 1, 0))
{
	Vector3<T> zDir = eyePos - focusPos;
	zDir.normalize();
	Vector3<T> xDir = Cross(upDir, zDir);
	xDir.normalize();
	Vector3<T> yDir = Cross(zDir, xDir);
	yDir.normalize();

	Matrix4x4<T> mat = IdentityMatrix4x4<T>();

	mat[0][0] = xDir.x;
	mat[0][1] = xDir.y;
	mat[0][2] = xDir.z;
	mat[0][3] = -Dot(eyePos, xDir);

	mat[1][0] = yDir.x;
	mat[1][1] = yDir.y;
	mat[1][2] = yDir.z;
	mat[1][3] = -Dot(eyePos, yDir);

	mat[2][0] = zDir.x;
	mat[2][1] = zDir.y;
	mat[2][2] = zDir.z;
	mat[2][3] = -Dot(eyePos, zDir);

	mat[3][3] = 1;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4Perspect(const float& fov, const float& aspect, const float& znear, const float& zfar)
{
	float cot = atan(fov * 0.5);

	Matrix4x4<T> mat = IdentityMatrix4x4<T>();
	mat[0][0] = cot / aspect;
	mat[1][1] = cot;
	mat[2][2] = (znear + zfar) / (znear - zfar);
	mat[2][3] = -(2 * znear * zfar) / (znear - zfar);
	mat[3][2] = -1;
	mat[3][3] = 0;

	return mat;
}

template<typename T>
Matrix4x4<T> Matrix4x4Screen(const Vector2<T>& center, const float& width, const float& height)
{
	Matrix4x4<T> mat = IdentityMatrix4x4<T>();
	mat[0][0] = width * 0.5;
	mat[0][3] = center.x;
	mat[1][1] = height * 0.5;
	mat[1][3] = center.y;

	return mat;
}
#pragma endregion

#pragma region Tringle Geometry
//检测线段相交
inline bool FasterLineSegmentIntersection(const Vec2i& a, const Vec2i& b, const Vec2i& c, const Vec2i& d)
{
	Vec2i ab = b - a;
	Vec2i cd = d - c;

	Vec2i ac = c - a;
	Vec2i ad = d - a;
	Vec2i cb = b - c;

	float t = Cross(ac, ab) * Cross(ad, ab);
	float p = Cross(cd, ac) * Cross(cb, cd);//Cross(cd, ac) = Cross(ac, cd)
	return (t <= 0) && (p <= 0);
}
//检测线段与三角形相交
inline bool SegmenTriangleIntersection(const Vec2i& a, const Vec2i& b, const Vec2i& v0, const Vec2i& v1, const Vec2i& v2)
{
	if (FasterLineSegmentIntersection(a, b, v0, v1)
		|| FasterLineSegmentIntersection(a, b, v1, v2)
		|| FasterLineSegmentIntersection(a, b, v2, v1))
	{
		return true;
	}
	return false;
}
//检测方块与三角形相交
inline bool BlockTriangleSegmentIntersection(const Vec2i& lt, const int blockSize, const Vec2i& v0, const Vec2i& v1, const Vec2i& v2)
{
	Vec2i rt(lt.x + blockSize - 1, lt.y);
	Vec2i lb(lt.x, lt.y + blockSize - 1);
	Vec2i rb(lt.x + blockSize - 1, lt.y + blockSize - 1);

	if (SegmenTriangleIntersection(lt, rt, v0, v1, v2)
		|| SegmenTriangleIntersection(lt, lb, v0, v1, v2)
		|| SegmenTriangleIntersection(rt, rb, v0, v1, v2)
		|| SegmenTriangleIntersection(lb, rb, v0, v1, v2))
	{
		return true;
	}
	return false;
}
#pragma endregion