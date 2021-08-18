#pragma once
#include <SDL.h>
#include "EntityWalker.h"
#include "WorldObjects.h"
#include "CAnimation.h"
#include "Location.h"

class App;

class PlayerWalker : public EntityWalker
{
public:
	PlayerWalker(std::shared_ptr<Location> loc);
	PlayerWalker(std::shared_ptr<Location> loc, int x, int y);
	PlayerWalker(int x, int y);

	void Move(int x, int y, WorldObjects& w_obj);
	SDL_Point GetSpriteCoordinates();
	unsigned int IsPlayerAtEdgePosition();

private:
	void OnLoop();

	void MoveLeft(int x);
	void MoveRight(int x);
	void MoveUp(int y);
	void MoveDown(int y);
	void StopMove();
	

	void SetPlayerDir(int x, int y);
	bool Isintractable();
	int DirToSpriteId();
public:

	bool m_left_border, m_top_border, m_bottom_border, m_right_border;

	void InitBorders();
	void CheckBorder();

	CAnimation player_anim;
	bool m_moving = false;

	// Inherited via EntityWalker
	virtual void Interact() override;
private:
	//Location& m_current_location;
	//int m_x, m_y;
};