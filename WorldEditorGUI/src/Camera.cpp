#include "Camera.h"
#include "Defines.h"

//Camera::Camera() : m_x(0), m_y(0), m_right_limit(0), m_bottom_limit(0)
//{
//}

Camera::Camera(int x, int y, int right_limit, int bottom_limit) : m_x(x), m_y(y), m_right_limit(right_limit), m_bottom_limit(bottom_limit), half_view_width(SCREEN_WIDTH / 2), half_view_height (SCREEN_HEIGHT/2)
{
	//if (m_x < 0)
	//{
	//	m_x = x;
	//}
	//if (m_y < 0)
	//{
	//	m_y = y;
	//}
	SetCameraBasedOnPlayerPos(x, y);	
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
}

void Camera::SetLimits(int right_limit, int bottom_limit)
{
	m_right_limit = right_limit;
	m_bottom_limit = bottom_limit;
}

int Camera::GetX() const
{
	return m_x;
}

int Camera::GetY() const
{
	return m_y;
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