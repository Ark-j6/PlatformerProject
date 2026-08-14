#pragma once
#include "Effect.h"

class CheckpointEffect : public Platformer::Effect
{
	TYPE_DECLARATIONS(CheckpointEffect, Effect)

public:
	CheckpointEffect(const Platformer::Vector2& position);
};

