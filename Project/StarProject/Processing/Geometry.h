#pragma once
#include <DxLib.h>

struct Vector2 {
	Vector2() :x(0), y(0) {}
	Vector2(float inx, float iny) : x(inx), y(iny) {}
	float x, y;
	void operator=(const Vector2& in) {
		x = in.x;
		y = in.y;
	}
	void operator=(const VECTOR& in) {
		x = in.x;
		y = in.y;
	}
	void operator+=(const Vector2& in) {
		x += in.x;
		y += in.y;
	}
	void operator-=(const Vector2& in) {
		x -= in.x;
		y -= in.y;
	}
	void operator*=(const float& in) {
		x *= in;
		y *= in;
	}
	void operator/=(const float& in) {
		x /= in;
		y /= in;
	}

	void Zero();

	float Magnitude() const;

	void Normalize();

	const Vector2 Normalized() const;

	// Vector3Å®VECTORÇ÷ïœä∑
	const VECTOR V_Cast() const;
};

Vector2 operator+(const Vector2& lv, const Vector2 rv);
Vector2 operator-(const Vector2& lv, const Vector2 rv);
Vector2 operator-(const Vector2& lv, const float rv);
Vector2 operator-(const Vector2& lv);
Vector2 operator*(const Vector2& lv, const float rv);
Vector2 operator*(const float lv, const Vector2 rv);
Vector2 operator/(const Vector2& lv, const float rv);
Vector2 operator/(const Vector2& lv, const Vector2& rv);

float Dot(const Vector2& v1, const Vector2& v2);
float Cross(const Vector2& v1, const Vector2& v2);
//Vector2 Cross(const Vector2& v1, const Vector2& v2);

typedef Vector2 Position2;

struct Size {
	Size();
	Size(int inx, int iny);
	int width;
	int height;
};

struct Rect {
	Position2 center;
	Size size;
	Rect();
	Rect(int x, int y, int w, int h);
	Rect(Position2& p, Size& s);
	const int Left()const;
	const int Right()const;
	const int Top()const;
	const int Bottom()const;
	const int Width()const { return size.width; }
	const int Height()const { return size.height; }
};
