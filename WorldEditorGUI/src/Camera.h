#pragma once

class Camera
{
public:
	Camera(int x, int y, int right_limit, int bottom_limit);

	void Move(int dx, int dy);
	void MoveX(int dx);
	void MoveY(int dy);
	void SetCameraBasedOnPlayerPos(int p_x, int p_y);
	void SetLimits(int right_limit, int bottom_limit);

	int GetX() const;
	int GetY() const;

	/*
		Checks if new x, y position is valid.
	*/


	// All below functions take values that are edge camera boundries ( ie. cam_center_x - view_port_width)
	bool IsLeftEdge(int x);
	bool IsRightEdge(int x);
	bool IsTopEdge(int y);
	bool IsBottomEdge(int y);


	bool CanMoveX(int x);
	bool CanMoveY(int y);

public:
	int m_x, m_y, m_right_limit, m_bottom_limit;
	int half_view_width, half_view_height;
};