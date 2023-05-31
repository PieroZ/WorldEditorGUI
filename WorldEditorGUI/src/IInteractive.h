#pragma once
#include <stdint.h>
#include <memory>
#include <vector>
#include "Camera.h"
#include "Location.h"


class App;

class IInteractive
{
public:
	IInteractive();
	IInteractive(std::shared_ptr< Location> occupied_location);
	
	// MVP 
	virtual void Interact(App* app) = 0;

	// Returns the corresponding id of tile in map coordinates
	virtual int GetCorrespondingTileIds() const = 0;

	virtual bool IsAtTile(int x, int y);
	bool IsWithinRange(int x, int y, int w, int h);
	uint32_t GetInteractableId() const;
	virtual void SetLocation(std::shared_ptr<Location> occupied_loc);
	std::shared_ptr<Location> GetOccupiedLocation();
	void GetXY(int& x, int& y);

public:
	int m_x, m_y;
	bool m_is_dialogue = false;
	void SetCamera(Camera* location_camera);
	virtual bool IsRotatable() const;
protected:
	Camera* m_location_camera;
	uint32_t  m_interactable_id;
	std::shared_ptr< Location> m_occupied_location;
};