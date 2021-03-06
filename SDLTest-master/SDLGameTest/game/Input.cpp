#include "Input.h"

#include <iostream>

Input::Input()
{
	for (size_t i = 0; i < nkeys; ++i) keys[i] = KEY_OFF;
}

void Input::read(SDL_Event const & event)
{
	//Keys RELEASED become OFF
	//Keys PRESSED become ON
	//It is in case that a key stay pressed it doesn't poll and event so you have to be aware of it
	for (size_t i = 0; i < nkeys; ++i) keys[i] = keys[i] & 0x0001;
	switch (event.type) {
	case SDL_KEYDOWN:
		if (event.key.repeat) break;
		switch (event.key.keysym.sym) {
		case SDLK_UP:
			keys[KUP] = KEY_PRESSED;
			break;
		case SDLK_DOWN:
			keys[KDOWN] = KEY_PRESSED;
			break;
		case SDLK_RIGHT:
			keys[KRIGHT] = KEY_PRESSED;
			break;
		case SDLK_LEFT:
			keys[KLEFT] = KEY_PRESSED;
			break;
		case SDLK_w:
			keys[KW] = KEY_PRESSED;
			break;
		case SDLK_a:
			keys[KA] = KEY_PRESSED;
			break;
		case SDLK_s:
			keys[KS] = KEY_PRESSED;
			break;
		case SDLK_d:
			keys[KD] = KEY_PRESSED;
			break;
		case SDLK_ESCAPE:
			keys[KESC] = KEY_PRESSED;
			break;
		case SDLK_SPACE:
			keys[KSPACE] = KEY_PRESSED;
			break;
		case SDLK_RETURN:
			keys[KENTER] = KEY_PRESSED;
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym) {
		case SDLK_UP:
			keys[KUP] = KEY_RELEASED;
			break;
		case SDLK_DOWN:
			keys[KDOWN] = KEY_RELEASED;
			break;
		case SDLK_RIGHT:
			keys[KRIGHT] = KEY_RELEASED;
			break;
		case SDLK_LEFT:
			keys[KLEFT] = KEY_RELEASED;
			break;
		case SDLK_w:
			keys[KW] = KEY_RELEASED;
			break;
		case SDLK_a:
			keys[KA] = KEY_RELEASED;
			break;
		case SDLK_s:
			keys[KS] = KEY_RELEASED;
			break;
		case SDLK_d:
			keys[KD] = KEY_RELEASED;
			break;
		case SDLK_ESCAPE:
			keys[KESC] = KEY_RELEASED;
			break;
		case SDLK_SPACE:
			keys[KSPACE] = KEY_RELEASED;
			break;
		case SDLK_RETURN:
			keys[KENTER] = KEY_RELEASED;
			break;
		}
		break;	
	case SDL_QUIT:
		keys[KESC] = KEY_ON;
		break;
	}
}


bool Input::check(unsigned int key) const
{
	return keys[key] == KEY_ON || keys[key] == KEY_PRESSED;
}

bool Input::checkPressed(unsigned int key) const
{
 	return keys[key] == KEY_PRESSED;
}

bool Input::checkReleased(unsigned int key) const
{
	return keys[key] == KEY_RELEASED;
}


