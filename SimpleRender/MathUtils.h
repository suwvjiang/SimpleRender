#pragma once
#include <math.h>

const double TWO_PI = 6.2831853;
const double PI = 3.1415926;

typedef float Matrix3x3[3][3];
typedef float Matrix4x4[4][4];

class vector2D
{
public:
	float x, y, z;
	vector2D() { x = y = 0.0; z = 1; }
	void setCoords(float vx, float vy, float vz = 1) { x = vx; y = vy; z = vz; }
};

class vector3D
{
public:
	float x, y, z, w;
	vector3D() {
		x = y = z = 0;
		w = 1;
	}
	void setCoords(float xCoord, float yCoord, float zCoord, float wCoord = 1)
	{
		x = xCoord;
		y = yCoord;
		z = zCoord;
		w = wCoord;
	}

	void homogeneous()
	{
		x = x / w;
		y = y / w;
		z = z / w;
		w = 1;
	}
};

inline int signPlus(const int a) { return (a > 0) - (a < 0); }

inline float lerpPlus(const float v1, const float v2, const float t) { return v1 + (v2 - v1) * t; }

inline float dotPlus(const vector3D& p1, const vector3D& p2) { return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z; }

inline vector3D crossPlus(const vector3D& p1, const vector3D& p2)
{
	vector3D pt;
	pt.setCoords(p1.y * p2.z - p1.z * p2.y, p1.z * p2.x - p1.x * p2.z, p1.x * p2.y - p1.y * p2.x);
	return pt;
}

inline vector3D dir(const vector3D& p1, const vector3D& p2)
{
	vector3D result;
	result.setCoords(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	return result;
}

inline void normalize(vector3D& pt)
{
	float len = sqrt(dotPlus(pt, pt));
	pt.x /= len;
	pt.y /= len;
	pt.z /= len;
}

inline void swapPts(vector2D* p1, vector2D* p2)
{
	vector2D temp;
	temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

void matrix3x3SetIdentity(Matrix3x3& matIdent3x3);
void matrix3x3PreMultiply(Matrix3x3& m1, Matrix3x3& m2);
void translate2D(float tx, float ty, Matrix3x3& matComposite);
void rotate2D(vector2D& pivotPt, float theta, Matrix3x3& matComposite);
void scale2D(float sx, float sy, vector2D& fixedPt, Matrix3x3& matComposite);
void transformVerts2D(int nVerts, vector2D* verts, const Matrix3x3& matComposite);

void matrix4x4SetIdentity(Matrix4x4& matIdentity);
void matrix4x4PreMultiply(Matrix4x4& m1, Matrix4x4& m2);
void translate3D(float tx, float ty, float tz, Matrix4x4& matComposite3D);
void rotate3D(const vector3D& p1, const vector3D& p2, float radianAngle, Matrix4x4& matComposite3D);
void scale3D(float sx, float sy, float sz, const vector3D& fixedPt, Matrix4x4& matComposite3D);
void transformVerts3D(int nVerts, vector3D* verts, const Matrix4x4& matComposite3D);

void generateCameraModel(Matrix4x4& cameraMatrix, const vector3D& origin, const vector3D& lookAt, const vector3D& upDir);
void generateProjectModel(Matrix4x4& proMatrix, float fov, float aspect, float znear, float zfar);
void generateScreenModel(Matrix4x4& screenMatrix, const vector3D& center, float width, float height);


#pragma region 2DClip
const int winLeftBitCode = 0x1;
const int winRightBitCode = 0x2;
const int winBottomBitCode = 0x4;
const int winTopBitCode = 0x8;

inline int inside(int code) { return int(!code); }//È¡·´
inline int reject(int code1, int code2) { return int(code1 & code2); }//ÇÒ
inline int accept(int code1, int code2) { return int(!(code1 | code2)); }

int encode(vector2D& pt, const vector2D& winMin, const vector2D& winMax);
int clipTest(float p, float q, float* uIn, float* uOut);
#pragma  endregion