#pragma once
class LocationHelper
{
	LocationHelper();
public:
	LocationHelper(LocationHelper const&) = delete;
	void operator=(LocationHelper const&) = delete;

	static LocationHelper& get();

	int GetIdBasedOnXY(const int x, const int y, const int tiles_per_col);

	void GetXYBasedOnId(const int ID, const int tiles_per_col, int& x, int& y);
};

