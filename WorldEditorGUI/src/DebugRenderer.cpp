#include "DebugRenderer.h"
#include "Log.h"

DebugRenderer::DebugRenderer()
{

}
DebugRenderer& DebugRenderer::get()
{
	static DebugRenderer debug_renderer;
	return debug_renderer;
}
void DebugRenderer::AddDebugLine(const std::string& debug_line_key, const std::string& debug_line, SDL_Renderer* renderer, const SDL_Color color /*= { 0,0,0 }*/)
{
	if (!m_debug_font)
	{
		ERROR("Debug font in DebugRender is not correctly set!");
		ERROR("Unable to append debug line");
		return;
	}
	int map_index = m_debug_lines_textures.size() + 1;
	if(m_debug_lines_textures.find(debug_line_key) != m_debug_lines_textures.end())
	{
		map_index = m_debug_lines_textures[debug_line_key].first;
	}

	m_debug_lines.emplace_back(debug_line);
	
	m_debug_lines_textures[debug_line_key] = std::make_pair(map_index, std::make_unique<Texture>());
	m_debug_lines_textures[debug_line_key].second->loadFromRenderedText(renderer, debug_line, color, m_debug_font);

	/*m_debug_lines_textures.emplace_back(std::make_unique<Texture>());
	m_debug_lines_textures.back()->loadFromRenderedText(renderer, debug_line, color, m_debug_font);*/
}
void DebugRenderer::SetFont(TTF_Font* debug_font)
{
	m_debug_font = debug_font;
}
void DebugRenderer::SetYOffset(const int y_offset)
{
	m_y_offset_inc = y_offset;
}
void DebugRenderer::SetX0(const int x0)
{
	m_x0 = x0;
}
void DebugRenderer::SetY0(const int y0)
{
	m_y0 = y0;
}
void DebugRenderer::OnRender(SDL_Renderer* renderer)
{
	if (!m_debug_font)
	{
		ERROR("Debug font in DebugRender is not correctly set!");
		return;
	}
	/*int y_offset = 0;
	for (auto&& debug_line_texture : m_debug_lines_textures)
	{
		debug_line_texture.second->Render(m_x0, m_y0 + y_offset);
		y_offset += m_y_offset_inc;
	}*/

	for (auto&& id_texture_pair : m_debug_lines_textures)
	{
		int y = id_texture_pair.second.first * m_y_offset_inc;
		id_texture_pair.second.second->Render(m_x0, y);
	}
}