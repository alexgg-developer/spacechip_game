#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "TextureMgr.h"

#include <iostream>

using namespace dodf;

TextureMgr::~TextureMgr()
{
	
}

void TextureMgr::clean()
{
	for (size_t i = 0; i < TextureID::SIZE; ++i) {
		SDL_DestroyTexture(textures[i]);
	}
}

int TextureMgr::init(SDL_Renderer* const renderer)
{
	//*textures = static_cast<SDL_Texture*>(MemoryPool::Get(TextureID::SIZE * sizeof(SDL_Texture*)));
	//textures[0] = static_cast<SDL_Texture*>(MemoryPool::Get(TextureID::SIZE * sizeof(SDL_Texture*)));
	textures = static_cast<SDL_Texture**>(MemoryPool::Get(TextureID::SIZE * sizeof(SDL_Texture*)));

	//auto texture = *textures;
	int error = 0;
	error += loadTexture(ENEMY, "Assets/Enemy/spaceship_enemy_red.png", renderer);
	error += loadTexture(PLAYER, "Assets/Player/spaceship.png", renderer);
	error += loadTexture(PLAYER_BULLET, "Assets/Player/bullet.png", renderer);

	return error;
}

int TextureMgr::loadTexture(TextureID textureID, std::string path, SDL_Renderer* const renderer)
{
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (!surface) {
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	textures[textureID] = SDL_CreateTextureFromSurface(renderer, surface);
	if (!textures[textureID]) {
		std::cout << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_FreeSurface(surface);

	return 0;
}

SDL_Texture* TextureMgr::getTexture(TextureID textureID) 
{
	return textures[textureID];
}
