#include "VictoryEffect.h"
#include <Level/PlatformLevel.h>

using namespace Platformer;

VictoryEffect::VictoryEffect(const Vector2& position)
{
	this->position = position;
	this->position.x = position.x - 6;

	sequence = {
		{"S", 0.18f, Color::Yellow},
		{"St", 0.18f, Color::Yellow},
		{"Sta", 0.18f, Color::Yellow},
		{"Stag", 0.18f, Color::Yellow},
		{"Stage", 0.18f, Color::Yellow},
		{"Stage ", 0.18f, Color::Yellow},
		{"Stage C", 0.18f, Color::Yellow},
		{"Stage Cl", 0.18f, Color::Yellow},
		{"Stage Cle", 0.18f, Color::Yellow},
		{"Stage Clea", 0.18f, Color::Yellow},
		{"Stage Clear", 0.18f, Color::Yellow},
		{"Stage Clear!", 0.18f, Color::Yellow},
		{" ", 0.18f, Color::Yellow},
		{"Stage Clear!", 0.25f, Color::Yellow},
		{" ", 0.18f, Color::Yellow},
		{"Stage Clear!", 0.5f, Color::Yellow}
	};

	image = sequence[0].frame;
	color = sequence[0].color;

	sortingOrder = 10;

	OnInitiated();
}

void VictoryEffect::Destroy()
{
	if (!hasExpired)
	{
		if (std::shared_ptr<PlatformLevel> level = Cast<PlatformLevel>(GetOwner()))
		{
			Actor::Destroy();
			level->RequestNextLevel();
		}
	}
}
