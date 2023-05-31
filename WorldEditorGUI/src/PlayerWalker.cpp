#include "PlayerWalker.h"
#include <stdio.h>

//PlayerWalker::PlayerWalker() : m_x(0), m_y(0), m_dir(direction::DOWN)
PlayerWalker::PlayerWalker(std::shared_ptr<Location> loc) : EntityWalker(loc, 0, 0, 0, 0, 0)//, m_dir(direction::DOWN)
{
	InitBorders();
	m_dir = Direction::DOWN;
	printf("Empty player const!\n");
}

//PlayerWalker::PlayerWalker(int x, int y) : m_x(x), m_y(y), m_dir(direction::DOWN)
PlayerWalker::PlayerWalker( std::shared_ptr<Location> loc, int x, int y) : EntityWalker(loc, x, y, 0, 0, 0)//, m_dir(direction::DOWN)
{
	InitBorders();
	m_dir = Direction::DOWN;
	player_anim.MaxFrames = 3;
	player_anim.SetFrameRate(150);
	Col_X = 7;
	Col_Y = 26;
	Col_Width = 72;
	Col_Height = 139;
}

PlayerWalker::PlayerWalker(int x, int y) : EntityWalker(x, y, 0, 0, 0)//, m_dir(Direction::DOWN)
{
	InitBorders();
	m_dir = Direction::DOWN;
	player_anim.MaxFrames = 3;
	player_anim.SetFrameRate(150);
	Col_X = 7;
	Col_Y = 76;
	Col_Width = 72;
	Col_Height = 80; // 139
}


void PlayerWalker::OnLoop()
{

}

void PlayerWalker::Move(int x, int y, WorldObjects& w_obj)
{
	m_moving = true;
	player_anim.OnAnimate();
	SetPlayerDir(x, y);
	if (w_obj.IsPosValid(this, m_x + x, m_y + y, m_occupied_location))
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
	int y = DirToSpriteId(); // lewo, prawo, gora
	return SDL_Point{ 0, y };
}

// Should be unused
unsigned int PlayerWalker::IsPlayerAtEdgePosition()
{
	return 0;
	//return m_current_location.IsPlayerAtEdgePosition(m_x, m_y);
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
		m_dir = Direction::RIGHT;
	}
	else if (x < 0)
	{
		m_dir = Direction::LEFT;
	}

	else if (y > 0)
	{
		m_dir = Direction::DOWN;
	}
	else if (y < 0)
	{
		m_dir = Direction::UP;
	}
}

bool PlayerWalker::Isintractable()
{
	return false;
}

int PlayerWalker::DirToSpriteId()
{
	int result = -1;

	if (m_dir == Direction::DOWN)
	{
		result = 0;
	}
	else if (m_dir == Direction::LEFT)
	{
		result = 1;
	}
	else if (m_dir == Direction::RIGHT)
	{
		result = 2;
	}
	else if (m_dir == Direction::UP)
	{
		result = 3;
	}
	return result;
}

void PlayerWalker::InitBorders()
{
	m_left_border = false;
	m_top_border = false;
	m_bottom_border = false;
	m_right_border = false;
}

void PlayerWalker::CheckBorder()
{
}

void PlayerWalker::Interact(App* app)
{
}

