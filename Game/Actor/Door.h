#pragma once
#include <Actor/Actor.h>

class Door : public Platformer::Actor
{
	TYPE_DECLARATIONS(Door, Actor)

public:
	Door(const Platformer::Vector2& position);

	virtual void Draw(int screenX = 0) override;

	virtual bool CheckActorPosition(const Platformer::Vector2& position) override;

private:
	int height = 0;
};

