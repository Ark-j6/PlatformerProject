#pragma once
#include <Actor\Actor.h>

class Platform : public Platformer::Actor
{
	TYPE_DECLARATIONS(Platform, Actor)

public:
	Platform(const Platformer::Vector2& position);
};

