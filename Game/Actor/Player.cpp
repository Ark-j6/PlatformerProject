#include "Player.h"
#include <Input/Input.h>
#include <Level/PlatformLevel.h>
#include <Actor/Platform.h>

using namespace Platformer;

Player::Player(const Vector2& position) : super("P", position, Color::Green)
{
	sortingOrder = 3;
	posX = static_cast<float>(position.x);
	posY = static_cast<float>(position.y);
}

Player::Player(const Platformer::Vector2& position, Platformer::Color color) : super("P", position, color)
{
	sortingOrder = 3;
	posX = static_cast<float>(position.x);
	posY = static_cast<float>(position.y);
}

void Player::ChangeColor()
{
	switch (color)
	{
	case Color::White:
		color = Color::Red;
		break;
	case Color::Red:
		color = Color::Green;
		break;
	case Color::Green:
		color = Color::Blue;
		break;
	case Color::Blue:
		color = Color::White;
		break;
	case Color::BrightWhite:
	default:
		break;
	}
}

void Player::UpdateStandingPlatform(Platform* platform)
{
	if (standingPlatform == platform || !platform->isMovable)
		return;

	standingPlatform = platform; 
}

void Player::GoToCheckPoint(const Platformer::Vector2& nextPosition)
{
	SetPosition(nextPosition);
	posX = static_cast<float>(position.x);
	posY = static_cast<float>(position.y);
	UpdatePlayerInput(true);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (!enableInput)
		return;

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 종료 처리
		QuitGame();
		return;
	}

	// 이동 플랫폼의 이동량 반영
	if (standingPlatform && !isJumping)
	{
		float platX = 0;
		float platY = 0;
		standingPlatform->GetCurrentPlatformPos(platX, platY);

		if (standingPlatform->currentDirection.x != 0)
		{
			posX += platX;
		}
		else
		{
			posY = platY - 1.0f;
			velocityY = 0.f;
		}
	}

	// 이동 처리
	if (Input::Get().GetKey(VK_RIGHT))
	{
		ProcessInput(1.f, deltaTime);
	}

	if (Input::Get().GetKey(VK_LEFT))
	{
		ProcessInput(-1.f, deltaTime);
	}

	// 이동 결과 플랫폼 밖으로 나갔는지 확인
	if (standingPlatform && !isJumping)
	{
		const int playerX = static_cast<int>(posX);

		const int platformLeft = standingPlatform->GetPosition().x;
		const int platformRignt = platformLeft + standingPlatform->GetImageWidth() - 1;

		if (playerX < platformLeft || playerX > platformRignt)
		{
			standingPlatform = nullptr;
		}
	}


	// 점프
	if (Input::Get().GetKeyDown(VK_SPACE) && !isJumping)
	{
		isJumping = true;
		velocityY -= jumpSpeed;
		standingPlatform = nullptr;
		RequestChangeColor();
	}

	// 중력 적용 -> Y축 이동 여부 결정
	if (!standingPlatform || isJumping)
	{
		velocityY += gravity * deltaTime;
		if (velocityY > 30.f)
		{
			velocityY = 30.f;
		}
		float newPosY = posY + velocityY * deltaTime;

		CheckPlayerMovement(false, newPosY);
	}

	SetPosition(Vector2(static_cast<int>(posX), static_cast<int>(posY)));
}

void Player::ProcessInput(float direction, float deltaTime)
{
	float amount = direction * moveSpeed * deltaTime;
	float newPosX = posX + amount;

	CheckPlayerMovement(true, newPosX);
}

void Player::CheckPlayerMovement(bool isX, float newValue)
{
	int next = static_cast<int>(newValue);
	int current = isX ? GetPosition().x : GetPosition().y;

	if (next == current)
	{
		isX ? posX = newValue : posY = newValue;
		return;
	}

	if (std::shared_ptr<PlatformLevel> level = Cast<PlatformLevel>(GetOwner()))
	{
		Vector2 newPosition = GetPosition();
		isX ? newPosition.x = next : newPosition.y = next;

		// 해당 위치에 액터가 있을 경우, 이동 가능한 액터인지 검사
		if (Actor* actor = level->GetActorAt(newPosition))
		{
			if (level->CanMove(actor, color))
			{
				isX ? posX = newValue : posY = newValue;
			}
			else
			{
				if (!isX)
				{
					isJumping = false;
					velocityY = 0;
				}
			}

			// 액터가 있으므로 이동 가능 여부와 상관없이 상호작용 검사
			Vector2 direction = newPosition - GetPosition();
			level->HandleInteraction(actor, direction);
		}
		else
		{
			// 액터가 없을 경우 불가능한 지역(래벨 범위 외부)인지 여부만 확인
			if (isX)
			{
				if (level->CheckValidXPos(next))
				{
					posX = newValue;
				}
			}
			else
			{
				standingPlatform = nullptr;

				if (level->CheckValidYPos(next))
				{
					posY = newValue;
				}
				else
				{
					isJumping = false;
					velocityY = 0;
				}
			}
		}
	}
}

void Player::RequestChangeColor()
{
	if (std::shared_ptr<PlatformLevel> level = Cast<PlatformLevel>(GetOwner()))
	{
		level->ChangeActorColors();
	}
}
