#include "Scene.h"
#include "../Particle/Bubble.h"

constexpr int BubbleNum = 5000;
const Vector2 BubbleSeed = Vector2(600,850);

std::unique_ptr<Particle> Scene::FadeBubble = std::make_unique<Bubble>(BubbleSeed.x, BubbleSeed.y, BubbleNum,false,10);


Scene::Scene()
{
}


Scene::~Scene()
{
}
