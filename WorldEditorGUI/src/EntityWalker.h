#pragma once
#include <SDL.h>
#include "Location.h"
#include "IInteractable.h"
#include "RenderableSprite.h"

class App;

class EntityWalker : public IInteractable, public RenderableSprite
{
public:

	enum class Direction {
		UP,
		DOWN,
		LEFT,
		RIGHT
	} m_dir;

public:
	static std::vector<EntityWalker*>    m_entity_arr;

	// Reference to world
	//App& m_world;
public:
	EntityWalker(std::shared_ptr<Location> occupied_location, int x, int y, int id, uint8_t sprite_x, uint8_t sprite_y);
	EntityWalker(int x, int y, int id, uint8_t sprite_x, uint8_t sprite_y);
	EntityWalker();
	SDL_Point GetMapCoordinates() const;
	void SetPosition(int x, int y);
	bool Collides(int oX, int oY, int oW, int oH);
	bool IsPosValid(int NewX, int NewY);
	bool PosValidTile(CTile* Tile);
	bool PosValidEntity(EntityWalker* Entity, int NewX, int NewY);
	Direction GetDir() const;

	virtual void OnRender(SDL_Renderer* renderer);
	virtual void DrawBoundingRect(SDL_Renderer* renderer);
public:

	int m_id;
	int Col_X = 0, Col_Y = 0;
	int Col_Width = 0, Col_Height = 0;
	
	// OBSOLETE
	//uint8_t m_sprite_x, m_sprite_y;

	// Inherited via IInteractable
	virtual void Interact() override;
};