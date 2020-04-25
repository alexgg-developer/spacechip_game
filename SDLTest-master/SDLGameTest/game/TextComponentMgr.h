#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

#include "TextureMgr.h"

#include <unordered_map>
#include <functional>

#include <SDL_ttf.h>

using namespace std;

class TextComponentMgr  
{
private :
	struct InstanceData {
		Entity* entity;
		vec2* position;
		uint32_t* width;
		uint32_t* height;
		SDL_Texture** textures;

	};
	InstanceData m_data;
	size_t m_instanceCount, m_capacity;
	unordered_map<uint32_t, Instance> m_map;
	TTF_Font* m_font = nullptr;
	SDL_Color m_textColor;
public:
	inline vec2 getPosition(const Entity& e) const { return m_data.position[lookup(e).i]; }
	inline vec2* const getPositions() const { return m_data.position; }
	inline SDL_Texture** const getTextures() const { return m_data.textures; }
	inline size_t const getSize() const { return m_instanceCount; }
	inline uint32_t* const getWidths() const { return m_data.width; }
	inline uint32_t* const getHeights() const { return m_data.height; }
	inline Instance lookup(const Entity& e) const { return m_map.at(e.index()); }

	void add(const Entity& e, const vec2 & position, const std::string& text, SDL_Renderer * const renderer);
	void clean();
	void destroy(const Entity & e);
	void init(size_t size);
	void reset();
	void setText(const Entity&e, const std::string& newText, SDL_Renderer * const renderer);
};

