#include "Physics\CollisionManager.h"
#include "Physics\RigidBodyGrid.h"
#include "globalConstants.h"
#include <SDL.h>
#include <vector>

CollisionManager::CollisionManager()
{
	//this->grid = RigidBodyGrid();
	this->debug = false;
	this->gridColor = { 0, 100, 255, 0 };
	this->graphicsManager = nullptr;
	this->currentScreenSize = { 0,0,0,0 };
}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::intialize()
{
	this->grid.intiailze(this->currentScreenSize);
}

void CollisionManager::setDebug(bool debug)
{
	this->debug = debug;
}

void CollisionManager::update()
{
	this->grid.updateCells();
	this->grid.checkCollision();
}

void CollisionManager::clearGrid()
{
	this->grid.clearGrid();
}

void CollisionManager::addRigidBody(std::shared_ptr<RigidBody> rigidBody)
{
	this->grid.addRigidBody(rigidBody);
}

void CollisionManager::render()
{
	if (this->debug)
	{
		int cellWidth = this->grid.getCellWidth();
		int cellHeight = this->grid.getCellHeight();
		int gridSize = this->grid.getGridSize();
		int borderSize = 10;
		std::vector<SDL_Rect> horizontalLines;
		std::vector<SDL_Rect> verticalLines;
		for (int i = 1; i < gridSize; i++)
		{
			//SDL_SetRenderDrawColor(gRenderer, gridColor.r, gridColor.g, gridColor.b, gridColor.a);
			SDL_Rect horizontalLine = { 0, (cellHeight * i), currentScreenSize.w, borderSize };
			SDL_Rect verticalLine = { (cellWidth * i), 0, borderSize, currentScreenSize.h };
			horizontalLines.push_back(horizontalLine);
			verticalLines.push_back(verticalLine);
		}

		for (int i = 0; i < gridSize - 1; i++)
		{
			/*SDL_RenderDrawRect(gRenderer, &horizontalLines[i]);
			SDL_RenderDrawRect(gRenderer, &verticalLines[i]);*/
		}

		//SDL_RenderPresent(gRenderer);
	}
}

bool CollisionManager::inDebugMode()
{
	return this->debug;
}

void CollisionManager::setGraphicsManager(std::shared_ptr<GraphicsManager> graphicsManager)
{
	this->graphicsManager = graphicsManager;
	this->currentScreenSize = this->graphicsManager->getScreenRect();
}