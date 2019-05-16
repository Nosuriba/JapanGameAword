#include "Particle/Particle.h"

const int Particle::screen_x = 1200;
const int Particle::screen_y = 800;

Particle::~Particle()
{
	if (p_thread.joinable())
	{
		p_thread.join();
	}
}
