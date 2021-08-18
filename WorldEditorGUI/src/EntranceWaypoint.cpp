#include "EntranceWaypoint.h"
#include "Location.h"
#include "PlayerWalker.h"

EntranceWaypoint::EntranceWaypoint(const std::string& loc_name, const std::string& waypoint_name, int x, int y) : m_loc_name(loc_name), m_waypoint_name(waypoint_name), m_x(x), m_y(y)
{
}

void EntranceWaypoint::OnEnter(PlayerWalker& player, std::shared_ptr<Location>& entered_loc, int& loc_x, int& loc_y)
{
	// Set below after fade out?
	
	
	/*player.SetLocation(m_entrance_location);
	player.SetPosition(m_x, m_y);*/

	entered_loc = m_entrance_location;
	loc_x = m_x;
	loc_y = m_y;


	// Teleport player to new location
	// Change players x & y
	// Change current location to new location
}

std::string EntranceWaypoint::GetWaypointLocation() const
{
	return m_loc_name;
}

std::string EntranceWaypoint::GetWaypointName() const
{
	return m_waypoint_name;
}

void EntranceWaypoint::AssociateLocation(std::shared_ptr<Location> entrance_location)
{
	m_entrance_location = entrance_location;
}
