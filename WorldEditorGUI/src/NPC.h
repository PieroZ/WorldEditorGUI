#pragma once
#include "EntityWalker.h"
#include "CSVCharactersParser.h"
#include "sol.hpp"

#include "DialogControllerState.h"
#include "DialogGUI.h"

class NPC : public EntityWalker
{
public:
	NPC(/*App& world,*/ std::shared_ptr<Location> occupied_location, int x, int y, int id, int sprite_x, int sprite_y);
	NPC(const std::string& npc_name);
	//void OnRender(SDL_Renderer* renderer) override;
	void OnRender(SDL_Renderer* renderer, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);
	void DrawBoundingRect(SDL_Renderer* renderer) override;
	void Talk(DialogGUI* gui, DialogControllerState& state, bool& has_said, std::vector<std::string>& choices_lines, int& choice_id, bool& last_line_flag);
	std::string GetNpcName() const;
	void SetName(const std::string& name);
	bool LoadDialogue();
	bool LoadDialogue(const std::string& dialogue_file_path);

	//sol::state GetDialogLuaState();
	void SetDialogLuaState(sol::state* dialog_state);

	bool IsRotatable() const override;
	void SetDirBasedOnPlayer(const Direction& players_dir);
	void SetDir(const Direction& dir);
	int GetSpriteIdFromDir(const Direction& dir) const;
	void SetSpriteXYFromDir(const Direction& dir);

	// Inherited via IInteractive
	virtual void Interact(App* app) override;
	void FillWithDBData(const DBCharacterEntry& entry);
	bool HasDialogue() const;	
	static void ReadLuaTableValues(sol::table properties);
	static bool CheckLuaFacts(sol::table table_facts_check);



	// debug methods only

	void SetDebugFont(TTF_Font* debug_font);

	// end of debug methods

private:
	bool ShouldBeDrawnOnScreen(int x, int y);
public:


	sol::state m_dialog_lua;
	std::string m_npc_name;
	bool dialogue_exists = false;

	// debug only vars
	bool m_render_npc_name;
	std::shared_ptr<Texture> m_npc_name_texture;
	TTF_Font* m_debug_font;

	// end of debug vars

	inline static const char LUA_CURRENT_DIALOG[] = "CurrentDialog";
	inline static const char LUA_SAY[] = "say";
	inline static const char LUA_DONE[] = "done";
	inline static const char LUA_ON_SELECTION[] = "onSelection";
	inline static const char LUA_ON_CHOICES[] = "choices";
	inline static const char LUA_CONTINUE[] = "continue";
	inline static const char LUA_END[] = "end";
	inline static const char LUA_FACTS_MODIFIED[] = "FactInc";
	inline static const char LUA_FACT_INC_TABLE[] = "FactIncTable";
	inline static const char LUA_UPDATE_FACTS[] = "facts_update";
	inline static const char LUA_PICK_STARTING_POINT[] = "starting_point";
};

