#pragma once
#include <Actor/Actor.h>

class Platform;

class Player : public Platformer::Actor
{
	TYPE_DECLARATIONS(Player, Actor)

public:
	Player(const Platformer::Vector2& position);
	Player(const Platformer::Vector2& position, Platformer::Color color);

	virtual void ChangeColor() override;
	
	inline bool HasKey() const { return hasKey; }
	inline void GetKey() { hasKey = true; }

	inline void UpdatePlayerInput(bool isEnabled) { enableInput = isEnabled; }

	void UpdateStandingPlatform(Platform* platform);

	void GoToCheckPoint(const Platformer::Vector2& nextPosition);

private:
	virtual void Tick(float deltaTime) override;

	void ProcessInput(float direction, float deltaTime);

	void CheckPlayerMovement(bool isX, float newValue);

	void RequestChangeColor();

private:
	bool enableInput = true;

	Platform* standingPlatform = nullptr;

	float posX = 0.f;
	float posY = 0.f;

	const float moveSpeed = 20.f;

	bool isJumping = false;
	const float jumpSpeed = 24.5f;
	float velocityY = 0.f;
	float gravity = 100.0f;

	bool hasKey = false;
};

