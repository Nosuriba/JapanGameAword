#pragma once
#include "Particle.h"
class Water :
	public Particle
{
private:
	void Move();

public:
	Water(int _x, int _y, int _Enum,float _Rota=0);
	~Water();

	void Init();

	void Create();

	void Draw();
};

