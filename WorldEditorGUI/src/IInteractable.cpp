#include "IInteractable.h"
#include "Defines.h"


bool IInteractable::IsAtTile(int x, int y)
{
	if (x <= m_x && x + SCALED_SPRITE_WIDTH > m_x && y <= m_y && x + SCALED_SPRITE_HEIGHT > m_y)
	{
		return true;
	}

	return false;
}

uint32_t IInteractable::GetInteractableId() const
{
	return m_interactable_id;
}
