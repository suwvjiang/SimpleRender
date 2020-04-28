#include "MathUtils.h"

#pragma region Matrix3x3
//
void matrix3x3SetIdentity(Matrix3x3& matIdent3x3)
{
	int row, col;
	for (row = 0; row < 3; ++row)
	{
		for (col = 0; col < 3; ++col)
		{
			matIdent3x3[row][col] = (row == col);
		}
	}
}

void matrix3x3PreMultiply(Matrix3x3& m1, Matrix3x3& m2)
{
	int row, col;
	Matrix3x3 matTemp;
	for (row = 0; row < 3; ++row)
	{
		for (col = 0; col < 3; ++col)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col];
		}
	}

	for (row = 0; row < 3; ++row)
	{
		for (col = 0; col < 3; ++col)
		{
			m2[row][col] = matTemp[row][col];
		}
	}
}

void translate2D(float tx, float ty, Matrix3x3& matComposite)
{
	Matrix3x3 matTrans;
	matrix3x3SetIdentity(matTrans);

	matTrans[0][2] = tx;
	matTrans[1][2] = ty;

	matrix3x3PreMultiply(matTrans, matComposite);
}

void rotate2D(vector2D& pivotPt, float theta, Matrix3x3& matComposite)
{
	Matrix3x3 matRot;
	matrix3x3SetIdentity(matRot);

	matRot[0][0] = cos(theta);
	matRot[0][1] = -sin(theta);
	matRot[0][2] = pivotPt.x * (1 - cos(theta)) + pivotPt.y * sin(theta);

	matRot[1][0] = sin(theta);
	matRot[1][1] = cos(theta);
	matRot[1][2] = pivotPt.y * (1 - cos(theta)) - pivotPt.x * sin(theta);

	matrix3x3PreMultiply(matRot, matComposite);
}

void scale2D(float sx, float sy, vector2D& fixedPt, Matrix3x3& matComposite)
{
	Matrix3x3 matScale;
	matrix3x3SetIdentity(matScale);

	matScale[0][0] = sx;
	matScale[0][2] = (1 - sx) * fixedPt.x;

	matScale[1][1] = sy;
	matScale[1][2] = (1 - sy) * fixedPt.y;

	matrix3x3PreMultiply(matScale, matComposite);
}

void transformVerts2D(int nVerts, vector2D* verts, const Matrix3x3& matComposite)
{
	int k;
	float temp;

	for (k = 0; k < nVerts; ++k)
	{
		temp = matComposite[0][0] * verts[k].x + matComposite[0][1] * verts[k].y + matComposite[0][2];
		verts[k].y = matComposite[1][0] * verts[k].x + matComposite[1][1] * verts[k].y + matComposite[1][2];
		verts[k].x = temp;
	}
}

#pragma endregion

#pragma region Matrix4x4
void matrix4x4SetIdentity(Matrix4x4& matIdentity)
{
	int row, col;
	for (row = 0; row < 4; ++row)
		for (col = 0; col < 4; ++col)
			matIdentity[row][col] = col == row;
}

void matrix4x4PreMultiply(Matrix4x4& m1, Matrix4x4& m2)
{
	int row, col;
	Matrix4x4 matTemp;
	for (row = 0; row < 4; ++row)
	{
		for (col = 0; col < 4; ++col)
		{
			matTemp[row][col] = m1[row][0] * m2[0][col] + m1[row][1] * m2[1][col] + m1[row][2] * m2[2][col] + m1[row][3] * m2[3][col];
		}
	}

	for (row = 0; row < 4; ++row)
	{
		for (col = 0; col < 4; ++col)
		{
			m2[row][col] = matTemp[row][col];
		}
	}
}

//
void translate3D(float tx, float ty, float tz, Matrix4x4& matComposite3D)
{
	Matrix4x4 matTrans3D;
	matrix4x4SetIdentity(matTrans3D);

	matTrans3D[0][3] = tx;
	matTrans3D[1][3] = ty;
	matTrans3D[2][3] = tz;

	matrix4x4PreMultiply(matTrans3D, matComposite3D);
}

//
void rotate3D(const vector3D& p1, const vector3D& p2, float radianAngle, Matrix4x4& matComposite3D)
{
	Matrix4x4 matQuaternionRot;
	float axisVectLen = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z));

	float sinA = sin(radianAngle);
	float cosA = cos(radianAngle);
	float oneC = 1 - cosA;

	float ux = (p2.x - p1.x) / axisVectLen;
	float uy = (p2.y - p1.y) / axisVectLen;
	float uz = (p2.z - p1.z) / axisVectLen;

	translate3D(-p1.x, -p1.y, -p1.z, matComposite3D);

	matrix4x4SetIdentity(matQuaternionRot);
	matQuaternionRot[0][0] = ux * ux * oneC + cosA;
	matQuaternionRot[0][1] = ux * uy * oneC - uz * sinA;
	matQuaternionRot[0][2] = ux * uz * oneC + uy * sinA;
	matQuaternionRot[1][0] = uy * ux * oneC + uz * sinA;
	matQuaternionRot[1][1] = uy * uy * oneC + cosA;
	matQuaternionRot[1][2] = uy * uz * oneC - ux * sinA;
	matQuaternionRot[2][0] = uz * ux * oneC - uy * sinA;
	matQuaternionRot[2][1] = uz * uy * oneC + ux * sinA;
	matQuaternionRot[2][2] = uz * uz * oneC + cosA;

	matrix4x4PreMultiply(matQuaternionRot, matComposite3D);

	translate3D(p1.x, p1.y, p1.z, matComposite3D);
}

void scale3D(float sx, float sy, float sz, const vector3D& fixedPt, Matrix4x4& matComposite3D)
{
	Matrix4x4 matScale3D;
	matrix4x4SetIdentity(matScale3D);

	matScale3D[0][0] = sx;
	matScale3D[0][3] = (1 - sx) * fixedPt.x;
	matScale3D[1][0] = sy;
	matScale3D[1][3] = (1 - sy) * fixedPt.y;
	matScale3D[2][0] = sz;
	matScale3D[2][3] = (1 - sz) * fixedPt.z;

	matrix4x4PreMultiply(matScale3D, matComposite3D);
}

void transformVerts3D(int nVerts, vector3D* verts, const Matrix4x4& matComposite3D)
{
	int k;
	for (k = 0; k < nVerts; ++k)
	{
		float tempX = verts[k].x * matComposite3D[0][0] + verts[k].y * matComposite3D[0][1] + verts[k].z * matComposite3D[0][2] + verts[k].w * matComposite3D[0][3];
		float tempY = verts[k].x * matComposite3D[1][0] + verts[k].y * matComposite3D[1][1] + verts[k].z * matComposite3D[1][2] + verts[k].w * matComposite3D[1][3];
		float tempZ = verts[k].x * matComposite3D[2][0] + verts[k].y * matComposite3D[2][1] + verts[k].z * matComposite3D[2][2] + verts[k].w * matComposite3D[2][3];
		verts[k].w = verts[k].x * matComposite3D[3][0] + verts[k].y * matComposite3D[3][1] + verts[k].z * matComposite3D[3][2] + verts[k].w * matComposite3D[3][3];
		verts[k].z = tempZ;
		verts[k].y = tempY;
		verts[k].x = tempX;
	}
}
#pragma endregion

#pragma region 3D View
//相机矩阵
void generateCameraModel(Matrix4x4& cameraMatrix, const vector3D& origin, const vector3D& lookAt, const vector3D& upDir)
{
	matrix4x4SetIdentity(cameraMatrix);

	vector3D zDir = dir(lookAt, origin);
	normalize(zDir);
	vector3D xDir = crossPlus(upDir, zDir);
	normalize(xDir);
	vector3D yDir = crossPlus(zDir, xDir);
	normalize(yDir);

	cameraMatrix[0][0] = xDir.x;
	cameraMatrix[0][1] = xDir.y;
	cameraMatrix[0][2] = xDir.z;
	cameraMatrix[0][3] = -dotPlus(origin, xDir);
	cameraMatrix[1][0] = yDir.x;
	cameraMatrix[1][1] = yDir.y;
	cameraMatrix[1][2] = yDir.z;
	cameraMatrix[1][3] = -dotPlus(origin, yDir);
	cameraMatrix[2][0] = zDir.x;
	cameraMatrix[2][1] = zDir.y;
	cameraMatrix[2][2] = zDir.z;
	cameraMatrix[2][3] = -dotPlus(origin, zDir);
}
//投影矩阵
void generateProjectModel(Matrix4x4& proMatrix, float fov, float aspect, float znear, float zfar)
{
	matrix4x4SetIdentity(proMatrix);

	float cot = atan(fov * 0.5);

	proMatrix[0][0] = cot / aspect;
	proMatrix[1][1] = cot;
	proMatrix[2][2] = (znear + zfar) / (znear - zfar);
	proMatrix[2][3] = -(2 * znear * zfar) / (znear - zfar);
	proMatrix[3][2] = -1;
	proMatrix[3][3] = 0;
}
//投屏矩阵
void generateScreenModel(Matrix4x4& screenMatrix, const vector3D& center, float width, float height)
{
	matrix4x4SetIdentity(screenMatrix);

	screenMatrix[0][0] = width / 2;
	screenMatrix[0][3] = center.x;
	screenMatrix[1][1] = height / 2;
	screenMatrix[1][3] = center.y;
	screenMatrix[2][2] = 0;
	screenMatrix[3][3] = 0;
}
#pragma endregion

#pragma region 2DClip
int encode(vector2D& pt, const vector2D& winMin, const vector2D& winMax)
{
	int code = 0x00;
	if (pt.x < winMin.x)
		code = code | winLeftBitCode;
	if (pt.x > winMax.x)
		code |= winRightBitCode;
	if (pt.y < winMin.y)
		code |= winBottomBitCode;
	if (pt.y > winMax.y)
		code |= winTopBitCode;
	return code;
}

int clipTest(float p, float q, float* uIn, float* uOut)
{
	float r;
	int returnValue = true;

	if (p < .0)//入边取最大值
	{
		r = q / p;
		if (r > * uOut)
			returnValue = false;
		else
		{
			if (r > * uIn)
				*uIn = r;
		}
	}
	else
	{
		if (p > .0)//出边取最小值
		{
			r = q / p;
			if (r < *uIn)
				returnValue = false;
			else
			{
				if (r < *uOut)
					*uOut = r;
			}
		}
		else//与边界平行
			if (q < .0)
				returnValue = false;
	}
	return returnValue;
}

#pragma endregion