#pragma once
#include <Actor/Actor.h>

class Obstacle : public Platformer::Actor
{
	TYPE_DECLARATIONS(Obstacle, Platformer::Actor)

public:
	Obstacle(const std::string& image, const Platformer::Vector2& position);
};

