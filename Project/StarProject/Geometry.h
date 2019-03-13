#pragma once
#include <DxLib.h>

struct Vector3 {
	Vector3() :x(0), y(0), z(0) {}
	Vector3(float inx, float iny, float inz) : x(inx), y(iny), z(inz) {}
	float x, y, z;
	void operator=(const Vector3& in) {
		x = in.x;
		y = in.y;
		z = in.z;
	}
	void operator=(const VECTOR& in) {
		x = in.x;
		y = in.y;
		z = in.z;
	}
	void operator+=(const Vector3& in) {
		x += in.x;
		y += in.y;
		z += in.z;
	}
	void operator-=(const Vector3& in) {
		x -= in.x;
		y -= in.y;
		z -= in.z;
	}
	void operator*=(const float& in) {
		x *= in;
		y *= in;
		z *= in;
	}
	void operator/=(const float& in) {
		x /= in;
		y /= in;
		z /= in;
	}

	void Zero();

	float Magnitude() const;

	void Normalize();

	Vector3 Normalized();


	// Vector3Å®VECTORÇ÷ïœä∑
	VECTOR V_Cast();
};

Vector3 operator+(const Vector3& lv, const Vector3 rv);
Vector3 operator-(const Vector3& lv, const Vector3 rv);
Vector3 operator-(const Vector3& lv, const float rv);
Vector3 operator-(const Vector3& lv);
Vector3 operator*(const Vector3& lv, const float rv);
Vector3 operator*(const float lv, const Vector3 rv);
Vector3 operator/(const Vector3& lv, const float rv);
Vector3 operator/(const Vector3& lv, const Vector3& rv);

float Dot(const Vector3& v1, const Vector3& v2);
Vector3 Cross(const Vector3& v1, const Vector3& v2);

typedef Vector3 Position3;

struct Size {
	Size();
	Size(int inx, int iny, int inz);
	int width;
	int height;
	int depth;
};

struct Rect {
	Position3 center;
	Size size;
	Rect();
	Rect(int x, int y, int z, int w, int h, int d);
	Rect(Position3& p, Size& s);
	const int Left()const;
	const int Right()const;
	const int Top()const;
	const int Bottom()const;
	const int Width()const { return size.width; }
	const int Height()const { return size.height; }
	const int Depth()const { return size.depth; }
};
