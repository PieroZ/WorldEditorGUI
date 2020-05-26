#pragma once
#include <stdint.h>

class IInteractable
{
public:
	virtual void Interact() = 0;
	virtual bool IsAtTile(int x, int y);
	uint32_t GetInteractableId() const;

public:
	int m_x, m_y;
	uint32_t  m_interactable_id;
	bool m_is_dialogue = false;
};