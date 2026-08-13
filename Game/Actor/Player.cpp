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
	if (standingPlatform == platform)
		return;

	standingPlatform = platform; 
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

	if (standingPlatform)
	{
		float platX = 0;
		float platY = 0;
		standingPlatform->CurrentDelta(platX, platY);

		std::string s = "Delta x - " + std::to_string(platX) + ", DeltaY - " + std::to_string(platY) + "\n";
		OutputDebugStringA(s.c_str());

		float newPosX = posX + platX;
		float newPosY = posY + platY;

		if (std::shared_ptr<PlatformLevel> level = Cast<PlatformLevel>(GetOwner()))
		{
			Vector2 newPosition = GetPosition();
			newPosition.x = static_cast<int>(newPosX);
			newPosition.y = static_cast<int>(newPosY);

			// 해당 위치에 액터가 있을 경우, 이동 가능한 액터인지 검사
			if (Actor* actor = level->GetActorAt(newPosition))
			{
				if (level->CanMove(actor, color))
				{
					posX = newPosX;
					posY = newPosY;
				}

				// 액터가 있으므로 이동 가능 여부와 상관없이 상호작용 검사
				Vector2 direction = newPosition - GetPosition();
				level->HandleInteraction(actor, direction);
			}
			else
			{
				// 액터가 없을 경우 불가능한 지역(래벨 범위 외부)인지 여부만 확인
				if (level->CheckValidXPos(newPosition.x) || level->CheckValidYPos(newPosition.y))
				{
					posX = newPosX;
					posY = newPosY;
				}
			}
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

	// 점프
	if (Input::Get().GetKeyDown(VK_SPACE) && !isJumping)
	{
		isJumping = true;
		velocityY -= jumpSpeed;
		RequestChangeColor();
	}

	velocityY += gravity * deltaTime;
	if (velocityY > 30.f)
	{
		velocityY = 30.f;
	}
	float newPosY = posY + velocityY * deltaTime;
	int next = static_cast<int>(newPosY);
	
	if (std::shared_ptr<PlatformLevel> level = Cast<PlatformLevel>(GetOwner()))
	{
		Vector2 newPosition = GetPosition();
		newPosition.y = next;

		// 해당 위치에 액터가 있을 경우, 이동 가능한 액터인지 검사
		if (Actor* actor = level->GetActorAt(newPosition))
		{
			if (level->CanMove(actor, color))
			{
				posY = newPosY;
			}
			else
			{
				isJumping = false;
				velocityY = 0;
			}

  
      	  	 	// 액터가 있으므로 이동 가능 여부와 상관없이 상호작용 검사
			Vector2 direction = newPosition - GetPosition();
			level->HandleInteraction(actor, direction);
		}
		else
		{
			standingPlatform = nullptr;

			// 액터가 없을 경우 불가능한 지역(래벨 범위 외부)인지 여부만 확인
			if (level->CheckValidYPos(newPosition.y))
			{
				posY = newPosY;
			}
			else
			{
				isJumping = false;
				velocityY = 0;
			}
		}
	}

	std::string s = "posx - " + std::to_string(posX) + ", posY - " + std::to_string(posY) + "\n";
	OutputDebugStringA(s.c_str());

	SetPosition(Vector2(static_cast<int>(posX), static_cast<int>(posY)));
}

void Player::ProcessInput(float direction, float deltaTime)
{
	float amount = direction * moveSpeed * deltaTime;
	float newPosX = posX + amount;
	int next = static_cast<int>(newPosX);

	if (next == GetPosition().x)
	{
		posX = newPosX;
		return;
	}

	if (std::shared_ptr<PlatformLevel> level = Cast<PlatformLevel>(GetOwner()))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x = next;

		// 해당 위치에 액터가 있을 경우, 이동 가능한 액터인지 검사
		if (Actor* actor = level->GetActorAt(newPosition))
		{
			if (level->CanMove(actor, color))
			{
				posX = newPosX;
			}

			// 액터가 있으므로 이동 가능 여부와 상관없이 상호작용 검사
			Vector2 direction = newPosition - GetPosition();
			level->HandleInteraction(actor, direction);
		}
		else
		{
			// 액터가 없을 경우 불가능한 지역(래벨 범위 외부)인지 여부만 확인
			if (level->CheckValidXPos(next))
			{
				posX = newPosX;
			}
		}
	}
}


void Player::legacyMove(float direction, float deltaTime)
{
	float amount = direction * moveSpeed * deltaTime;
	posX += amount;
	int next = static_cast<int>(posX);

	if (next == GetPosition().x)
		return;

	if (std::shared_ptr<PlatformLevel> level = Cast<PlatformLevel>(GetOwner()))
	{
		Vector2 newPosition = GetPosition();
		newPosition.x = next;

		// 해당 위치에 액터가 있을 경우, 이동 가능한 액터인지 검사
		if (Actor* actor = level->GetActorAt(newPosition))
		{
			if (level->CanMove(actor, color))
			{
				SetPosition(newPosition);
			}

			// 액터가 있으므로 이동 가능 여부와 상관없이 상호작용 검사
			Vector2 direction = newPosition - GetPosition();
			level->HandleInteraction(actor, direction);
		}
		else
		{
			// 액터가 없을 경우 불가능한 지역(래벨 범위 외부)인지 여부만 확인
			if (level->CheckValidXPos(next))
			{
				SetPosition(newPosition);
			}
		}
	}

}

void Player::RequestMove(bool isX, const int next, const float amount)
{
	if (std::shared_ptr<PlatformLevel> level = Cast<PlatformLevel>(GetOwner()))
	{
		Vector2 newPosition = GetPosition();
		isX ? newPosition.x = next : newPosition.y = next;

		// 해당 위치에 액터가 있을 경우, 이동 가능한 액터인지 검사
		if (Actor* actor = level->GetActorAt(newPosition))
		{
			if (level->CanMove(actor, color))
			{
				//isX ? posX += amount : posY += amount;
				if (!isX) posY = amount;
				SetPosition(newPosition);
			}
			else
			{
				if (isX)
				{ 
					posX -= amount;
				}
				else
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
 			if (isX ? level->CheckValidXPos(next) : level->CheckValidYPos(next))
			{
				//isX ? posX += amount : posY += amount;
				if (!isX) posY = amount;
				SetPosition(newPosition);
			}
			else
			{
				if (isX)
				{
					posX -= amount;
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
