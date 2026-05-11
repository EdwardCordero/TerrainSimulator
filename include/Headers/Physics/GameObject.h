#pragma once
#include "Graphics\Texture.h"
#include "Physics\RigidBody.h"

struct GameObject
{
	Texture texture;
	SDL_Rect position;
};