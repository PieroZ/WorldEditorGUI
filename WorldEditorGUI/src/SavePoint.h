#pragma once

#include "IInteractive.h"
#include "CTile.h"
#include <memory>

class SavePoint : public IInteractive, public CTile
{
public:
	SavePoint();
	SavePoint(int tile_spritesheet_x, int tile_spritesheet_y, int tile_id, int type_id, const SDL_Rect& collider);
	//SavePoint(std::shared_ptr<CTile> tile);

	// Inherited via IInteractive
	virtual void Interact(App* app) override;

	// Inherited via IInteractive
	virtual int GetCorrespondingTileIds() const override;
};
