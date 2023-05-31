#include "LocationHelper.h"
#include "Defines.h"
#include <math.h>

LocationHelper::LocationHelper()
{

}

LocationHelper& LocationHelper::get()
{
	static LocationHelper location_helper;
	return location_helper;
}

// Is ID always positive?
int LocationHelper::GetIdBasedOnXY(const int x, const int y, const int tiles_per_col)
{
	int id = x / SCALED_SPRITE_WIDTH;
	id = id + (tiles_per_col * floor(y / SCALED_SPRITE_HEIGHT));
	
	return id;
}

void LocationHelper::GetXYBasedOnId(const int ID, const int tiles_per_col, int& x, int& y)
{
	x = (ID % tiles_per_col) * SCALED_SPRITE_WIDTH;
	y = (ID / tiles_per_col) * SCALED_SPRITE_HEIGHT;
}
