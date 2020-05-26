#include "WorldObjects.h"
#include "Defines.h"
#include <filesystem>


WorldObjects::WorldObjects()
{
	//AddNpc(400, 400, 0, 1, 4);
	//AddNpc(1000, 400, 1, 4, 0);
	AddNpc("res/scripts/npcs/TESTLOC_NPC_10000.lua");
}

WorldObjects::~WorldObjects()
{
}

void WorldObjects::AddNpc(int x, int y, int id, int sprite_x, int sprite_y)
{
	auto npc = std::make_shared<NPC>(x, y, id, sprite_x, sprite_y);
	if (npc->dialogue_exists)
	{
		interactable_arr.push_back(npc);
	}
	npc_arr.push_back(npc);
}

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
		AddNpc(x, y, id, sprite_x, sprite_y);
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

std::string WorldObjects::PlayDialogue(std::shared_ptr<NPC> dialogue_holder, bool& last_line)
{
	// Which dialogue should I play?
	return (dialogue_holder->Talk(last_line));
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

bool WorldObjects::IsPosValid(EntityWalker* ent, int NewX, int NewY)
{
	bool result = true;

	int StartX = (NewX + ent->Col_X) / SCALED_SPRITE_WIDTH;
	int StartY = (NewY + ent->Col_Y) / SCALED_SPRITE_HEIGHT;

	int EndX = ((NewX + ent->Col_X) + SCALED_HERO_SPRITE_WIDTH - ent->Col_Width - 1) / SCALED_SPRITE_WIDTH;
	int EndY = ((NewY + ent->Col_Y) + SCALED_HERO_SPRITE_HEIGHT - ent->Col_Height - 1) / SCALED_SPRITE_HEIGHT;

	for (int iY = StartY; iY <= EndY; iY++) {
		for (int iX = StartX; iX <= EndX; iX++) {
			CTile* Tile = Location::starting_loc.GetTile(iX * SCALED_SPRITE_WIDTH, iY * SCALED_SPRITE_HEIGHT);

			if (PosValidTile(Tile) == false)
			{
				result = false;
			}
		}
	}


	for (int i = 0; i < npc_arr.size(); i++)
	{
		if (PosValidEntity(npc_arr[i], NewX, NewY, ent->Col_X, ent->Col_Y, ent->Col_Width, ent->Col_Height) == false) {
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
