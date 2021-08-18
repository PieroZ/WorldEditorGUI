#include "DialogGUI.h"

// TODO: CHANGE IOSTREAM TO LOGGING 
#include <iostream>


DialogGUI::DialogGUI() : m_renderer(nullptr), m_font(nullptr), m_font_size(DEFAULT_FONT_SIZE), m_text_color{ 255, 255, 255 }, m_screen_x(0), m_screen_y(0), m_screen_width(0), m_screen_height(0), m_highlighted_row(-1)
{
}

DialogGUI::DialogGUI(SDL_Renderer* renderer) : m_renderer(renderer), m_font(nullptr), m_font_size(DEFAULT_FONT_SIZE), m_text_color{0, 0 ,0}, m_screen_x(0), m_screen_y(0), m_screen_width(0), m_screen_height(0), m_highlighted_row(-1)
{
}

DialogGUI::~DialogGUI()
{
	Cleanup();
}

bool DialogGUI::Init(SDL_Renderer* renderer, const std::string& font_file_path, const std::string& displayed_text, const std::string& dialog_frame_name)
{
	SetRenderer(renderer);
	if (!(m_font = TTF_OpenFont(font_file_path.c_str(), m_font_size)))
	{
		std::cerr << "Error: Unable to open font" << std::endl;
		return false;
	}
	//TTF_SetFontKerning(m_font, 1);
	SetDisplayedText(displayed_text, "KUPA");
	SetDialogFrameName(dialog_frame_name);
	if (!LoadTexture())
	{
		return false;
	}

	return true;
}

void DialogGUI::Cleanup()
{
	if (m_font)
	{
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
}

void DialogGUI::SetDisplayedText(const std::string& text, const std::string& speaker_name)
{
	m_displayed_text = text;
	m_speaker_name = speaker_name;
	LoadTexture();
}

void DialogGUI::SetDialogFrameName(const std::string& dialog_frame_name)
{
	m_dialog_frame_sprite_name = dialog_frame_name;
}

void DialogGUI::SetFont(TTF_Font* font)
{
	m_font = font;
}

void DialogGUI::SetTextColor(const SDL_Color& text_color)
{
	m_text_color = text_color;
}

void DialogGUI::SetFontSize(int font_size)
{
	m_font_size = font_size;
}

void DialogGUI::SetRenderer(SDL_Renderer* renderer)
{
	m_renderer = renderer;
}

bool DialogGUI::LoadTexture()
{
	return m_text_texture.loadFromRenderedText(m_renderer, m_displayed_text, m_text_color, m_font) 
		&& m_speaker_name_text_texture.loadFromRenderedText(m_renderer, m_speaker_name, { 255, 46, 57 }, m_font);
}

void DialogGUI::OnRender()
{
	static const int x_offset = 20;
	static const int y_offset = 50;
	const int scale = 2;
	//Uint8 r = 0x1F;
	//Uint8 g = 0x00;
	//Uint8 b = 0xBF;	
	
	/*Uint8 r = 0xDF;
	Uint8 g = 0xA1;
	Uint8 b = 0x00;*/

	Uint8 r = 0xD1;
	Uint8 g = 0xD1;
	Uint8 b = 0xD1;


	//TextureBank::Get("dialogue_glow")->setColor(r, g, b);
	TextureBank::Get("dialogue_frame_border")->setColor(r, g, b);
	TextureBank::Get("dialogue_frame_border_horiz")->setColor(r, g, b);
	//TextureBank::Get("dialogue_frame_border_corner")->setColor(0xFF, 0, 0);
	TextureBank::Get("dialogue_frame_border_corner_t_l")->setColor(r, g, b);
	TextureBank::Get("dialogue_frame_border_corner_t_r")->setColor(r, g, b);
	TextureBank::Get("dialogue_frame_border_corner_b_l")->setColor(r, g, b);
	TextureBank::Get("dialogue_frame_border_corner_b_r")->setColor(r, g, b);
	

	TextureBank::Get(m_dialog_frame_sprite_name)->Render(m_screen_x - x_offset, m_screen_y - y_offset, m_screen_width, m_screen_height);

	
	
	const int border_height = TextureBank::Get("dialogue_frame_border")->GetHeight() * scale;

	//TextureBank::Get("dialogue_glow")->Render(m_screen_x - x_offset, m_screen_y - y_offset - 64 - border_height, m_screen_width, 128);

	TextureBank::Get("dialogue_frame_border")->Render(m_screen_x - x_offset, m_screen_y - y_offset, m_screen_width, border_height);
	TextureBank::Get("dialogue_frame_border")->Render(m_screen_x - x_offset, m_screen_y - y_offset + m_screen_height - border_height, m_screen_width, border_height);

	

	const int horizontal_border_width = TextureBank::Get("dialogue_frame_border_horiz")->GetWidth() * scale;

	TextureBank::Get("dialogue_frame_border_horiz")->Render(m_screen_x - x_offset, m_screen_y - y_offset, horizontal_border_width, m_screen_height);
	TextureBank::Get("dialogue_frame_border_horiz")->Render(m_screen_x - x_offset + m_screen_width - horizontal_border_width, m_screen_y - y_offset, horizontal_border_width, m_screen_height);

	TextureBank::Get("dialogue_frame_border_corner_t_l")->Render(m_screen_x - x_offset, m_screen_y - y_offset, horizontal_border_width, border_height);
	//TextureBank::Get("dialogue_frame_border_corner_t_l_2")->Render(m_screen_x - x_offset, m_screen_y - y_offset);
	TextureBank::Get("dialogue_frame_border_corner_t_r")->Render(m_screen_x - x_offset + m_screen_width - horizontal_border_width, m_screen_y - y_offset, horizontal_border_width, border_height);
	TextureBank::Get("dialogue_frame_border_corner_b_l")->Render(m_screen_x - x_offset, m_screen_y - y_offset + m_screen_height - border_height, horizontal_border_width, border_height);
	TextureBank::Get("dialogue_frame_border_corner_b_r")->Render(m_screen_x - x_offset + m_screen_width - horizontal_border_width, m_screen_y - y_offset + m_screen_height - border_height, horizontal_border_width, border_height);

	if (IsHighlightActive())
	{
		const SDL_Rect rect{ m_screen_x, m_highlighted_row * TTF_FontHeight(m_font) + m_screen_y, Texture::GetLineWrapLimit(), TTF_FontHeight(m_font) };
		SDL_RenderDrawRect(m_renderer, &rect);
	}
	m_speaker_name_text_texture.Render(m_screen_x, m_screen_y - y_offset + 10);
	m_text_texture.Render(m_screen_x, m_screen_y);

}

void DialogGUI::SetCoords(int x, int y, int w, int h)
{
	m_screen_x = x;
	m_screen_y = y;
	m_screen_width = w;
	m_screen_height = h;
}

void DialogGUI::SetHighlightedRow(int highlighted_row)
{
	m_highlighted_row = highlighted_row;
}

void DialogGUI::NextHighlightedRow()
{
	if (m_highlighted_row < m_rows_count - 1)
	{
		++m_highlighted_row;
	}
	else
	{
		m_highlighted_row = 0;
	}
}

void DialogGUI::PreviousHighlightedRow()
{
	if (m_highlighted_row > 0)
	{
		--m_highlighted_row;
	}
	else
	{
		m_highlighted_row = m_rows_count - 1;
	}
}

bool DialogGUI::IsHighlightActive() const
{
	return m_highlighted_row >= 0;
}
