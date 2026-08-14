#include "DeathEffect.h"
#include <Level/PlatformLevel.h>

using namespace Platformer;

DeathEffect::DeathEffect(const Vector2& position)
{
	this->position = position;

	sequence = {
		{"P", 0.1f, Color::Yellow}
		, {"p", 0.1f, Color::Yellow}
		, {"p", 0.1f, Color::Red}
		, {"*", 0.1f, Color::Red}
		, {".", 1.f, Color::Red}
	};

	image = sequence[0].frame;
	color = sequence[0].color;

	sortingOrder = 10;

	OnInitiated();
}

void DeathEffect::Destroy()
{
	if (!hasExpired)
	{
		if (std::shared_ptr<PlatformLevel> level = Cast<PlatformLevel>(GetOwner()))
		{
			level->RespawnPlayer();
			Actor::Destroy();
		}
	}
}
