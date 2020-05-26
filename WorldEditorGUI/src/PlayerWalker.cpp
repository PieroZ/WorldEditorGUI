#include "PlayerWalker.h"
#include <stdio.h>

//PlayerWalker::PlayerWalker() : m_x(0), m_y(0), m_dir(direction::DOWN)
PlayerWalker::PlayerWalker() : EntityWalker(0, 0, 0, 0, 0), m_dir(direction::DOWN)
{
	printf("Empty player const!\n");
}

//PlayerWalker::PlayerWalker(int x, int y) : m_x(x), m_y(y), m_dir(direction::DOWN)
PlayerWalker::PlayerWalker(int x, int y) : EntityWalker(x, y, 0, 0, 0), m_dir(direction::DOWN)
{
	player_anim.MaxFrames = 3;
	Col_X = 7;
	Col_Y = 26;
	Col_Width = 72;
	Col_Height = 139;
}

void PlayerWalker::OnLoop()
{

}

void PlayerWalker::Move(int x, int y, WorldObjects& w_obj)
{
	m_moving = true;
	player_anim.OnAnimate();
	SetPlayerDir(x, y);
	if (w_obj.IsPosValid(this, m_x + x, m_y + y))
	{
		if (x > 0)
		{
			MoveRight(x);
		}
		else if (x < 0)
		{
			MoveLeft(x);
		}

		if (y > 0)
		{
			MoveDown(y);
		}
		else if (y < 0)
		{
			MoveUp(y);
		}
	}
}

SDL_Point PlayerWalker::GetSpriteCoordinates()
{
	int y = DirToSpriteId();; // lewo, prawo, gora
	return SDL_Point{ 0, y };
}

void PlayerWalker::MoveLeft(int x)
{
	m_x += x;
	//m_dir = direction::LEFT;
}

void PlayerWalker::MoveRight(int x)
{
	m_x += x;
	//m_dir = direction::RIGHT;
}

void PlayerWalker::MoveUp(int y)
{
	m_y += y;
	//m_dir = direction::UP;
}

void PlayerWalker::MoveDown(int y)
{
	m_y += y;
	//m_dir = direction::DOWN;
}

void PlayerWalker::StopMove()
{
}

void PlayerWalker::SetPlayerDir(int x, int y)
{
	if (x > 0)
	{
		m_dir = direction::RIGHT;
	}
	else if (x < 0)
	{
		m_dir = direction::LEFT;
	}

	else if (y > 0)
	{
		m_dir = direction::DOWN;
	}
	else if (y < 0)
	{
		m_dir = direction::UP;
	}
}

bool PlayerWalker::Isintractable()
{
	return false;
}

int PlayerWalker::DirToSpriteId()
{
	int result = -1;

	if (m_dir == direction::DOWN)
	{
		result = 0;
	}
	else if (m_dir == direction::LEFT)
	{
		result = 1;
	}
	else if (m_dir == direction::RIGHT)
	{
		result = 2;
	}
	else if (m_dir == direction::UP)
	{
		result = 3;
	}
	return result;
}

void PlayerWalker::Interact()
{
}

