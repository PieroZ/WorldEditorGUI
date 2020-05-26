#pragma once
#include <SDL.h>
#include "Location.h"
#include "IInteractable.h"

class EntityWalker : public IInteractable
{
public:
	static std::vector<EntityWalker*>    m_entity_arr;
public:
	EntityWalker(int x, int y, int id, uint8_t sprite_x, uint8_t sprite_y);
	SDL_Point GetMapCoordinates() const;
	bool Collides(int oX, int oY, int oW, int oH);
	bool IsPosValid(int NewX, int NewY);
	bool PosValidTile(CTile* Tile);
	bool PosValidEntity(EntityWalker* Entity, int NewX, int NewY);

	virtual void OnRender(SDL_Renderer* renderer);
	virtual void DrawBoundingRect(SDL_Renderer* renderer);
public:
	int m_id;
	int Col_X = 0, Col_Y = 0;
	int Col_Width = 0, Col_Height = 0;
	uint8_t m_sprite_x, m_sprite_y;

	// Inherited via IInteractable
	virtual void Interact() override;
};