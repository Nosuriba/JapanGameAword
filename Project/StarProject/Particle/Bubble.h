#pragma once
#include "Particle.h"

class Bubble :
	public Particle
{
private:
	void Move();
	bool isSmall,flag;
	int imgBff;
	int v_Speed;
	const int BubbleMax;
public:
	Bubble(int _x, int _y, int _Enum, bool _isSmall = false, int BubbleMax = 100);
	Bubble(int _x, int _y, int _Enum, bool _isSmall = false, bool flag = false,int vs = 1, int BubbleMax = 100);
	~Bubble();

	void Init();

	void Create();
	void Create(int _x,int _y);

	void Draw();
};

