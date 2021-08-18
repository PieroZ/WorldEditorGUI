#pragma once
#include "EntranceWaypoint.h"

class PlayerWalker;

class ExitWaypoint
{
public:
	ExitWaypoint(int x, int y, int width, int height);
	ExitWaypoint(const std::string& waypoint_location, const std::string& waypoint_destination, int x, int y, int width, int height);
		
	int GetX0() const;
	int GetY0() const;
	int GetWidth() const;
	int GetHeight() const;
	std::string GetWaypointDestination() const;
	std::string GetWaypointLocation() const;

	void AssociateEntranceWaypoint(EntranceWaypoint* entrance_waypoint);

	void OnExit(PlayerWalker& player, std::shared_ptr<Location>& entered_loc, int& loc_x, int& loc_y);
private:
	int m_x0, m_y0, m_width, m_height;
	std::string m_waypoint_destination, m_waypoint_location; // Redundant ?
	EntranceWaypoint* m_entrance_waypoint;
};