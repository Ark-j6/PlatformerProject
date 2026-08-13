#pragma once
#include <Actor/Actor.h>
#include <Actor/Movable.h>

class Obstacle : public Platformer::Actor, public Platformer::MovableObject
{
	TYPE_DECLARATIONS(Obstacle, Platformer::Actor)

public:
	Obstacle(const std::string& image, const Platformer::Vector2& position);
	~Obstacle();

	virtual void Tick(float deltaTime) override;

	virtual void SetMovableConfig(Platformer::PlatformConfig config) override;

	virtual void Moving(float deltaTime) override;

private:
	virtual void CheckMovingDirection() override;

private:
	Platformer::PlatformConfig config;

	Platformer::Vector2 currentDirection = Platformer::Vector2::Zero;
	float movingDistance = 0;
	float movingX = 0;
	float movingY = 0;

public:
	bool isColorChangable = false;
	bool isMovable = false;
};