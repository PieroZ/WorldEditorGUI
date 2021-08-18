#pragma once
#include "TextureBank.h"

#include <SDL_ttf.h>
#include <string>

class DialogGUI
{
public:
	DialogGUI();
	DialogGUI(SDL_Renderer* renderer);
	~DialogGUI();
	bool Init(SDL_Renderer* renderer, const std::string& font_file_path, const std::string& displayed_text, const std::string& dialog_frame_name);
	void Cleanup();
	void SetDisplayedText(const std::string& text, const std::string& speaker_name);
	void SetDialogFrameName(const std::string& dialog_frame_name);
	void SetFont(TTF_Font* font);
	void SetTextColor(const SDL_Color& text_color);
	void SetFontSize(int font_size);
	void SetRenderer(SDL_Renderer* renderer);
	bool LoadTexture();
	void OnRender();
	void SetCoords(int x, int y, int w, int h);
	void SetHighlightedRow(int highlighted_row);
	void NextHighlightedRow();
	void PreviousHighlightedRow();
	bool IsHighlightActive() const;
public:
	SDL_Renderer* m_renderer;
	Texture m_text_texture;
	Texture m_speaker_name_text_texture;

	std::string m_displayed_text;
	std::string m_speaker_name;

	std::string m_dialog_frame_sprite_name;
	TTF_Font* m_font;
	SDL_Color m_text_color;
	int m_font_size;
	int m_screen_x, m_screen_y, m_screen_width, m_screen_height;
	int m_highlighted_row;
	
	// Highlight
	int m_rows_count;

	static const int DEFAULT_FONT_SIZE = 26;
};
