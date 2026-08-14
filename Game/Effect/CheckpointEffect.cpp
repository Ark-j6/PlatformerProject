#include "CheckpointEffect.h"

using namespace Platformer;

CheckpointEffect::CheckpointEffect(const Vector2& position)
{
	this->position.x = position.x - 10;
	this->position.y = position.y - 3;

	sequence = { {"Save the CheckPoint!", 2.f, Color::Yellow} };

	image = sequence[0].frame;
	color = sequence[0].color;

	sortingOrder = 10;

	OnInitiated();
}
