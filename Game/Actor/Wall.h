#pragma once
#include <Actor\Actor.h>

class Wall : public Platformer::Actor
{
	TYPE_DECLARATIONS(Wall, Actor)

public:
	Wall(const Platformer::Vector2& position);
};

