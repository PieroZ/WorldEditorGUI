#pragma once

#include "NPC.h"
//#include "PlayerWalker.h"
#include "Location.h"
#include <vector>
#include <memory>
#include "CSVCharactersParser.h"
#include "CSVFactsParser.h"

class App;

class PlayerWalker;

class WorldObjects final
{
public:
	//WorldObjects(World& owner);
	WorldObjects();
	~WorldObjects();

	//void AddNpc(Location& loc, int x, int y, int id, int sprite_x, int sprite_y);
	void AddNpc(App& world, std::shared_ptr<Location> loc, int x, int y, int id, int sprite_x, int sprite_y);
	void AddNpc(const std::string& lua_filepath);
	std::shared_ptr<NPC> FindNpcById(const uint32_t interactable_id);
	bool IsInteractableAt(int x, int y, bool& is_dialogue, std::shared_ptr<NPC>& interact);
	bool IsInteractableAt(EntityWalker::Direction player_dir, int player_x, int player_y, bool& is_dialogue, std::shared_ptr<NPC>& interact);
	
	bool IsInteractableAt(PlayerWalker& player, bool& is_dialogue, std::shared_ptr<NPC>& interact, std::shared_ptr<Location> loc);

	//bool IsInteractableAt(int x, int y, bool& is_dialogue, uint32_t& interactable_id);
	std::string PlayDialogue(std::shared_ptr<NPC> dialogue_holder, bool& last_line);

	bool Collides(std::shared_ptr<EntityWalker> ent, int oX, int oY, int oW, int oH);
	bool IsPosValid(EntityWalker* ent, int NewX, int NewY, std::shared_ptr<Location> loc);
	bool PosValidTile(CTile* Tile);
	bool IsTileCollidable(std::shared_ptr<CTile> Tile);
	bool DoesEntityColliadeWithTile(const SDL_Rect& tile_collision, const SDL_Rect& entity_collision);
	bool PosValidEntity(std::shared_ptr<EntityWalker> Entity, int NewX, int NewY, int col_x, int col_y, int col_w, int col_h);

public:
	std::vector<std::shared_ptr<NPC>>& GetNpcArr();
	void AddEntranceWaypoint(EntranceWaypoint entrance_waypoint);
	void AddExitWaypoint(ExitWaypoint& exit_waypoint);
	void AddEmptyLocation(std::shared_ptr < Location > location);
	void AssociateEntranceToEndpoint();
	void AssociateEndpointsToLocations();
	void AssociateEntrypointsToLocations();
	bool LoadLuaLocations();
	bool LoadDBCharacterEntries();

	void FillInteractablesLocationMap();

	bool MatchNPCToTiledRepresentation();
	void LoadNPCDialogues();
	void LoadLocationInteractables(const std::shared_ptr<Location>& loc);
	bool LoadFacts();
	bool LoadResources();
	bool LoadNpcsInLocation(const std::string& map_file_path, std::shared_ptr<Location> location);

	EntranceWaypoint* GetEntryPoint(const std::string& entrance_waypoint_name);
	std::shared_ptr<Location> GetLocation(const std::string& location_name);

	std::vector<std::shared_ptr<NPC>> GetNpcLocationVector(const std::shared_ptr<Location>& loc) const;
	std::vector<std::shared_ptr<IInteractable>> GetInteractableVector(const std::shared_ptr<Location>& loc) const;

private:
	std::vector<std::shared_ptr<NPC>>  npc_arr;
	std::vector<std::shared_ptr<IInteractable>>  interactable_arr;
	std::unordered_map< std::shared_ptr<Location>, std::vector<std::shared_ptr<NPC>>> npcs_location_map;
	std::unordered_map< std::shared_ptr<Location>, std::vector<std::shared_ptr<IInteractable>>> interactables_location_map;
//	std::vector<std::string> locations_name_arr;

	std::vector< std::shared_ptr<Location>> m_world_locations;
	std::vector<ExitWaypoint> m_world_exit_waypoints;
	std::vector<EntranceWaypoint> m_world_entrance_waypoints;

	std::vector< DBCharacterEntry > m_db_character_entries;

	// Change int to Fact object so we can proto-serialize it later?
	std::unordered_map<std::string, int> m_world_facts;
};