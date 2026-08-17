#pragma once
#include <Math/Vector2.h>
#include <Core/Core.h>

namespace Platformer
{
	struct PlatformConfig
	{
		Vector2 basePosition = Vector2::Zero;
		Vector2 direction = Vector2::Zero;
		int distance = 0;
		int speed = 0;
		bool isLooping = false;
		int autoColor = 0;
	};

	class PLATFORMER_API MovableObject
	{
	public:
		MovableObject();
		virtual ~MovableObject();

		virtual void SetMovableConfig(PlatformConfig config) = 0;

	protected:
		virtual void Moving(float deltaTime) = 0;
		virtual void CheckMovingDirection() = 0;
	};
}