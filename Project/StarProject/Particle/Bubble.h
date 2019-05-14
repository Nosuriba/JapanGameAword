#pragma once
#include "Particle.h"

class Bubble :
	public Particle
{
private:

	void Move();
public:
	Bubble(int _x, int _y, int _Enum);
	~Bubble();

	void Init();

	void Create();

	void Draw();
};

