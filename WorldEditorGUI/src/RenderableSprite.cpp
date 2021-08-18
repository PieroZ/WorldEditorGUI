#include "RenderableSprite.h"

void RenderableSprite::SetSpritesheetName(const std::string& sprite_sheet_name)
{
	m_sprite_sheet_name = sprite_sheet_name;
}

void RenderableSprite::SetSpriteCoords(int x, int y)
{
	m_sprite_x = x;
	m_sprite_y = y;

	m_base_sprite_x = m_sprite_x;
	m_base_sprite_y = m_sprite_y;
}

void RenderableSprite::SetSpriteWidth(int width)
{
	m_sprite_width = width;
}

void RenderableSprite::SetSpriteHeight(int height)
{
	m_sprite_height = height;
}

std::string RenderableSprite::GetSpritesheetName() const
{
	return m_sprite_sheet_name;
}

int RenderableSprite::GetSpriteX() const
{
	return m_sprite_x;
}

int RenderableSprite::GetSpriteY() const
{
	return m_sprite_y;
}

int RenderableSprite::GetBaseSpriteX() const
{
	return m_base_sprite_x;
}

int RenderableSprite::GetBaseSpriteY() const
{
	return m_base_sprite_y;
}

int RenderableSprite::GetSpriteWidth() const
{
	return m_sprite_width;
}

int RenderableSprite::GetSpriteHeight() const
{
	return m_sprite_height;
}
