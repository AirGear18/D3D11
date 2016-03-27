#include "pch.h"
#include "Vector4.h"
#include "Matrix4.h"
#include <math.h>

Vector4& Vector4::operator=(const Vector4& v)
{
	if (this != &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	return *this;
}
Vector4 Vector4::operator+(const Vector4& v) const
{
	Vector4 vec;
	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;
	vec.w = w + v.w;
	return vec;
}
Vector4 Vector4::operator-(const Vector4& v) const
{
	Vector4 vec;
	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;
	vec.w = w - v.w;
	return vec;
}
Vector4 Vector4::operator+(const Vector4& v)
{
	Vector4 vec;
	vec.x = x + v.x;
	vec.y = y + v.y;
	vec.z = z + v.z;
	vec.w = w + v.w;
	return vec;
}
Vector4 Vector4::operator-(const Vector4& v)
{
	Vector4 vec;
	vec.x = x - v.x;
	vec.y = y - v.y;
	vec.z = z - v.z;
	vec.w = w - v.w;
	return vec;
}
void Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}

Vector4 Vector4::operator / (const Vector4& v)
{
	Vector4 a;
	a.x = x / v.x;
	a.y = y / v.y;
	a.z = z / v.z;
	a.w = w / v.w;

	return a;
}

void Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}
float& Vector4::operator[](const unsigned int index)
{
	return component[index];
}
const float& Vector4::operator[](const unsigned int index) const
{
	return component[index];
}

float Vector4::operator*(const Vector4& v) const
{
	return ((x * v.x) + (y * v.y) + (z * v.z) + (w * v.w));
}
Vector4 Vector4::operator*(const Matrix4& m) const
{
	Vector4 v;
	v.x = (x * m.Xx) + (y * m.Yx) + (z * m.Zx) + (w * m.Wx);
	v.y = (x * m.Xy) + (y * m.Yy) + (z * m.Zy) + (w * m.Wy);
	v.z = (x * m.Xz) + (y * m.Yz) + (z * m.Zz) + (w * m.Wz);
	v.w = (x * m.Xw) + (y * m.Yw) + (z * m.Zw) + (w * m.Ww);
	return v;
}
void Vector4::operator*= (const Matrix4& m)
{
	Vector4 v;
	v.x = (x * m.Xx) + (y * m.Yx) + (z * m.Zx) + (w * m.Wx);
	v.y = (x * m.Xy) + (y * m.Yy) + (z * m.Zy) + (w * m.Wy);
	v.z = (x * m.Xz) + (y * m.Yz) + (z * m.Zz) + (w * m.Wz);
	v.w = (x * m.Xw) + (y * m.Yw) + (z * m.Zw) + (w * m.Ww);
	*this = v;
}

float Vector4::ComputeMagnitude(void) const
{
	return sqrtf((x*x) + (y*y) + (z*z) + (w*w));
}
Vector4 Vector4::Homogenize(void)
{
	Vector4 v;
	v.x = (x / w);
	v.y = (y / w);
	v.z = (z / w);
	v.w = w;
	return v;
}
Vector4& Vector4::Normalize(void)
{
	float mag = ComputeMagnitude();
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
	return *this;
}
Vector4 Vector4::Negate(void) const
{
	Vector4 v;
	v.x = -x;
	v.y = -y;
	v.z = -z;
	v.w = -w;
	return v;
}

Vector4 Vector4::CrossProduct(const Vector4& a, const Vector4& b)
{
	Vector4 v;
	v.x = (a.y * b.z) - (a.z * b.y);
	v.y = (a.z * b.x) - (a.x * b.z);
	v.z = (a.x * b.y) - (a.y * b.x);
	v.w = 0;
	return v;
}
Vector4 Vector4::CrossProduct(const Vector4& b) const
{
	Vector4 v;
	v.x = (y * b.z) - (z * b.y);
	v.y = (z * b.x) - (x * b.z);
	v.z = (x * b.y) - (y * b.x);
	v.w = 0;
	return v;

}

float Vector4::Dot_Product(const Vector4& vectorA, const Vector4& vectorB)
{
	return vectorA.x*vectorB.x + vectorA.y*vectorB.y + vectorA.z*vectorB.z + vectorA.w*vectorB.w;
}