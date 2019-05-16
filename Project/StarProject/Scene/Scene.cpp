#include "Scene.h"
#include "../Particle/Bubble.h"

constexpr int BubbleNum = 500;
const Vector2 BubbleSeed = Vector2(600,850);

std::unique_ptr<Particle> Scene::FadeBubble = std::make_unique<Bubble>(BubbleSeed.x, BubbleSeed.y, BubbleNum);


Scene::Scene()
{
}


Scene::~Scene()
{
}
