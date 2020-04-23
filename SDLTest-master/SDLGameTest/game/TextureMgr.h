#pragma once

#include "Vec.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#include <SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

#include <string>

class TextureMgr  
{
public:
	enum TextureID: size_t {
		ENEMY,
		PLAYER,
		PLAYER_BULLET,		
		ENEMY_BULLET,
		OBSTACLE,
		SIZE
	};

	void clean();
	SDL_Texture* getTexture(TextureID textureID);
	inline SDL_Texture** getTextures() { return m_textures; }
	int init(SDL_Renderer* const renderer);

private:
	SDL_Texture** m_textures;

	int loadTexture(TextureID textureID, std::string path, SDL_Renderer* const renderer);

};

