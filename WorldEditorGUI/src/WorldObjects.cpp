#include <filesystem>

#include "WorldObjects.h"
#include "Defines.h"
#include "LuaLoader.h"
#include "NpcLuaLoader.h"
#include "PlayerWalker.h"
#include "ActiveFactStates.h"



WorldObjects::WorldObjects()
{
	//AddNpc(400, 400, 0, 1, 4);
	//AddNpc(1000, 400, 1, 4, 0);
	//AddNpc("res/scripts/npcs/TESTLOC_NPC_10000.lua");
}

WorldObjects::~WorldObjects()
{
}

//void WorldObjects::AddNpc(Location& loc, int x, int y, int id, int sprite_x, int sprite_y)
void WorldObjects::AddNpc(App& world, std::shared_ptr<Location> loc, int x, int y, int id, int sprite_x, int sprite_y)
{
	//auto npc = std::make_shared<NPC>(world, loc, x, y, id, sprite_x, sprite_y);
	auto npc = std::make_shared<NPC>(loc, x, y, id, sprite_x, sprite_y);
	if (npc->dialogue_exists)
	{
		interactable_arr.push_back(npc);
	}
	npc_arr.push_back(npc);
}

// Is this even used ? 
void WorldObjects::AddNpc(const std::string& lua_filepath)
{
	if (!std::filesystem::exists(lua_filepath))
	{
		printf("File %s does not exist ! \n", lua_filepath.c_str());
	}
	else
	{
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);		
		auto ob = lua.script_file(lua_filepath);		
		uint32_t id = (lua["NPC"]["id"]);
		int x = (lua["NPC"]["x"]);
		int y = (lua["NPC"]["y"]);
		int sprite_x = (lua["NPC"]["sprite_x"]);
		int sprite_y = (lua["NPC"]["sprite_y"]);

		// Add Location
		//AddNpc(x, y, id, sprite_x, sprite_y);
	}
}



std::shared_ptr<NPC> WorldObjects::FindNpcById(const uint32_t interactable_id)
{
	for (auto&& npc : npc_arr)
	{
		if (npc->GetInteractableId() == interactable_id)
		{
			return npc;
		}
	}
	return nullptr;
}

bool WorldObjects::IsInteractableAt(int x, int y, bool& is_dialogue, std::shared_ptr<NPC>& interact)
{
	for (auto&& inter : interactable_arr)
	{
		if (inter->IsAtTile(x, y))
		{
			printf("Czy ma dialog  = %d\n", inter->m_is_dialogue);
			printf("Cos na tym klocku stoi !\n");
			is_dialogue = inter->m_is_dialogue;
			int interactable_id = inter->GetInteractableId();
			interact = FindNpcById(interactable_id);
			return true;
		}
	}
	return false;
}

bool WorldObjects::IsInteractableAt(EntityWalker::Direction player_dir, int player_x, int player_y, bool& is_dialogue, std::shared_ptr<NPC>& interact)
{
	int x0_scan = player_x;// + SCALED_HERO_SPRITE_WIDTH / 2;
	int y0_scan = player_y;// + SCALED_HERO_SPRITE_HEIGHT / 2;
	int scan_width = SCALED_SPRITE_WIDTH;
	int scan_height = SCALED_SPRITE_HEIGHT;

	if (player_dir == EntityWalker::Direction::RIGHT)
	{
		//x0_scan = x0_scan + SCALED_HERO_SPRITE_WIDTH;
		scan_width = scan_width + +SCALED_HERO_SPRITE_WIDTH / 2;
	}
	else if (player_dir == EntityWalker::Direction::DOWN)
	{
		scan_height = scan_height + +SCALED_HERO_SPRITE_HEIGHT / 2;
		//y0_scan = y0_scan + SCALED_HERO_SPRITE_HEIGHT;
	}

	for (auto&& inter : interactable_arr)
	{
		if (inter->IsWithinRange(x0_scan, y0_scan, scan_width, scan_height))
		{
			is_dialogue = inter->m_is_dialogue;
			int interactable_id = inter->GetInteractableId();
			interact = FindNpcById(interactable_id);
			return true;
		}
	}

	return false;
}

bool WorldObjects::IsInteractableAt(PlayerWalker& player, bool& is_dialogue, std::shared_ptr<NPC>& interact, std::shared_ptr<Location> loc)
{
	EntityWalker::Direction player_dir = player.GetDir();
	int scan_offset_x = 0;
	int scan_offset_y = 0;

	if (player_dir == EntityWalker::Direction::UP)
	{
		//x0_scan = x0_scan + SCALED_HERO_SPRITE_WIDTH;
		scan_offset_y = SCALED_HERO_SPRITE_HEIGHT/2;
	}
	else if (player_dir == EntityWalker::Direction::LEFT)
	{
		scan_offset_x = SCALED_HERO_SPRITE_WIDTH/2;
		//y0_scan = y0_scan + SCALED_HERO_SPRITE_HEIGHT;
	}

	std::vector<std::shared_ptr<IInteractable>>  interactable_in_loc_arr = interactables_location_map[loc];

	//for (auto&& inter : interactable_arr)
	for (auto&& inter : interactable_in_loc_arr)
	{
		int scan_x = inter->m_x;
		int scan_y = inter->m_y;
		if (player.Collides(inter->m_x + scan_offset_x, inter->m_y + scan_offset_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT))
		{
			is_dialogue = inter->m_is_dialogue;
			int interactable_id = inter->GetInteractableId();
			interact = FindNpcById(interactable_id);
			return true;
		}
	}
	return false;
}


std::string WorldObjects::PlayDialogue(std::shared_ptr<NPC> dialogue_holder, bool& last_line)
{
	// Which dialogue should I play?
	// DialogGUI* gui, DialogControllerState& state, bool& has_said, std::vector<std::string>& choices_lines, int& choice_id, bool& last_line_flag)
	return "HEY HEY";
	//return (dialogue_holder->Talk(last_line));
}

bool WorldObjects::Collides(std::shared_ptr<EntityWalker> ent, int oX, int oY, int oW, int oH)
{
	int left1, left2;
	int right1, right2;
	int top1, top2;
	int bottom1, bottom2;


	int tX = ent->m_x + ent->Col_X;
	int tY = ent->m_y + ent->Col_Y;

	left1 = tX;
	left2 = oX;

	right1 = left1 + SCALED_HERO_SPRITE_WIDTH - 1 - ent->Col_Width;
	right2 = oX + oW - 1;

	top1 = tY;
	top2 = oY;

	bottom1 = top1 + SCALED_HERO_SPRITE_HEIGHT - 1 - ent->Col_Height;
	bottom2 = oY + oH - 1;


	if (bottom1 < top2) return false;
	if (top1 > bottom2) return false;

	if (right1 < left2) return false;
	if (left1 > right2) return false;

	return true;
}

bool WorldObjects::IsPosValid(EntityWalker* ent, int NewX, int NewY, std::shared_ptr<Location> loc)
{
	bool result = true;

	int dst_x = (NewX + ent->Col_X) / SCALED_SPRITE_WIDTH;
	int dst_y = (NewY + ent->Col_Y) / SCALED_SPRITE_HEIGHT;

	int dst_bot_x = ((NewX + ent->Col_X) + ent->Col_Width - 1) / SCALED_SPRITE_WIDTH;
	int dst_bot_y = ((NewY + ent->Col_Y) + ent->Col_Height - 1) / SCALED_SPRITE_HEIGHT;

	//int dst_bot_x = ((NewX + ent->Col_X) + SCALED_HERO_SPRITE_WIDTH - ent->Col_Width - 1) / SCALED_SPRITE_WIDTH;
	//int dst_bot_y = ((NewY + ent->Col_Y) + SCALED_HERO_SPRITE_HEIGHT - ent->Col_Height - 1) / SCALED_SPRITE_HEIGHT;

	for (int iY = dst_y; iY <= dst_bot_y; iY++) {
		for (int iX = dst_x; iX <= dst_bot_x; iX++) {
			//CTile* Tile = Location::starting_loc.GetTile(iX * SCALED_SPRITE_WIDTH, iY * SCALED_SPRITE_HEIGHT, 30);
			
			// iterate over all layers instead of one
			const int layers_count = ent->GetOccupiedLocation()->GetLayersCount();

			//CTile* Tile = ent->GetOccupiedLocation()->GetTile(iX * SCALED_SPRITE_WIDTH, iY * SCALED_SPRITE_HEIGHT, 30);
			
			for (int layer_index = 0; layer_index < layers_count; ++layer_index)
			{
				std::shared_ptr<CTile> Tile = ent->GetOccupiedLocation()->GetTileByLayer(iX * SCALED_SPRITE_WIDTH, iY * SCALED_SPRITE_HEIGHT, 30, layer_index);
				if (IsTileCollidable(Tile))
				{
					//if (DoesEntityColliadeWithTile(Tile, { dst_x ,dst_y, (ent->Col_Width - 1) * SCALED_HERO_SPRITE_WIDTH, (ent->Col_Height - 1) * SCALED_HERO_SPRITE_HEIGHT } ))


					int x_tile = Tile->m_collider.x + iX * SCALED_SPRITE_WIDTH;
					int w_tile =  Tile->m_collider.w;
					int y_tile = Tile->m_collider.y + iY * SCALED_SPRITE_HEIGHT;
					int h_tile =  Tile->m_collider.h;

					SDL_Rect map_tile_collision_box{ x_tile, y_tile, w_tile, h_tile };

					if (DoesEntityColliadeWithTile(map_tile_collision_box, { NewX + ent->Col_X ,NewY + ent->Col_Y, ent->Col_Width - 1, ent->Col_Height - 1 } ))
					{
						result = false;
					}
				}
			}
		}
	}


	std::vector<std::shared_ptr<IInteractable>>  interactable_in_loc_arr = interactables_location_map[loc];

	//for (int i = 0; i < npc_arr.size(); i++)
	for (int i = 0; i < interactable_in_loc_arr.size(); i++)
	{
		if (PosValidEntity(npc_arr[i], NewX, NewY, ent->Col_X, ent->Col_Y, ent->Col_Width, ent->Col_Height) == false) 
		{
			result = false;
		}
	}


	return result;
}

bool WorldObjects::PosValidTile(CTile* Tile)
{
	if (Tile == NULL) {
		return true;
	}

	if (Tile->TypeID == TILE_TYPE_BLOCK) {
		return false;
	}

	return true;
}

bool WorldObjects::IsTileCollidable(std::shared_ptr<CTile> Tile)
{
	return Tile->TypeID == TILE_TYPE_BLOCK;
}

bool WorldObjects::DoesEntityColliadeWithTile(const SDL_Rect& tile_collision, const SDL_Rect& entity_collision)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = entity_collision.x;
	rightA = entity_collision.x + entity_collision.w;
	topA = entity_collision.y;
	bottomA = entity_collision.y + entity_collision.h;

	//Calculate the sides of rect B
	leftB = tile_collision.x;
	rightB = tile_collision.x + tile_collision.w;
	topB = tile_collision.y;
	bottomB = tile_collision.y + tile_collision.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool WorldObjects::PosValidEntity(std::shared_ptr<EntityWalker> Entity, int NewX, int NewY, int col_x, int col_y, int col_w, int col_h)
{
	if (Entity != NULL && Collides(Entity, NewX + col_x, NewY + col_y, SCALED_HERO_SPRITE_WIDTH - col_w - 1, SCALED_HERO_SPRITE_HEIGHT - col_h - 1) == true)
	{

		//CEntityCol EntityCol;

		//EntityCol.EntityA = this;
		//EntityCol.EntityB = Entity;

		//CEntityCol::EntityColList.push_back(EntityCol);

		return false;
	}

	return true;
}



std::vector<std::shared_ptr<NPC>>& WorldObjects::GetNpcArr()
{
	return npc_arr;
}

void WorldObjects::AddEntranceWaypoint(EntranceWaypoint entrance_waypoint)
{
	m_world_entrance_waypoints.push_back(entrance_waypoint);
}

void WorldObjects::AddExitWaypoint(ExitWaypoint& exit_waypoint)
{
	m_world_exit_waypoints.push_back(exit_waypoint);
}

void WorldObjects::AddEmptyLocation(std::shared_ptr < Location >  location)
{
	m_world_locations.push_back(location);
}

void WorldObjects::AssociateEntranceToEndpoint()
{
	for (auto&& exit_waypoint : m_world_exit_waypoints)
	{
		exit_waypoint.AssociateEntranceWaypoint(GetEntryPoint(exit_waypoint.GetWaypointDestination()));
	}
}

void WorldObjects::AssociateEndpointsToLocations()
{
	for (auto&& exit_waypoint : m_world_exit_waypoints)
	{
		auto location = GetLocation(exit_waypoint.GetWaypointLocation());
		location->AddExitWaypoint(exit_waypoint);
		//exit_waypoint.AssociateEntranceWaypoint(GetEntryPoint(exit_waypoint.GetWaypointDestination()));
	}
}

void WorldObjects::AssociateEntrypointsToLocations()
{
	for (auto&& entrance_waypoint : m_world_entrance_waypoints)
	{
		auto location = GetLocation(entrance_waypoint.GetWaypointLocation());
		entrance_waypoint.AssociateLocation(location);
	}
}

bool WorldObjects::LoadLuaLocations()
{
	bool success = true;
	const std::string MAP_RESOURCE_LOCATION = "res/map/";
	for (auto&& location : m_world_locations)
	{
		const std::string loc_path = MAP_RESOURCE_LOCATION + location->GetLocationName() + ".lua";
		//if (!location->OnLoadLua(loc_path, 2) || !location->OnLoadLua(loc_path, 1))
		if (!location->OnLoadLua(loc_path, 1))
		{
			// FATAL
			std::cout << "Failed to load " << location->GetLocationName() << std::endl;
			success = false;
		}
		
		int elements_layer_count = LuaLoader::GetElementLayersCount(loc_path) - 2;
		if (elements_layer_count > 0)
		{
			location->SetLocationElementsVectorSize(elements_layer_count);
			for (int i = 0; i < elements_layer_count; ++i)
			{
				LocationElements loc_elements(location->GetLocationName());
				location->InsertLocationElements(loc_elements);
				std::vector<std::shared_ptr<SavePoint>> location_save_points;
				LuaLoader::OnLocationLoadLua(loc_path, i + 2, location->GetLocationElements(i), location_save_points);
			}
		}

		location->SetLocationLayersPtrs();

		LoadNpcsInLocation(loc_path, location);
		/*LuaLoader::OnLocationLoadLua(loc_path, 2, location);
		LuaLoader::OnLocationLoadLua(loc_path, 3, location);*/
	}
	return success;
}

bool WorldObjects::LoadDBCharacterEntries()
{	
	bool success = true;

	if (std::filesystem::exists(CHARACTER_DATABASE_EXTRACT_LOCATION))
	{
		CSVCharacterParser::Parse(CHARACTER_DATABASE_EXTRACT_LOCATION, m_db_character_entries);
	}
	else
	{
		success = false;
	}
	
	return success;
}

void WorldObjects::FillInteractablesLocationMap()
{
	for (auto [key, value] : npcs_location_map)
	{
		for (auto&& el : value)
		{
			interactables_location_map[key].emplace_back(el);
		}
	}
}

bool WorldObjects::MatchNPCToTiledRepresentation()
{
	for (auto&& npc : npc_arr)
	{
		bool found_match = false;
		for (auto&& db_npc_entry : m_db_character_entries)
		{
			if (npc->GetNpcName() == db_npc_entry.m_name)
			{
				npc->FillWithDBData(db_npc_entry);
				found_match = true;
				break;
			}
		}
		if (!found_match)
		{
			//FATAL ?
			std::cout << " Unable to match npc to it's corresponding db entry ! \n";
			std::cout << "npc name " << npc->GetNpcName() << std::endl;
			return false;
		}
	}
	return true;
}

void WorldObjects::LoadNPCDialogues()
{
	for (auto&& npc : npc_arr)
	{
		if (npc->LoadDialogue())
		{
			interactable_arr.push_back(npc);
		}
	}
}

void WorldObjects::LoadLocationInteractables(const std::shared_ptr<Location>& loc)
{

}

bool WorldObjects::LoadFacts()
{
	const char FACTS_EXTRACT_LOCATION[] = "res/scripts/facts/facts.csv";
	bool result = CSVFactParser::Parse(FACTS_EXTRACT_LOCATION, m_world_facts);
	ActiveFactStates::get().InitFactValues(m_world_facts);

	return result;
}

bool WorldObjects::LoadResources()
{
	if (LoadLuaLocations() && LoadDBCharacterEntries() && MatchNPCToTiledRepresentation() && LoadFacts())
	{
		LoadNPCDialogues();
		FillInteractablesLocationMap();
		
		return true;
	}
	return false;
}

bool WorldObjects::LoadNpcsInLocation(const std::string& map_file_path, std::shared_ptr<Location> location)
{
	std::vector<std::shared_ptr<NPC>>  npc_per_location_arr;
	   
	NpcLuaLoader npc_loader;
	npc_loader.OnLoadLua(map_file_path, npc_per_location_arr);

	npcs_location_map[location] = npc_per_location_arr;

	std::vector<std::shared_ptr<IInteractable>> interactables_in_location;

	for (auto&& npc : npc_per_location_arr)
	{
		npc->SetLocation(location);
		npc_arr.emplace_back(npc);
		if (npc->HasDialogue())
		{
			//interactables_location_map[location] = npc;
			interactables_in_location.emplace_back(npc);
		}
		//npc->SetLocation(GetLocation(location_name));
	}
	interactables_location_map[location] = interactables_in_location;

	return false;
}

EntranceWaypoint* WorldObjects::GetEntryPoint(const std::string& entrance_waypoint_name)
{
	for (auto&& entrance_waypoint : m_world_entrance_waypoints)
	{
		// TODO:
		// Profile performance
		// Test different comparison techniques 
		if (entrance_waypoint.GetWaypointName() == entrance_waypoint_name)
		{
			return &entrance_waypoint;
		}
	}
	// FATAL
	std::cout << "Unable to associate entrypoint to its endpoint ="<< entrance_waypoint_name << std::endl;
	return nullptr;
}

std::shared_ptr<Location> WorldObjects::GetLocation(const std::string& location_name)
{
	for (auto&& location : m_world_locations)
	{
		// TODO:
		// Profile performance
		// Test different comparison techniques 
		if (location->GetLocationName() == location_name)
		{
			return location;
		}
	}
	// FATAL
	std::cout << "Unable to find location with location_name = " << location_name << std::endl;
	return nullptr;
}

std::vector<std::shared_ptr<NPC>> WorldObjects::GetNpcLocationVector(const std::shared_ptr<Location>& loc) const
{
	// TODO: insert return statement here
	return npcs_location_map.at(loc);
}

std::vector<std::shared_ptr<IInteractable>> WorldObjects::GetInteractableVector(const std::shared_ptr<Location>& loc) const
{
	return interactables_location_map.at(loc);
}
