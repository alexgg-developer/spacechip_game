#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "TextComponentMgr.h"


using namespace dodf;


void TextComponentMgr::init(size_t size)
{
	m_data.position = static_cast<vec2*>(MemoryPool::Get(size * sizeof(vec2)));
	m_data.width = static_cast<uint32_t*>(MemoryPool::Get(size * sizeof(uint32_t)));
	m_data.height = static_cast<uint32_t*>(MemoryPool::Get(size * sizeof(uint32_t)));
	m_data.textures = static_cast<SDL_Texture**>(MemoryPool::Get(size * sizeof(SDL_Texture*)));
	m_data.entity = static_cast<Entity*>(MemoryPool::Get(size * sizeof(Entity)));

	m_font = TTF_OpenFont("Assets/fonts/lazy.ttf", 28);
	if (!m_font) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	
	m_textColor.r = 255;
	m_textColor.g = 255;
	m_textColor.b = 255;
	m_textColor.a = 255;

}

void TextComponentMgr::setText(const Entity & e, const std::string & newText, SDL_Renderer * const renderer)
{
	Instance instance = lookup(e);
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, newText.c_str(), m_textColor);
	if (!textSurface) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		SDL_DestroyTexture(m_data.textures[instance.i]);
		m_data.textures[instance.i] = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (!m_data.textures[instance.i]) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			m_data.width[instance.i] = textSurface->w;
			m_data.height[instance.i] = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
}

void TextComponentMgr::add(const Entity& e, const vec2 & position, const std::string& text, SDL_Renderer * const renderer)
{
	auto instance = Instance::create(m_instanceCount);
	m_map[e.index()] = instance;
	m_data.entity[instance.i] = e;
	m_data.position[instance.i] = position;
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, text.c_str(), m_textColor);
	if (!textSurface) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else {
		m_data.textures[instance.i] = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (!m_data.textures[instance.i]) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			m_data.width[instance.i] = textSurface->w;
			m_data.height[instance.i] = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	++m_instanceCount;
}

void TextComponentMgr::clean()
{
	for (size_t i = 0; i < m_instanceCount; ++i) {
		SDL_DestroyTexture(m_data.textures[i]);
	}
}


void TextComponentMgr::destroy(const Entity & e)
{
	Instance i = lookup(e);
	size_t last = m_instanceCount - 1;

	SDL_DestroyTexture(m_data.textures[i.i]);

	Entity lastEntity = m_data.entity[last];
	m_data.entity[i.i] = m_data.entity[last];
	m_data.position[i.i] = m_data.position[last];
	m_data.width[i.i] = m_data.width[last];
	m_data.height[i.i] = m_data.height[last];
	m_data.textures[i.i] = m_data.textures[last];

	m_map[lastEntity.index()] = i;
	m_map.erase(e.index());

	--m_instanceCount;
}


