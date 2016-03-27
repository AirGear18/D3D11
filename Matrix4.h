#pragma once
#include "Vector4.h"

struct Matrix4
{
public:
	Matrix4(const Matrix4&);
	Matrix4(const Vector4& X, const Vector4& Y, const Vector4& Z, const Vector4& W);
	Matrix4(float _Xx = 1.f, float _Xy = 0.f, float _Xz = 0.f, float _Xw = 0.f,
		float _Yx = 0.f, float _Yy = 1.f, float _Yz = 0.f, float _Yw = 0.f,
		float _Zx = 0.f, float _Zy = 0.f, float _Zz = 1.f, float _Zw = 0.f,
		float _Wx = 0.f, float _Wy = 0.f, float _Wz = 0.f, float _Ww = 1.f);

	~Matrix4() = default;

	Matrix4& operator=(const Matrix4&);
	Matrix4 operator*(const Matrix4&) const;
	void operator*=(const Matrix4&);

	static Matrix4 Identity(void);

	static Matrix4 RotateX(float rad);
	static Matrix4 RotateY(float rad);
	static Matrix4 RotateZ(float rad);

	static Matrix4 GetLocalTranslate(const Matrix4&, const Vector4&);
	static Matrix4 GetLocalTranslate(const Matrix4&, float _x, float _y, float _z);
	static Matrix4 GetGlobalTranslate(const Matrix4&, const Vector4&);
	static Matrix4 GetGlobalTranslate(const Matrix4&, float _x, float _y, float _z);
	static Matrix4 GetLocalRotation(const Matrix4&, const Matrix4&);
	static Matrix4 GetGlobalRotation(const Matrix4&, const Matrix4&);
	static Matrix4 GetScale(const Matrix4&, float _x = 1, float _y = 1, float _z = 1);

	Matrix4 Inverse(void) const;
	Matrix4 Transpose(void) const;

	void LocalTranslate(const Vector4&);
	void LocalTranslate(float _x, float _y, float _z);
	void GlobalTranslate(const Vector4&);
	void GlobalTranslate(float _x, float _y, float _z);
	void LocalRotation(const Matrix4&);
	void GlobalRotation(const Matrix4&);
	void Scale(const Vector4&);
	void Scale(float _x, float _y, float _z);

	XMMATRIX ConvertToXMMatrix();

	union
	{
		//matrix as a 2d array
		float matrix[4][4];
		//matrix as a 1d array
		float matrixArr[16];
		//matrix as individual components
		struct
		{
			float Xx, Xy, Xz, Xw;
			float Yx, Yy, Yz, Yw;
			float Zx, Zy, Zz, Zw;
			float Wx, Wy, Wz, Ww;
		};
		struct
		{
			Vector4 XAxis;
			Vector4 YAxis;
			Vector4 ZAxis;
			Vector4 WAxis;
		};
	};

};

