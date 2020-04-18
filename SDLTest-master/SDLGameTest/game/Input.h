#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#include <SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

#include "Vec.h"

class Input
{
public:
	enum keysDefined { KUP, KDOWN, KRIGHT, KLEFT, KW, KA, KS, KD, KESC, KSPACE, nkeys };

	Input();
	void read(SDL_Event const & event);
	bool check(unsigned int key) const;
	bool checkPressed(unsigned int key)	;
	bool checkReleased(unsigned int key);

private:
	//KEY_OFF: The key hasn't been pressed
	//KEY_ON: The key is being pressed
	//KEY_RELEASED: The key has just been released
	//KEY_PRESSED: The key has just been pressed
	enum keyStatus { KEY_OFF, KEY_ON, KEY_RELEASED, KEY_PRESSED };
	unsigned int keys[nkeys];

};