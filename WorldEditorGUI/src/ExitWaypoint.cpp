#include "ExitWaypoint.h"
#include "PlayerWalker.h"

ExitWaypoint::ExitWaypoint(int x, int y, int width, int height) : m_x0(x), m_y0(y), m_width(width), m_height(height), m_entrance_waypoint(nullptr)
{
}

ExitWaypoint::ExitWaypoint(const std::string& waypoint_location, const std::string& waypoint_destination, int x, int y, int width, int height) : m_waypoint_location(waypoint_location), m_waypoint_destination(waypoint_destination), m_x0(x), m_y0(y), m_width(width), m_height(height), m_entrance_waypoint(nullptr)
{
}

int ExitWaypoint::GetX0() const
{
	return m_x0;
}

int ExitWaypoint::GetY0() const
{
	return m_y0;
}

int ExitWaypoint::GetWidth() const
{
	return m_width;
}

int ExitWaypoint::GetHeight() const
{
	return m_height;
}

std::string ExitWaypoint::GetWaypointDestination() const
{
	return m_waypoint_destination;
}

std::string ExitWaypoint::GetWaypointLocation() const
{
	return m_waypoint_location;
}

void ExitWaypoint::AssociateEntranceWaypoint(EntranceWaypoint* entrance_waypoint)
{
	m_entrance_waypoint = entrance_waypoint;
}

void ExitWaypoint::OnExit(PlayerWalker& player, std::shared_ptr<Location>& entered_loc, int& loc_x, int& loc_y)
{
	// Trigger exit sequence
	m_entrance_waypoint->OnEnter(player, entered_loc, loc_x, loc_y);
}
