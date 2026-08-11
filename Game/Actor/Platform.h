#pragma once
#include <Actor\Terrain.h>

class Platform : public Terrain
{
	TYPE_DECLARATIONS(Platform, Terrain)

public:
	Platform(const Platformer::Vector2& position, Platformer::Color color, bool isChangable = false, bool isMovable = false);

	inline void ChangeWidth(unsigned short newWidth)
	{
		width = newWidth;
		image = "";
		for (int i = 0; i < newWidth; ++i)
		{
			image += '-';
		}
	}

	virtual void ChangeColor() override;

	void Moving();

public:
	bool isColorChangable = false;
	bool isMovable = false;
};

