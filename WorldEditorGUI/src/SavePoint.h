#pragma once

#include "IInteractable.h"

class SavePoint : public IInteractable
{
public:
	// Inherited via IInteractable
	virtual void Interact() override;
};
