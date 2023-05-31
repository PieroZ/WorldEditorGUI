#include "Camera.h"
#include "Defines.h"
#include "LocationHelper.h"
#include "App.h"


//Camera::Camera() : m_x(0), m_y(0), m_right_limit(0), m_bottom_limit(0)
//{
//}

Camera::Camera(App& app, int x, int y, int right_limit, int bottom_limit) :
	m_app(app), m_x(x), m_y(y), m_right_limit(right_limit), m_bottom_limit(bottom_limit), half_view_width(SCREEN_WIDTH / 2), half_view_height (SCREEN_HEIGHT/2)
{
	//if (m_x < 0)
	//{
	//	m_x = x;
	//}
	//if (m_y < 0)
	//{
	//	m_y = y;
	//}
	TILES_COLS_COUNT_PER_WINDOW_WIDTH = SCREEN_WIDTH / SCALED_SPRITE_WIDTH;
	SetCameraBasedOnPlayerPos(x, y);	
	//Update();
}

void Camera::Move(int dx, int dy)
{
	MoveX(dx);
	MoveY(dy);
}

void Camera::MoveX(int dx)
{
	int view_port_offset = 0;
	if (dx > 0)
	{
		view_port_offset = half_view_width;
	}
	else
	{
		view_port_offset = -half_view_width;
	}
	if (CanMoveX(m_x + dx + view_port_offset))
	{
		m_x += dx;
	}
	Update();
}

void Camera::MoveY(int dy)
{
	int view_port_offset = 0;
	if (dy > 0)
	{
		view_port_offset = half_view_height;
	}
	else
	{
		view_port_offset = -half_view_height;
	}
	if (CanMoveY(m_y + dy + view_port_offset))
	{
		m_y += dy;
	}
	Update();
}

void Camera::SetCameraBasedOnPlayerPos(int p_x, int p_y)
{
	if (IsLeftEdge(p_x - half_view_width))
	{
		m_x = SCREEN_WIDTH / 2;
	}
	else if (IsRightEdge(p_x + half_view_width))
	{
		m_x = m_right_limit - SCREEN_WIDTH / 2;
	}
	else
	{
		m_x = p_x;
	}

	if (IsTopEdge(p_y - half_view_height))
	{
		m_y = SCREEN_HEIGHT/ 2;
	}
	else if (IsBottomEdge(p_y + half_view_height))
	{
		m_y = m_bottom_limit - SCREEN_HEIGHT / 2;
	}
	else
	{
		m_y = p_y;
	}

	Update();
}

void Camera::SetLimits(int right_limit, int bottom_limit, int tiles_per_col)
{
	m_right_limit = right_limit;
	m_bottom_limit = bottom_limit;
	m_tiles_per_col = tiles_per_col;
	Update();
}

int Camera::GetX() const
{
	return m_x;
}

int Camera::GetY() const
{
	return m_y;
}

SDL_Point Camera::GetTopLeftPos() const
{
	return m_top_left;
}

void Camera::SetX(int x)
{
	m_x = x;
	Update();
}

void Camera::SetY(int y)
{
	m_y = y;
	Update();
}


// Return true when corner coordinates have changed
void Camera::Update()
{
	int left =  m_x - (SCREEN_WIDTH / 2);
	int right = m_x + (SCREEN_WIDTH / 2 - 1);
	int top = m_y - (SCREEN_HEIGHT / 2);
	int bottom = m_y + (SCREEN_HEIGHT / 2 - 1);

	m_top_left = { left, top };

	m_top_right = { right, top };
	m_bottom_right = { right, bottom };
	m_bottom_left = { left, bottom };



	int top_left_id = floor(LocationHelper::get().GetIdBasedOnXY(m_top_left.x, m_top_left.y, m_tiles_per_col));
	int top_right_id = top_left_id + GetColsCountPerWindowWidth() + 1;
	int bottom_right_id = ceil(LocationHelper::get().GetIdBasedOnXY(m_bottom_right.x, m_bottom_right.y, m_tiles_per_col));
	int bottom_left_id = LocationHelper::get().GetIdBasedOnXY(m_bottom_left.x, m_bottom_left.y, m_tiles_per_col);

	if (	m_top_left_id != top_left_id 
		||  m_top_right_id != top_right_id
		||	m_bottom_right_id != bottom_right_id
		||	m_bottom_left_id != bottom_left_id
		)
	{	
		m_top_left_id = top_left_id;
		m_top_right_id = top_right_id;
		m_bottom_right_id = bottom_right_id;
		m_bottom_left_id = bottom_left_id;

		m_app.UpdateCurrentlyDrawnTilesId();
	}
}

int Camera::GetTopLeftID() const
{
	return m_top_left_id;
}

int Camera::GetTopRightID() const
{
	return m_top_right_id;
}

int Camera::GetBottomLeftID() const
{
	return m_bottom_left_id;
}

int Camera::GetBottomRightID() const
{
	return m_bottom_right_id;
}

bool Camera::IsLeftEdge(int x)
{
	return x <= 0;
}

bool Camera::IsRightEdge(int x)
{
	return x >= (m_right_limit - 1);
}

bool Camera::IsTopEdge(int y)
{
	return y <= 0;
}

bool Camera::IsBottomEdge(int y)
{
	return y >= (m_bottom_limit - 1);
}

bool Camera::CanMoveX(int x)
{
	return (x < m_right_limit - 1) && (x > 0);
}

bool Camera::CanMoveY(int y)
{
	return (y < m_bottom_limit - 1) && (y > 0);
}

int Camera::GetColsCountPerWindowWidth() const
{
	return TILES_COLS_COUNT_PER_WINDOW_WIDTH;
}
