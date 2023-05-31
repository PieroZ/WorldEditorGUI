#include "WindowGUI.h"
#include "TextureBank.h"

WindowGUI::WindowGUI() : SINGLE_ROW_Y_OFFSET(0), m_text_font(nullptr), m_renderer(nullptr), m_window_dimensions{0,0,0,0}
{
}

WindowGUI::WindowGUI(SDL_Renderer* renderer, TTF_Font* text_font, const SDL_Rect& window_dimensions) 
	: SINGLE_ROW_Y_OFFSET(0), m_renderer(renderer), m_text_font(text_font), m_window_dimensions(window_dimensions)
{
	if (m_text_font)
	{
		SINGLE_ROW_Y_OFFSET = 2 * TTF_FontLineSkip(m_text_font);
	}
	const int scale = 2;

	m_horizontal_border_width = TextureBank::Get("dialogue_frame_border_horiz")->GetWidth() * scale;
	m_border_height = TextureBank::Get("dialogue_frame_border")->GetHeight() * scale;
	m_currently_highlighted_text_row = -1;
	m_base_text_color = { 255, 255, 255 };
}

void WindowGUI::Render()
{
	TextureBank::Get("dialogue_frame")->Render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	const SDL_Rect gui_space_rect = { m_window_dimensions.x, m_window_dimensions.y + m_border_height, m_window_dimensions.w, m_window_dimensions.h - m_border_height };

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(m_renderer, &gui_space_rect);


	TextureBank::Get("dialogue_frame_border")->Render(m_window_dimensions.x + m_horizontal_border_width, m_window_dimensions.y, m_window_dimensions.w - m_horizontal_border_width, m_border_height);
	TextureBank::Get("dialogue_frame_border")->Render(m_window_dimensions.x + m_horizontal_border_width, m_window_dimensions.y + m_window_dimensions.h, m_window_dimensions.w - m_horizontal_border_width, m_border_height);

	//GUI shadow
	//TextureBank::Get("dialogue_frame")->Render(m_window_dimensions.x, m_window_dimensions.y + m_border_height, m_window_dimensions.w, m_window_dimensions.h - m_border_height);

	TextureBank::Get("dialogue_frame_border_horiz")->Render(m_window_dimensions.x, m_window_dimensions.y + m_border_height, m_horizontal_border_width, m_window_dimensions.h - m_border_height);
	TextureBank::Get("dialogue_frame_border_horiz")->Render(m_window_dimensions.x + m_window_dimensions.w, m_window_dimensions.y + m_border_height, m_horizontal_border_width, m_window_dimensions.h - m_border_height);

	TextureBank::Get("dialogue_frame_border_corner_t_l")->Render(m_window_dimensions.x, m_window_dimensions.y, m_horizontal_border_width, m_border_height);
	TextureBank::Get("dialogue_frame_border_corner_t_r")->Render(m_window_dimensions.x + m_window_dimensions.w, m_window_dimensions.y, m_horizontal_border_width, m_border_height);
	
	TextureBank::Get("dialogue_frame_border_corner_b_l")->Render(m_window_dimensions.x, m_window_dimensions.y + m_window_dimensions.h, m_horizontal_border_width, m_border_height);
	TextureBank::Get("dialogue_frame_border_corner_b_r")->Render(m_window_dimensions.x + m_window_dimensions.w, m_window_dimensions.y + m_window_dimensions.h, m_horizontal_border_width, m_border_height);


	/*TextureBank::Get("dialogue_frame_border_corner_t_l")->Render(m_window_dimensions.x, m_window_dimensions.y, m_horizontal_border_width, m_border_height);
	TextureBank::Get("dialogue_frame_border_corner_t_r")->Render(m_window_dimensions.x + m_window_dimensions.w, m_window_dimensions.y, m_horizontal_border_width, m_border_height);
	TextureBank::Get("dialogue_frame_border_corner_b_l")->Render(m_window_dimensions.x, m_window_dimensions.y + m_window_dimensions.h, m_horizontal_border_width, m_border_height);
	TextureBank::Get("dialogue_frame_border_corner_b_r")->Render(m_window_dimensions.x + m_window_dimensions.w, m_window_dimensions.y + m_window_dimensions.h, m_horizontal_border_width, m_border_height);*/

	for (auto&& texture_coordinates_pair : m_window_text_textures)
	{
		const SDL_Point& screen = texture_coordinates_pair.second;
	

		const SDL_Rect rect{ m_window_dimensions.x + m_window_dimensions.w / 9, screen.y,  7 * m_window_dimensions.w / 9, SINGLE_ROW_Y_OFFSET / 2 };

		/*SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 80);
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderFillRect(m_renderer, &rect);
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(m_renderer, &rect);*/

		texture_coordinates_pair.first->Render(screen.x, screen.y);
	}

	/*const SDL_Rect selection_rect = { m_window_dimensions.x + 20, m_window_text_textures[1].second.y - SINGLE_ROW_Y_OFFSET/4, m_window_dimensions.w - 20, SINGLE_ROW_Y_OFFSET };
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

	SDL_RenderDrawRect(m_renderer, &selection_rect);*/
}

bool WindowGUI::LoadText(const std::string& text, const SDL_Point& relative_pos, const SDL_Color text_color /*= { 0, 0 ,0 }*/)
{
	bool success = false;
	if (m_text_font && m_renderer)
	{
		success = true;
	}
	SDL_Point pos = { m_window_dimensions.x + relative_pos.x,  m_window_dimensions.y + relative_pos.y };
	m_window_text_textures.emplace_back(std::make_pair(std::make_unique<Texture>(), pos));
	m_gui_strings.emplace_back(text);
	m_window_text_textures.back().first->loadFromRenderedText(m_renderer, text, text_color, m_text_font);
	//m_debug_lines_textures[debug_line_key] = std::make_pair(map_index, std::make_unique<Texture>());
	return success;
}

bool WindowGUI::LoadCenteredText(const std::string& text, const SDL_Color text_color)
{
	bool success = false;
	if (m_text_font && m_renderer)
	{
		success = true;
	}

	int text_width = 0;
	int text_height = 0;


	TTF_SizeText(m_text_font, text.c_str(), &text_width, &text_height);
	SDL_Point pos = { m_window_dimensions.x + (m_window_dimensions.w + m_horizontal_border_width - text_width)/2,  m_window_dimensions.y + (m_window_dimensions.h - text_height)/2 };
	m_window_text_textures.emplace_back(std::make_pair(std::make_unique<Texture>(), pos));
	m_gui_strings.emplace_back(text);
	m_window_text_textures.back().first->loadFromRenderedText(m_renderer, text, text_color, m_text_font);

	//m_debug_lines_textures[debug_line_key] = std::make_pair(map_index, std::make_unique<Texture>());
	return success;
}

bool WindowGUI::AppendCenteredBottomText(const std::string& text, SDL_Color text_color)
{
	bool success = false;
	if (m_text_font && m_renderer)
	{
		success = true;

		if (SINGLE_ROW_Y_OFFSET > 0)
		{
			m_window_dimensions.h += SINGLE_ROW_Y_OFFSET;
		}


		int text_width = 0;
		int text_height = 0;


		TTF_SizeText(m_text_font, text.c_str(), &text_width, &text_height);
		SDL_Point pos = { m_window_dimensions.x + (m_window_dimensions.w + m_horizontal_border_width - text_width) / 2,  m_window_dimensions.y + m_window_dimensions.h - SINGLE_ROW_Y_OFFSET };
		m_window_text_textures.emplace_back(std::make_pair(std::make_unique<Texture>(), pos));
		m_gui_strings.emplace_back(text);
		m_window_text_textures.back().first->loadFromRenderedText(m_renderer, text, text_color, m_text_font);
	}

	return success;
}

bool WindowGUI::LoadBottomLeftText(const std::string& text, int min_x_offset, int min_y_offset, const SDL_Color text_color)
{
	bool success = false;
	if (m_text_font && m_renderer)
	{
		success = true;
	}

	int text_width = 0;
	int text_height = 0;


	TTF_SizeText(m_text_font, text.c_str(), &text_width, &text_height);
	SDL_Point pos = { m_window_dimensions.x + min_x_offset,  m_window_dimensions.y + m_window_dimensions.h - min_y_offset};
	m_window_text_textures.emplace_back(std::make_pair(std::make_unique<Texture>(), pos));
	m_gui_strings.emplace_back(text);
	m_window_text_textures.back().first->loadFromRenderedText(m_renderer, text, text_color, m_text_font);

	//m_debug_lines_textures[debug_line_key] = std::make_pair(map_index, std::make_unique<Texture>());
	return success;
}

bool WindowGUI::LoadBottomRightText(const std::string& text, int min_x_offset, int min_y_offset, const SDL_Color text_color)
{
	bool success = false;
	if (m_text_font && m_renderer)
	{
		success = true;
	}

	int text_width = 0;
	int text_height = 0;


	TTF_SizeText(m_text_font, text.c_str(), &text_width, &text_height);
	SDL_Point pos = { m_window_dimensions.x + (m_window_dimensions.w - text_width) / 2,  m_window_dimensions.y + (m_window_dimensions.h - text_height) / 2 };
	m_window_text_textures.emplace_back(std::make_pair(std::make_unique<Texture>(), pos));
	m_gui_strings.emplace_back(text);
	m_window_text_textures.back().first->loadFromRenderedText(m_renderer, text, text_color, m_text_font);

	//m_debug_lines_textures[debug_line_key] = std::make_pair(map_index, std::make_unique<Texture>());
	return success;
}

void WindowGUI::Clear()
{
}

void WindowGUI::RemoveHighlight(int row)
{
	m_window_text_textures[row].first->loadFromRenderedText(m_renderer, m_gui_strings[row], m_base_text_color, m_text_font);
}

void WindowGUI::SetFont(TTF_Font* debug_font)
{
}

void WindowGUI::SetHighlightedText(int row)
{
	if(m_currently_highlighted_text_row >= 0)
	{
		RemoveHighlight(m_currently_highlighted_text_row);
	}
	m_currently_highlighted_text_row = row;
	static const SDL_Color HIGHLIGHT_COLOR = { 255, 255, 0 };

	m_window_text_textures[row].first->loadFromRenderedText(m_renderer, m_gui_strings[row], HIGHLIGHT_COLOR, m_text_font);
}

int WindowGUI::GetHighlightedRow() const
{
	return m_currently_highlighted_text_row;
}
