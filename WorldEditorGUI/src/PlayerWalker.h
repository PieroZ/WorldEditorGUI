#pragma once
#include <SDL.h>
#include "EntityWalker.h"
#include "WorldObjects.h"
#include "CAnimation.h"


class PlayerWalker : public EntityWalker
{
public:
	PlayerWalker();
	PlayerWalker(int x, int y);

	void Move(int x, int y, WorldObjects& w_obj);
	SDL_Point GetSpriteCoordinates();

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

	enum class direction {
		UP,
		DOWN,
		LEFT,
		RIGHT
	} m_dir;

	CAnimation player_anim;
	bool m_moving = false;

	// Inherited via EntityWalker
	virtual void Interact() override;

	//int m_x, m_y;
};