#pragma once
#include "Effect.h"

class VictoryEffect : public Platformer::Effect
{
	TYPE_DECLARATIONS(VictoryEffect, Effect)

public:
	VictoryEffect(const Platformer::Vector2& position);
	~VictoryEffect() = default;

private:
	virtual void Destroy() override;
};

