#include "Platform.h"

using namespace Platformer;

Platform::Platform(const Vector2& position, Color color, bool isChangable, bool isMovable)
	: Actor("-", position, color), isColorChangable(isChangable), isMovable(isMovable)
{
	sortingOrder = 3;
	movingX = static_cast<float>(position.x);
	movingY = static_cast<float>(position.y);
}

Platform::~Platform()
{
}

void Platform::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!isMovable)
		return;

	if (isWait)
	{
		waitTimer += deltaTime;

		if (waitTimer >= waitDuration)
		{
			isWait = false;
			return;
		}
	}

	Moving(deltaTime);

	CheckMovingDirection();
}

bool Platform::CheckActorPosition(const Vector2& position)
{
	int maxX = GetPosition().x + width;
	bool isX = GetPosition().x <= position.x && position.x <= maxX;

	return isX && position.y == GetPosition().y;
}

void Platform::ChangeColor()
{
	if (isColorChangable)
	{
		switch (color)
		{
		case Color::White:
			color = Color::Red;
			break;
		case Color::Red:
			color = Color::Green;
			break;
		case Color::Green:
			color = Color::Blue;
			break;
		case Color::Blue:
			color = Color::White;
			break;
		default:
			break;
		}
	}
}

void Platform::SetMovableConfig(PlatformConfig config)
{
	this->config = config;
	currentDirection = config.direction;
}

void Platform::Moving(float deltaTime)
{
	float interval = config.speed * deltaTime;

	prevX = currentDirection.x * interval;
	prevY = currentDirection.y * interval;

	movingX += prevX;
	movingY += prevY;

	int newX = static_cast<int>(movingX);
	int newY = static_cast<int>(movingY);


	SetPosition(Vector2(newX, newY));

	movingDistance += interval;
}

void Platform::CheckMovingDirection()
{
	if (movingDistance >= static_cast<float>(config.distance))
	{
		movingDistance = 0.f;

		if (config.isLooping)
		{
			waitTimer = 0.f;
			isWait = true;

			currentDirection.x = currentDirection.x * -1;
			currentDirection.y = currentDirection.y * -1;
		}
		else
		{
			isMovable = false;
		}
	}
}
