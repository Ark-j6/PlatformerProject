#pragma once
#include "Effect.h"

class DeathEffect : public Platformer:: Effect
{
	TYPE_DECLARATIONS(DeathEffect, Effect)

public:
	DeathEffect(const Platformer::Vector2& position);
	~DeathEffect() = default;

private:
	virtual void Destroy() override;
};

