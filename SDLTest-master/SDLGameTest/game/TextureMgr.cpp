#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "TextureMgr.h"

#include <iostream>

using namespace dodf;

void TextureMgr::clean()
{
	for (size_t i = 0; i < TextureID::SIZE; ++i) {
		SDL_DestroyTexture(m_textures[i]);
	}
}

int TextureMgr::init(SDL_Renderer* const renderer)
{
	m_textures = static_cast<SDL_Texture**>(MemoryPool::Get(TextureID::SIZE * sizeof(SDL_Texture*)));
	m_animations = static_cast<SDL_Texture**>(MemoryPool::Get(AnimationID::SIZE_ANIM * sizeof(SDL_Texture*) * MAX_FRAMES));
	m_animationFrames = static_cast<size_t*>(MemoryPool::Get(AnimationID::SIZE_ANIM * sizeof(size_t)));
	m_animationsWidth = static_cast<int*>(MemoryPool::Get(AnimationID::SIZE_ANIM * sizeof(int)));
	m_animationsHeight = static_cast<int*>(MemoryPool::Get(AnimationID::SIZE_ANIM * sizeof(int)));
	
	int error = 0;
	error += loadTexture(TextureID::ENEMY, "Assets/Enemy/spaceship_enemy_red.png", renderer);
	error += loadTexture(TextureID::PLAYER, "Assets/Player/spaceship.png", renderer);
	error += loadTexture(TextureID::PLAYER_BULLET, "Assets/Player/bullet.png", renderer);
	error += loadTexture(TextureID::ENEMY_BULLET, "Assets/Enemy/bullet_enemy.png", renderer);
	error += loadTexture(TextureID::OBSTACLE, "Assets/Aestroids/aestroid_brown.png", renderer);

	error += loadAnimation(AnimationID::PLAYER_ANIM, "Assets/Player/Animation/", "", 1, 8, renderer);
	error += loadAnimation(AnimationID::EXPLOSION_ANIM, "Assets/Effects/Galaxy/", "galaxy_", 0, 17, renderer);
	error += loadAnimation(AnimationID::ENEMY_EXPLOSION_ANIM, "Assets/Effects/Red Explosion/", "1_", 0, 17, renderer);

	return error;
}

int TextureMgr::loadTexture(TextureID textureID, const std::string& path, SDL_Renderer* const renderer)
{
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (!surface) {
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	m_textures[textureID] = SDL_CreateTextureFromSurface(renderer, surface);
	if (!m_textures[textureID]) {
		std::cout << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return 1;
	}
	SDL_FreeSurface(surface);

	return 0;
}

int TextureMgr::loadAnimation(
	AnimationID animationID, 
	const std::string& path, 
	const std::string& filename, 
	size_t firstIndex, 
	size_t frames, 
	SDL_Renderer* const renderer)
{
	std::string basePath = path + filename;
	for (size_t i = 0; i < frames; ++i) {
		std::string completePath = basePath + std::to_string(firstIndex + i) + ".png";
		SDL_Surface *surface = IMG_Load(completePath.c_str());
		if (!surface) {
			std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
			return 1;
		}
		size_t currentIndex = animationID * MAX_FRAMES + i;
		m_animations[currentIndex] = SDL_CreateTextureFromSurface(renderer, surface);
		if (!m_animations[currentIndex]) {
			std::cout << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
			SDL_FreeSurface(surface);
			return 1;
		}
		if (i == 0) {
			m_animationsWidth[animationID] = surface->w;
			m_animationsHeight[animationID] = surface->h;
		}

		SDL_FreeSurface(surface);
	}
	m_animationFrames[animationID] = frames;
	return 0;
}

