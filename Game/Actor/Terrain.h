#pragma once
#include <Actor\Actor.h>
#include <Core/PlatformerObject.h>

class Terrain : public Platformer::Actor
{
	TYPE_DECLARATIONS(Terrain, Actor)

public:
	Terrain(const std::string& image, const Platformer::Vector2& position, Platformer::Color color = Platformer::Color::White);
};

