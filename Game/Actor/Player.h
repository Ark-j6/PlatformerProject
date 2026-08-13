#pragma once
#include <Actor/Actor.h>

class Player : public Platformer::Actor
{
	TYPE_DECLARATIONS(Player, Actor)

public:
	Player(const Platformer::Vector2& position);

	virtual void ChangeColor() override;
	
	inline bool HasKey() const { return hasKey; }
	inline void GetKey() { hasKey = true; }

	inline void UpdatePlayerInput(bool isEnabled) { enableInput = isEnabled; }

private:
	virtual void Tick(float deltaTime) override;
	void Move(float direction, float deltaTime);
	void RequestMove(bool isX, const int next, const float amount);

	void RequestChangeColor();

private:
	bool enableInput = false;

	float posX = 0.f;
	float posY = 0.f;

	const float moveSpeed = 20.f;

	bool isJumping = false;
	const float jumpSpeed = 24.5f;
	float velocityY = 0.f;
	float gravity = 100.0f;

	bool hasKey = false;
};

