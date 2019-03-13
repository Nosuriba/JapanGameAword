#include "Geometry.h"
#include <cmath>

void Vector3::Zero()
{
	x = y = z = 0;
}

float Vector3::Magnitude() const
{
	return sqrt(x * x + y * y + z * z);
}

void Vector3::Normalize()
{
	float m = Magnitude();
	x = x / m;
	y = y / m;
	z = z / m;
}

Vector3 Vector3::Normalized()
{
	float size = Magnitude();
	return Vector3(x / size, y / size, z / size);
}

VECTOR Vector3::V_Cast()
{
	return VGet(x, y, z);
}

Vector3 operator+(const Vector3 & lv, const Vector3 rv)
{
	return Vector3(lv.x + rv.x, lv.y + rv.y, lv.z + rv.z);
}

Vector3 operator-(const Vector3 & lv, const Vector3 rv)
{
	return Vector3(lv.x - rv.x, lv.y - rv.y, lv.z - rv.z);
}

Vector3 operator-(const Vector3 & lv, const float rv) {
	return Vector3(lv.x - rv, lv.y - rv, lv.z - rv);
}

Vector3 operator-(const Vector3 & lv)
{
	return Vector3(-lv.x, -lv.y, -lv.z);
}

Vector3 operator*(const Vector3 & lv, const float rv)
{
	return Vector3(lv.x * rv, lv.y * rv, lv.z * rv);
}

Vector3 operator*(const float lv, const Vector3 rv)
{
	return Vector3(lv * rv.x, lv * rv.y, lv * rv.z);
}

Vector3 operator/(const Vector3 & lv, const float rv)
{
	return Vector3(lv.x / rv, lv.y / rv, lv.z / rv);
}

Vector3 operator/(const Vector3 & lv, const Vector3 & rv) {
	return Vector3(lv.x / rv.x, lv.y / rv.y, lv.z / rv.z);
}

float Dot(const Vector3 & v1, const Vector3 & v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 Cross(const Vector3 & v1, const Vector3 & v2)
{
	return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

//‚ ‚½‚è
Size::Size() : width(0), height(0), depth(0) {}
Size::Size(int inx, int iny, int inz) : width(inx), height(iny), depth(inz) {}

Rect::Rect() : center(0, 0, 0), size(0, 0, 0) {}
Rect::Rect(int x, int y, int z, int w, int h, int d) {}
Rect::Rect(Position3& p, Size & s) : center(p), size(s) {}

const int Rect::Left() const
{
	return (center.x - size.width / 2);
}

const int Rect::Right() const
{
	return (center.x + size.width / 2);
}

const int Rect::Top() const
{
	return (center.y - size.height / 2);
}

const int Rect::Bottom() const
{
	return (center.y + size.height / 2);
}

