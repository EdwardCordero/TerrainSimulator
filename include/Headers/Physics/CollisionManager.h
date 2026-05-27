#pragma once
#include "Physics\RigidBody.h"
#include "Physics\RigidBodyGrid.h"
#include "Graphics\GraphicsManager.h"
#include <vector>
#include <map>
#include <SDL.h>
#include <memory>

class CollisionManager
{
	public:
		CollisionManager();
		~CollisionManager();
		void intialize();
		void update();
		void clearGrid();
		void setDebug(bool debug);
		void addRigidBody(std::shared_ptr<RigidBody> rigidBody);
		void render();
		bool inDebugMode();
		void setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager);

	private:
		std::shared_ptr<GraphicsManager> graphicsManager;
		RigidBodyGrid grid;
		bool debug;
		SDL_Color gridColor;
		SDL_Rect currentScreenSize;
};