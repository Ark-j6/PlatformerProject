#pragma once
#include <Actor/Actor.h>
#include <Actor/Movable.h>

class Platform : public Platformer::Actor, public Platformer::MovableObject
{
	TYPE_DECLARATIONS(Platform, Platformer::Actor)

public:
	Platform(const Platformer::Vector2& position, Platformer::Color color, bool isChangable = false, bool isMovable = false);
	~Platform();

	virtual void Tick(float deltaTime) override;

	virtual bool CheckActorPosition(const Platformer::Vector2& position) override;

	inline void ChangeWidth(unsigned short newWidth)
	{
		width = newWidth;
		image = "";
		for (int i = 0; i < newWidth; ++i)
		{
			image += '-';
		}
	}

	virtual void ChangeColor() override;

	virtual void SetMovableConfig(Platformer::PlatformConfig config) override;

	void CurrentDelta(float& x, float& y) const {	x = prevX; y = prevY; }

protected:
	virtual void Moving(float deltaTime) override;
	virtual void CheckMovingDirection() override;

private:
	Platformer::PlatformConfig config;

	Platformer::Vector2 currentDirection = Platformer::Vector2::Zero;

	float prevX = 0;
	float prevY = 0;

	float movingX = 0;
	float movingY = 0;
	float movingDistance = 0;

	bool isWait = false;
	float waitTimer = 0;
	const float waitDuration = 0.2f;

public:
	bool isColorChangable = false;
	bool isMovable = false;
};

