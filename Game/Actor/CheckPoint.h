#pragma once
#include <Actor\Actor.h>

class CheckPoint : public Platformer::Actor
{
	TYPE_DECLARATIONS(CheckPoint, Actor)

public:
	CheckPoint(const Platformer::Vector2& position);
};

