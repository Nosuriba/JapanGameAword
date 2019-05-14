#include "Scene.h"
#include "../Particle/Bubble.h"

std::unique_ptr<Particle> Scene::FadeBubble = std::make_unique<Bubble>(640,770,1000);


Scene::Scene()
{
}


Scene::~Scene()
{
}
