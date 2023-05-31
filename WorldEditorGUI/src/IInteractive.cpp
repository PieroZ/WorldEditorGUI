#include "IInteractive.h"
#include "Defines.h"


IInteractive::IInteractive()
{
	m_location_camera = nullptr;
	static int counter = 0;
	m_x = m_y = 0;
	m_interactable_id = counter;
	counter++;
}

// unused imo
IInteractive::IInteractive(std::shared_ptr<Location> occupied_location) : m_occupied_location(occupied_location)
{	
	m_x = m_y = 0;
}

bool IInteractive::IsAtTile(int x, int y)
{
	if (x <= m_x && x + SCALED_SPRITE_WIDTH > m_x && y <= m_y && y + SCALED_SPRITE_HEIGHT > m_y)
	{
		return true;
	}

	return false;
}

bool IInteractive::IsWithinRange(int x, int y, int w, int h)
{
	if (m_x >= x && m_x <= x + w && m_y >= y && m_y <= y + h)
	{
		return true;
	}
	return false;
}

uint32_t IInteractive::GetInteractableId() const
{
	return m_interactable_id;
}

void IInteractive::SetCamera(Camera* location_camera)
{
	m_location_camera = location_camera;
}

bool IInteractive::IsRotatable() const
{
	return false;
}

// TODO:
// Move to Interactable probably
void IInteractive::SetLocation(std::shared_ptr<Location> occupied_loc)
{
	// ? :D
	//m_world.SetLocation(occupied_loc);
	m_occupied_location = occupied_loc;
}

std::shared_ptr<Location> IInteractive::GetOccupiedLocation()
{
	return m_occupied_location;
}

void IInteractive::GetXY(int& x, int& y)
{
	x = m_x;
	y = m_y;
}
