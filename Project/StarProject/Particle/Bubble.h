#pragma once
#include "Particle.h"

class Bubble :
	public Particle
{
private:
	void Move();
	bool isSmall;
	int imgBff;
	const int BubbleMax;
public:
	Bubble(int _x, int _y, int _Enum,bool flag = false,int BubbleMax = 100);
	~Bubble();

	void Init();

	void Create();
	void Create(int _x,int _y);

	void Draw();
};

