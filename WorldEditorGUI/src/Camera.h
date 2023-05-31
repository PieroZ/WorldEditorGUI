#pragma once
#include <SDL.h>

class App;

class Camera
{
public:
	Camera(App& app, int x, int y, int right_limit, int bottom_limit);

	void Move(int dx, int dy);
	void MoveX(int dx);
	void MoveY(int dy);
	void SetCameraBasedOnPlayerPos(int p_x, int p_y);
	void SetLimits(int right_limit, int bottom_limit, int tiles_per_col);

	int GetX() const;
	int GetY() const;
	SDL_Point GetTopLeftPos() const;

	void SetX(int x);
	void SetY(int y);

	// Called everytime player moves?
	void Update();

	int GetTopLeftID() const;
	int GetTopRightID() const;
	int GetBottomLeftID() const;
	int GetBottomRightID() const;

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
	int GetColsCountPerWindowWidth() const;

public:

	int half_view_width, half_view_height;

private:

	App& m_app;

	int m_x, m_y, m_right_limit, m_bottom_limit;

	// Camera coordinates
	SDL_Point m_top_left;
	SDL_Point m_top_right;
	SDL_Point m_bottom_right;
	SDL_Point m_bottom_left;

	int m_top_left_id = 0;
	int m_top_right_id = 0;
	int m_bottom_right_id = 0;
	int m_bottom_left_id = 0;

	int TILES_COLS_COUNT_PER_WINDOW_WIDTH = 0;
	int m_tiles_per_col = 0;
};