#pragma once
#include <Actor/Actor.h>

class Player : public Platformer::Actor
{
	TYPE_DECLARATIONS(Player, Actor)

public:
	Player(const Platformer::Vector2& position);

	virtual void ChangeColor() override;
	
private:
	virtual void Tick(float deltaTime) override;
	void Move(float direction, float deltaTime);

	void CheckCameraView();

	void RequestChangeColor();

private:
	float posX = 0.f;
	float posY = 0.f;

	const float moveSpeed = 20.f;

	bool isFirstJumping = false;
	bool isJumping = false;
	const float jumpSpeed = 24.5f;
	const float secondJumpSpeed = 24.5f;
	float velocityY = 0.f;
	float gravity = 100.0f;
};

