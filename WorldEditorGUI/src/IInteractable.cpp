#include "IInteractable.h"
#include "Defines.h"


IInteractable::IInteractable()
{
	m_location_camera = nullptr;
	static int counter = 0;
	m_x = m_y = 0;
	m_interactable_id = counter;
	counter++;
}

IInteractable::IInteractable(std::shared_ptr<Location> occupied_location) : m_occupied_location(occupied_location)
{
}

bool IInteractable::IsAtTile(int x, int y)
{
	if (x <= m_x && x + SCALED_SPRITE_WIDTH > m_x && y <= m_y && y + SCALED_SPRITE_HEIGHT > m_y)
	{
		return true;
	}

	return false;
}

bool IInteractable::IsWithinRange(int x, int y, int w, int h)
{
	if (m_x >= x && m_x <= x + w && m_y >= y && m_y <= y + h)
	{
		return true;
	}
	return false;
}

uint32_t IInteractable::GetInteractableId() const
{
	return m_interactable_id;
}

void IInteractable::SetCamera(Camera* location_camera)
{
	m_location_camera = location_camera;
}

bool IInteractable::IsRotatable() const
{
	return false;
}

// TODO:
// Move to Interactable probably
void IInteractable::SetLocation(std::shared_ptr<Location> occupied_loc)
{
	// ? :D
	//m_world.SetLocation(occupied_loc);
	m_occupied_location = occupied_loc;
}

std::shared_ptr<Location> IInteractable::GetOccupiedLocation()
{
	return m_occupied_location;
}