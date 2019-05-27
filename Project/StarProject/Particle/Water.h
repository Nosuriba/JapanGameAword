#pragma once
#include "Particle.h"

class Camera;

class Water :
	public Particle
{
private:
	void Move();
	std::shared_ptr<Camera>camera;
public:
	Water(int _x, int _y, int _Enum, const std::shared_ptr<Camera>& c);
	~Water();

	void Init();

	void Create();

	void Draw(int color = -1);
	void Draw() { Draw(-1); };
};

