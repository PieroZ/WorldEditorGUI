#pragma once

#include "NPC.h"
#include <vector>
#include <memory>

class WorldObjects final
{
public:
	//WorldObjects(World& owner);
	WorldObjects();
	~WorldObjects();

	void AddNpc(int x, int y, int id, int sprite_x, int sprite_y);
	void AddNpc(const std::string& lua_filepath);
	std::shared_ptr<NPC> FindNpcById(const uint32_t interactable_id);
	bool IsInteractableAt(int x, int y, bool& is_dialogue, std::shared_ptr<NPC>& interact);
	//bool IsInteractableAt(int x, int y, bool& is_dialogue, uint32_t& interactable_id);
	std::string PlayDialogue(std::shared_ptr<NPC> dialogue_holder, bool& last_line);

	bool Collides(std::shared_ptr<EntityWalker> ent, int oX, int oY, int oW, int oH);
	bool IsPosValid(EntityWalker* ent, int NewX, int NewY);
	bool PosValidTile(CTile* Tile);
	bool PosValidEntity(std::shared_ptr<EntityWalker> Entity, int NewX, int NewY, int col_x, int col_y, int col_w, int col_h);

public:
	std::vector<std::shared_ptr<NPC>>& GetNpcArr();

private:
	std::vector<std::shared_ptr<NPC>>  npc_arr;
	std::vector<std::shared_ptr<IInteractable>>  interactable_arr;
};