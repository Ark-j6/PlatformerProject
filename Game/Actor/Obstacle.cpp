#include "Obstacle.h"

using namespace Platformer;

Obstacle::Obstacle(const std::string& image, const Vector2& position) : Actor(image, position, Color::Cyan)
{
	sortingOrder = 4;
}
