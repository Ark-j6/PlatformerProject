#include "Obstacle.h"

using namespace Platformer;

Obstacle::Obstacle(const std::string& image, const Vector2& position) : Actor(image, position, Color::Cyan)
{
	sortingOrder = 4;
}

Obstacle::~Obstacle()
{
}

void Obstacle::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!isMovable)
		return;

	Moving(deltaTime);

	CheckMovingDirection();
}

void Obstacle::SetMovableConfig(PlatformConfig config)
{
	this->config = config;
	currentDirection = config.direction;
}

void Obstacle::Moving(float deltaTime)
{
	float interval = config.speed * deltaTime;

	movingX += currentDirection.x * interval;
	movingY += currentDirection.y * interval;

	int newX = static_cast<int>(movingX);
	int newY = static_cast<int>(movingY);


	SetPosition(Vector2(newX, newY));
	movingDistance += interval;
}

void Obstacle::CheckMovingDirection()
{
	if (movingDistance >= static_cast<float>(config.distance))
	{
		movingDistance = 0.f;

		if (config.isLooping)
		{
			currentDirection = currentDirection * -1;
		}
		else
		{
			isMovable = false;
		}
	}
}
