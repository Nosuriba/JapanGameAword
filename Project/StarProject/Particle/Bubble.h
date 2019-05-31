#pragma once
#include "Particle.h"
class Camera;
class Bubble :
	public Particle
{
private:
	void Move();
	bool isSmall,flag;
	int imgBff;
	int v_Speed;
	const std::shared_ptr<Camera>c;
	const int BubbleMax, color;
public:
	Bubble(int _x, int _y, int _Enum, bool _isSmall = false, int BubbleMax = 100, int color = -1);
	Bubble(int _x, int _y, int _Enum, bool _isSmall = false, bool flag = false,int vs = 1,int BubbleMax = 100, int color = -1, const std::shared_ptr<Camera>& c=nullptr);
	~Bubble();

	void Init();

	void Create();
	void Create(int _x,int _y);

	void Draw();
};

