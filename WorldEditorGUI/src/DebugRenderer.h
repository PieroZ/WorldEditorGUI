#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>
#include "Texture.h"

class DebugRenderer
{
	DebugRenderer();
public:
	DebugRenderer(DebugRenderer const&) = delete;
	void operator=(DebugRenderer const&) = delete;

	static DebugRenderer& get();
	
	void AddDebugLine(const std::string& debug_line_key, const std::string& debug_line, SDL_Renderer* renderer, const SDL_Color color = { 0,0,0 });
	void SetFont(TTF_Font* debug_font);
	void SetYOffset(const int y_offset);
	void SetX0(const int x0);
	void SetY0(const int y0);
	void OnRender(SDL_Renderer* renderer);

private:
	TTF_Font* m_debug_font = nullptr;
	int m_x0 = 50;
	int m_y0 = 50;
	int m_y_offset_inc = 20;
	std::vector<std::string> m_debug_lines;
	
	//std::vector<std::unique_ptr<Texture>> m_debug_lines_textures;
	std::unordered_map<std::string, std::pair<int, std::unique_ptr<Texture>>> m_debug_lines_textures;
};

