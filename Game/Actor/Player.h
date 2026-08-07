#pragma once
#include <Actor/Actor.h>

class Player : public Platformer::Actor
{
	TYPE_DECLARATIONS(Player, Actor)

public:
	Player(const Platformer::Vector2& position);

private:
	virtual void Tick(float deltaTime) override;
};

