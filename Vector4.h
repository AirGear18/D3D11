#pragma once

struct Matrix4;
struct Vector4
{
public:
	//dtor
	~Vector4() = default;

	//overloads
	Vector4& operator=(const Vector4&);
	Vector4 operator+(const Vector4&) const;
	Vector4 operator-(const Vector4&) const;
	Vector4 operator+(const Vector4&);
	Vector4 operator-(const Vector4&);
	Vector4 operator*(const Matrix4&) const;

	void operator+=(const Vector4&);
	void operator-=(const Vector4&);
	float operator*(const Vector4&) const;
	void operator*=(const Matrix4&);

	float& operator[](const unsigned int);
	const float& operator[](const unsigned int) const;
	
	Vector4& Normalize(void);
	Vector4 Homogenize(void);
	Vector4 Negate(void) const;
	float ComputeMagnitude(void) const;
	
	static Vector4 CrossProduct(const Vector4& a, const Vector4& b);
	Vector4 CrossProduct(const Vector4& b) const;

	template<typename Type>
	Vector4 operator*(const Type) const;
	template<typename Type>
	void operator*=(const Type);

	union
	{
		float component[4];
		struct
		{
		float x, y, z, w;
		};
		struct
		{
		float r, g, b, a;
		};
		struct
		{
		float u, v, padU, padV;
		};

	};
};

template<typename Type>
Vector4 Vector4::operator*(const Type val) const
{
	Vector4 vec;
	vec.x = x * val;
	vec.y = y * val;
	vec.z = z * val;
	vec.w = w * val;
	return vec;

}
template<typename Type>
void Vector4::operator*=(const Type val)
{
	x *= val;
	y *= val;
	z *= val;
	w *= val;
}
