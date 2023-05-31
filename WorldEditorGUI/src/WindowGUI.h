#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <memory>
#include <utility>

#include "Texture.h"

class WindowGUI
{
public:
	WindowGUI();
	WindowGUI(SDL_Renderer* renderer, TTF_Font* text_font, const SDL_Rect& window_dimensions);
	virtual void Render();
	virtual void Clear();
	bool LoadText(const std::string& text, const SDL_Point& pos, const SDL_Color text_color = { 255,255,255 });
	bool LoadCenteredText(const std::string& text, const SDL_Color text_color = { 255,255,255 });
	
	// Method appends text at the bottom of GUI expanding it size by single text line height
	bool AppendCenteredBottomText(const std::string& text, const SDL_Color text_color = { 255,255,255 });

	bool LoadBottomLeftText(const std::string& text, int min_x_offset, int min_y_offset, const SDL_Color text_color = { 255,255,255 });
	bool LoadBottomRightText(const std::string& text, int min_x_offset, int min_y_offset, const SDL_Color text_color = { 255,255,255 });
	void RemoveHighlight(int row);
	void SetFont(TTF_Font* debug_font);
	void SetHighlightedText(int row);
	int GetHighlightedRow() const;


private:
	int m_margin_top, m_margin_bottom, m_margin_left, m_margin_right;
	int SINGLE_ROW_Y_OFFSET;
	TTF_Font* m_text_font;
	SDL_Renderer* m_renderer;
	SDL_Rect m_window_dimensions;
	std::vector<std::pair <std::unique_ptr<Texture>, SDL_Point> > m_window_text_textures;
	std::vector<std::string> m_gui_strings;
	int m_currently_highlighted_text_row;

	int m_border_height;
	int m_horizontal_border_width;

	SDL_Color m_base_text_color;
};