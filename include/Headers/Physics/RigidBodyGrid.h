#pragma once
#include "Physics\RigidBody.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include <SDL.h>
#include <map>

class RBGridCell
{
	public:
		RBGridCell();
		~RBGridCell();
		int getXPos();
		int getYPos();
		void setCellXPos(int newX);
		void setCellYPos(int newY);
		std::vector<std::shared_ptr<RigidBody>>& getRigidBodies();
		void addRigidBody(std::shared_ptr<RigidBody> body);
		void removeRigidBody(int index);
		void checkPossibleCollision();
		void clearCell();
	private:
		int x;
		int y;
		std::vector<std::shared_ptr<RigidBody>> rigidBodies;
};

class RigidBodyGrid
{
	public:
		RigidBodyGrid();
		~RigidBodyGrid();
		void intiailze(SDL_Rect screenSize);
		void addRigidBody(std::shared_ptr<RigidBody> rigidBody);
		void updateCells();
		void checkCollision();
		void clearGrid();
		int getCellWidth();
		int getCellHeight();
		std::unordered_set<int> getCellIds(SDL_FRect bodyRect);
		int getGridSize();

		void setScreenSize(SDL_Rect screenSize);
		void resizeGrid(int cellWidth, int cellHeight, int xMin, int xMax, int yMin, int yMax);
	private:
		std::vector<RBGridCell> cells;
		int gridSize;
		int cellWidth;
		int cellHeight;
		std::vector<std::shared_ptr<RigidBody>> rigidBodies;
		SDL_Rect currentScreenSize;
};