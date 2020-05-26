#pragma once
#include "EntityWalker.h"
#include "sol.hpp"

class NPC : public EntityWalker
{
public:
	NPC(int x, int y, int id, int sprite_x, int sprite_y);
	void OnRender(SDL_Renderer* renderer) override;
	void DrawBoundingRect(SDL_Renderer* renderer) override;
	std::string Talk(bool& last_line);

	sol::state lua;
	bool dialogue_exists = false;


	// Inherited via IInteractable
	virtual void Interact() override;

};

