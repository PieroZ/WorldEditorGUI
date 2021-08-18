#pragma once
#include <string>

class RenderableSprite
{
protected:
	int m_sprite_x{ -1 };
	int m_sprite_y{ -1 };
	int m_base_sprite_x{ -1 };
	int m_base_sprite_y{ -1 };
	int m_sprite_width{ -1 };
	int m_sprite_height{ -1 };

	std::string m_sprite_sheet_name;
public:
	void SetSpritesheetName(const std::string& sprite_sheet_name);
	void SetSpriteCoords(int x, int y);
	void SetSpriteWidth(int width);
	void SetSpriteHeight(int height);
	std::string GetSpritesheetName() const;
	int GetSpriteX() const;
	int GetSpriteY() const;
	int GetBaseSpriteX() const;
	int GetBaseSpriteY() const;
	int GetSpriteWidth() const;
	int GetSpriteHeight() const;
};