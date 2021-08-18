#pragma once

//#include "PlayerWalker.h"
#include <string>
#include <memory>

class Location;
class PlayerWalker;

class EntranceWaypoint // public class Waypoint
{
public:
	EntranceWaypoint(const std::string& loc_name, const std::string& waypoint_name, int x, int y);

	void OnEnter(PlayerWalker& player, std::shared_ptr<Location>& entered_loc, int& loc_x, int& loc_y);

	std::string GetWaypointLocation() const;
	std::string GetWaypointName() const;
	void AssociateLocation(std::shared_ptr<Location> entrance_location);

private:
	int m_x, m_y;
	std::string m_loc_name;
	std::string m_waypoint_name;
	std::shared_ptr<Location> m_entrance_location;
	// virtual OnTeleport() ?
};