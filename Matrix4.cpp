#include "pch.h"
#include "Matrix4.h"
#include <math.h>


//#define PI 3.1415f
/***********************************************************************************
|	Function: Matrix4 Copy Constructor
|	Purpose: create a matrix with the same values as another
|	Paramter(s):
|				const Matrix4& - a constant matrix reference to copy from
|	Return Value(s): N/A
***********************************************************************************/
Matrix4::Matrix4(const Matrix4& src)
{
	*this = src;
}

/***********************************************************************************
|	Function: Matrix4 Constructor [overload]
|	Purpose: create a matrix using four vectors
|	Paramter(s):
|			const Vector4& X- constant vector reference for the X-Axis
|			const Vector4& Y- constant vector reference for the W-Axis
|			const Vector4& Z- constant vector reference for the Z-Axis
|			const Vector4& W- constant vector reference for the W-Axis
|	Return Value(s): N/A
***********************************************************************************/
Matrix4::Matrix4(const Vector4& X, const Vector4& Y, const Vector4& Z, const Vector4& W)
{
	XAxis = X;
	YAxis = Y;
	ZAxis = Z;
	WAxis = W;
}

Matrix4::Matrix4(float _Xx, float _Xy, float _Xz, float _Xw,
	float _Yx, float _Yy, float _Yz, float _Yw,
	float _Zx, float _Zy, float _Zz, float _Zw,
	float _Wx, float _Wy, float _Wz, float _Ww)
{
	Xx = _Xx; 	Xy = _Xy;  Xz = _Xz;  	Xw = _Xw;
	Yx = _Yx;	Yy = _Yy;  Yz = _Yz;	Yw = _Yw;
	Zx = _Zx;	Zy = _Zy;  Zz = _Zz;	Zw = _Zw;
	Wx = _Wx;	Wy = _Wy;  Wz = _Wz;	Ww = _Ww;
}


Matrix4& Matrix4::operator=(const Matrix4& src)
{
	if (this != &src)
	{
		for (unsigned int i = 0; i < 16; ++i)
			matrixArr[i] = src.matrixArr[i];
	}
	return *this;
}
Matrix4 Matrix4::operator*(const Matrix4& other) const
{
	Matrix4 m;

	m.Xx = (Xx * other.Xx) + (Xy * other.Yx) + (Xz * other.Zx) + (Xw * other.Wx);
	m.Xy = (Xx * other.Xy) + (Xy * other.Yy) + (Xz * other.Zy) + (Xw * other.Wy);
	m.Xz = (Xx * other.Xz) + (Xy * other.Yz) + (Xz * other.Zz) + (Xw * other.Wz);
	m.Xw = (Xx * other.Xw) + (Xy * other.Yw) + (Xz * other.Zw) + (Xw * other.Ww);

	m.Yx = (Yx * other.Xx) + (Yy * other.Yx) + (Yz * other.Zx) + (Yw * other.Wx);
	m.Yy = (Yx * other.Xy) + (Yy * other.Yy) + (Yz * other.Zy) + (Yw * other.Wy);
	m.Yz = (Yx * other.Xz) + (Yy * other.Yz) + (Yz * other.Zz) + (Yw * other.Wz);
	m.Yw = (Yx * other.Xw) + (Yy * other.Yw) + (Yz * other.Zw) + (Yw * other.Ww);

	m.Zx = (Zx * other.Xx) + (Zy * other.Yx) + (Zz * other.Zx) + (Zw * other.Wx);
	m.Zy = (Zx * other.Xy) + (Zy * other.Yy) + (Zz * other.Zy) + (Zw * other.Wy);
	m.Zz = (Zx * other.Xz) + (Zy * other.Yz) + (Zz * other.Zz) + (Zw * other.Wz);
	m.Zw = (Zx * other.Xw) + (Zy * other.Yw) + (Zz * other.Zw) + (Zw * other.Ww);

	m.Wx = (Wx * other.Xx) + (Wy * other.Yx) + (Wz * other.Zx) + (Ww * other.Wx);
	m.Wy = (Wx * other.Xy) + (Wy * other.Yy) + (Wz * other.Zy) + (Ww * other.Wy);
	m.Wz = (Wx * other.Xz) + (Wy * other.Yz) + (Wz * other.Zz) + (Ww * other.Wz);
	m.Ww = (Wx * other.Xw) + (Wy * other.Yw) + (Wz * other.Zw) + (Ww * other.Ww);

	return m;
}
void Matrix4::operator*=(const Matrix4& other)
{
	*this = (*this) * other;
}
//STATIC
Matrix4 Matrix4::Identity(void)
{
	return Matrix4();
}

Matrix4 Matrix4::RotateX(float rad)
{
	Matrix4 m;
	//Y-axis
	m.Yy = cosf(rad);
	m.Yz = -sinf(rad);
	//Z-axis
	m.Zy = sinf(rad);
	m.Zz = cosf(rad);

	return m;
}
Matrix4 Matrix4::RotateY(float rad)
{
	Matrix4 m;
	//X-Axis
	m.Xx = cosf(rad);
	m.Xz = sinf(rad);
	//Z-axiz
	m.Zx = -sinf(rad);
	m.Zz = cosf(rad);
	return m;
}
Matrix4 Matrix4::RotateZ(float rad)
{
	Matrix4 m;
	//X-axis
	m.Xx = cosf(rad);
	m.Xy = -sinf(rad);
	//Y-axis
	m.Yx = sinf(rad);
	m.Yy = cosf(rad);

	return m;
}
Matrix4 Matrix4::GetScale(const Matrix4& mat, float _x, float _y, float _z)
{
	Matrix4 m(mat);
	m.Xx *= _x;
	m.Yy *= _y;
	m.Zz *= _z;
	return m;
}

//NON-STATIC
Matrix4 Matrix4::Inverse(void) const
{
	//copy matric
	Matrix4 m(*this);
	//copy position vector
	Vector4 pos = m.WAxis;
	m.WAxis = { 0, 0, 0, 1 };
	//transpose
	m = m.Transpose();
	//multiply by the matrix
	pos = pos * m;
	//negate
	pos = pos.Negate();
	//reset position
	m.WAxis = pos;
	m.WAxis.w = 1;

	return m;
}
Matrix4 Matrix4::Transpose(void) const
{
	Matrix4 m;
	for (unsigned int x = 0; x < 4; ++x)
		for (unsigned int y = 0; y < 4; ++y)
			m.matrix[x][y] = matrix[y][x];
	return m;

}
void Matrix4::LocalTranslate(const Vector4& translate)
{
	WAxis += (XAxis * translate.x) + (YAxis * translate.y) + (ZAxis * translate.z);
}
void Matrix4::LocalTranslate(float _x, float _y, float _z)
{
	WAxis += (XAxis * _x) + (YAxis * _y) + (ZAxis * _z);

}
void Matrix4::GlobalTranslate(const Vector4& translate)
{
	Wx += translate.x;
	Wy += translate.y;
	Wz += translate.z;
}
void Matrix4::GlobalTranslate(float _x, float _y, float _z)
{
	Wx += _x;
	Wy += _y;
	Wz += _z;
}

Matrix4 Matrix4::GetLocalTranslate(const Matrix4& m, const Vector4& translate)
{
	Matrix4 mat = m;
	mat.WAxis += (m.XAxis * translate.x) + (m.YAxis * translate.y) + (m.ZAxis * translate.z);
	return mat;
}
Matrix4 Matrix4::GetLocalTranslate(const Matrix4& m, float _x, float _y, float _z)
{
	Matrix4 mat = m;
	mat.WAxis += (m.XAxis * _x) + (m.YAxis * _y) + (m.ZAxis * _z);
	return mat;

}
Matrix4 Matrix4::GetGlobalTranslate(const Matrix4& m, const Vector4& translate)
{
	Matrix4 mat = m;
	mat.Wx += translate.x;
	mat.Wy += translate.y;
	mat.Wz += translate.z;
	return mat;
}
Matrix4 Matrix4::GetGlobalTranslate(const Matrix4& m, float _x, float _y, float _z)
{
	Matrix4 mat = m;
	mat.Wx += _x;
	mat.Wy += _y;
	mat.Wz += _z;
	return mat;

}

void Matrix4::LocalRotation(const Matrix4& m)
{
	*this = m* *this;
}
void Matrix4::GlobalRotation(const Matrix4& m)
{
	*this *= m;
}
Matrix4 Matrix4::GetLocalRotation(const Matrix4& mat, const Matrix4& rotation)
{
	return rotation * mat;
}
Matrix4 Matrix4::GetGlobalRotation(const Matrix4& mat, const Matrix4& rotation)
{
	return mat * rotation;
}
void Matrix4::Scale(const Vector4& scale)
{
	Xx *= scale.x;
	Yy *= scale.y;
	Zz *= scale.z;
}
void Matrix4::Scale(float _x, float _y, float _z)
{
	Xx *= _x;
	Yy *= _y;
	Zz *= _z;
}

XMMATRIX Matrix4::ConvertToXMMatrix()
{
	XMFLOAT4X4 temp;
	memcpy(&temp, this, sizeof(float) * 16);
	//temp._44 = 1;
	temp._43 = -temp._43;
	temp._42 = -temp._42;
	temp._41 = -temp._41;
	return XMLoadFloat4x4(&temp);
}