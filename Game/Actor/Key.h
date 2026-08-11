#pragma once
#include <Actor/Actor.h>

class Key : public Platformer::Actor
{
	TYPE_DECLARATIONS(Key, Actor)

public:
	Key(const Platformer::Vector2& position);
};

