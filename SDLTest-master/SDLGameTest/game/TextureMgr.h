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
	enum TextureID: uint32_t {
		ENEMY,
		PLAYER,
		PLAYER_BULLET,		
		ENEMY_BULLET,
		OBSTACLE,
		SIZE
	};

	enum AnimationID : uint32_t {
		PLAYER_ANIM,
		EXPLOSION_ANIM,
		ENEMY_EXPLOSION_ANIM,
		SIZE_ANIM
	};

	inline size_t getFrames(AnimationID animationID) const { return m_animationFrames[animationID]; }
	inline SDL_Texture* getFrame(AnimationID animationID, size_t frame) const { return m_animations[animationID * MAX_FRAMES + frame]; }
	inline int getWidth(AnimationID animationID) const { return m_animationsWidth[animationID]; }
	inline int getHeight(AnimationID animationID) const { return m_animationsHeight[animationID]; }
	inline SDL_Texture* const getTexture(TextureID textureID) const { return m_textures[textureID]; }
	inline SDL_Texture** getTextures() const { return m_textures; }
	
	void clean();
	int init(SDL_Renderer* const renderer);

private:
	static const size_t MAX_FRAMES = 20;
	SDL_Texture** m_textures;
	SDL_Texture** m_animations;
	size_t* m_animationFrames;
	int* m_animationsWidth;
	int* m_animationsHeight;

	int loadTexture(TextureID textureID, const std::string& path, SDL_Renderer* const renderer);
	int loadAnimation(
		AnimationID animationID,
		const std::string& path,
		const std::string& filename,
		size_t firstIndex,
		size_t frames,
		SDL_Renderer* const renderer);

};

