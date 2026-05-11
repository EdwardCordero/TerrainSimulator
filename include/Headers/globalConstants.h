#pragma once
#include "Graphics\Texture.h"
#include "Player\Player.h"
#include "UserInterface.h"
#include "Factories\EnemyFactory.h"
#include "Utils\Stopwatch.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <chrono>
#include <random>
#include <filesystem>

const bool RENDER_HITBOXES = true;
extern bool QUIT;
// For random numbers
extern std::mt19937 gRandGen;