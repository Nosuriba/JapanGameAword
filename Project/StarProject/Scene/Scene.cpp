#include "Scene.h"
#include "../Particle/Bubble.h"

constexpr int BubbleNum = 1000;
const Vector2 BubbleSeed = Vector2(640,770);

std::unique_ptr<Particle> Scene::FadeBubble = std::make_unique<Bubble>(BubbleSeed.x, BubbleSeed.y, BubbleNum);


Scene::Scene()
{
}


Scene::~Scene()
{
}
