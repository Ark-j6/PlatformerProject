#pragma once
#include <Actor\Actor.h>

class DeadZone : public Platformer::Actor
{
	TYPE_DECLARATIONS(DeadZone, Actor)

public:
	DeadZone(const Platformer::Vector2& position);
};

