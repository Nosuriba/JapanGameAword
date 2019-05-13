#pragma once
#include "Processing/Geometry.h"

class Camera
{
private:
	Vector2 _pos;
	Vector2 _range;

	Vector2 _correction;

public:
	Camera();
	~Camera();

	void Update(const Vector2&);
	void Draw();

	// ï\é¶ï‚ê≥
	const Vector2 CameraCorrection() const;

	const Vector2 GetRange() const;
};

