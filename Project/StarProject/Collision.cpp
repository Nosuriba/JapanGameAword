#include "Collision.h"
#include <cmath>

Collision::Collision()
{
}

Collision::~Collision()
{
}

void Collision::Update()
{
}

bool Collision::SquareToSquare(Position2& posA, Size& sizeA, Position2& posB, Size& sizeB)
{
	return ((abs(posA.x - posB.x) < sizeA.width + sizeB.height) && abs(posA.y - posB.y) < sizeA.height + sizeB.height);
}
