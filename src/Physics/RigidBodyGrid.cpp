#include "Physics\RigidBodyGrid.h"
#include "Physics\RigidBody.h"
#include "GameLogic\HelperFunctions.h"
#include <vector>
#include <unordered_set>
#include <memory>
#include <SDL.h>

/// <summary>
/// CELLS
/// </summary>
RBGridCell::RBGridCell()
{
	this->rigidBodies.resize(0);
	this->x = 0;
	this->y = 0;
}
//RBGridCell::RBGridCell(int x, int y, std::vector<RigidBody> bodies)
//{
//	this->rigidBodies = bodies;
//	this->x = x;
//	this->y = y;
//}

RBGridCell::~RBGridCell()
{

}

int RBGridCell::getXPos()
{
	return this->x;
}

int RBGridCell::getYPos()
{
	return this->y;
}

void RBGridCell::setCellXPos(int newX)
{
	this->x = newX;
}

void RBGridCell::setCellYPos(int newY)
{
	this->y = newY;
}

std::vector<std::shared_ptr<RigidBody>>& RBGridCell::getRigidBodies()
{
	return this->rigidBodies;
}

void RBGridCell::addRigidBody(std::shared_ptr<RigidBody> body)
{
	this->rigidBodies.push_back(body);
}

void RBGridCell::removeRigidBody(int index)
{
	this->rigidBodies.erase(this->rigidBodies.begin() + index);
}

void RBGridCell::checkPossibleCollision()
{
	for (int i = 0; i < this->rigidBodies.size(); i++)
	{
		auto& currBody = this->rigidBodies[i];
		if (!currBody->isCollidable()) continue;

		for (int j = 0; j < this->rigidBodies.size(); j++)
		{
			if (i == j) continue;
			auto& bodyCollidingInto = this->rigidBodies[j];
			if (!bodyCollidingInto->isActive()) continue;
			if (currBody->getBodyType() == BulletBody && (bodyCollidingInto->getBodyType() == BulletBody || bodyCollidingInto->getBodyType() == PlayerBody)) continue;
			if (currBody->getBodyType() == EnemyBody && bodyCollidingInto->getBodyType() == EnemyBody) continue;
			if (checkCollision(currBody->getRigidBodyRect(), bodyCollidingInto->getRigidBodyRect()))
			{
				//std::cout << std::format("The {} body collided with the {} body", currBody->getBodyTypeStr(), bodyCollidingInto->getBodyTypeStr()) << std::endl;
 				currBody->onCollision(bodyCollidingInto);
			}
		}
	}
}

void RBGridCell::clearCell()
{
	this->rigidBodies.clear();
}


/// <summary>
/// GRID 
/// </summary>
RigidBodyGrid::RigidBodyGrid()
{
	/*this->gridSize = 4;
	this->cellWidth = SCREEN_WIDTH / this->gridSize;
	this->cellHeight = SCREEN_HEIGHT / this->gridSize;

	int NumOfColumns = SCREEN_WIDTH / this->cellWidth;
	int NumOfRows = SCREEN_HEIGHT / this->cellHeight;
	this->cells.resize(NumOfRows * NumOfColumns);

	for (int i = 0; i < this->cells.size(); i++)
	{
		int x = 0 + this->cellWidth * i;
		int y = 0 + this->cellHeight * i;
		cells[i].setCellXPos(x);
		cells[i].setCellYPos(y);
	}*/

	// intialize hased rigid body map
	/*for (int i = 0; i != NullBody; i++)
	{
		RigidBodyType bodyType = static_cast<RigidBodyType>(i);
		this->hashedRigidBodies[bodyType].resize(0);
	}*/
}

RigidBodyGrid::~RigidBodyGrid()
{

}

void RigidBodyGrid::intiailze(SDL_Rect screenSize)
{
	this->currentScreenSize = screenSize;
	this->gridSize = 4;
	this->cellWidth = currentScreenSize.w / this->gridSize;
	this->cellHeight = currentScreenSize.h / this->gridSize;

	int NumOfColumns = currentScreenSize.w / this->cellWidth;
	int NumOfRows = currentScreenSize.h / this->cellHeight;
	this->cells.resize(NumOfRows * NumOfColumns);

	for (int i = 0; i < this->cells.size(); i++)
	{
		int x = 0 + this->cellWidth * i;
		int y = 0 + this->cellHeight * i;
		cells[i].setCellXPos(x);
		cells[i].setCellYPos(y);
	}
}

void RigidBodyGrid::setScreenSize(SDL_Rect screenSize)
{
	this->currentScreenSize = screenSize;
}

void RigidBodyGrid::resizeGrid(int cellWidth, int cellHeight, int xMin, int xMax, int yMin, int yMax)
{
	this->gridSize = (xMax - xMin) / cellWidth;
	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;

	int numOfColumns = (xMax - xMin) / cellWidth;
	int numOfRows = (yMax - yMin) / cellHeight;
	this->cells.resize(numOfRows * numOfColumns);

	for (int i = 0; i < this->cells.size(); i++)
	{
		int x = xMin + (this->cellWidth * i);
		int y = yMin + (this->cellHeight * i);

		cells[i].setCellXPos(x);
		cells[i].setCellYPos(y);
	}
}

void RigidBodyGrid::updateCells()
{
	for (int i = 0; i < this->cells.size(); i++)
	{
		auto& cell = cells[i];
		cell.getRigidBodies().clear();
	}

	for (int i = 0; i < this->rigidBodies.size(); i++)
	{
		std::shared_ptr<RigidBody> body = this->rigidBodies[i];
		if (body->isActive())
		{
			//check if body is within screen, if not then skip for now.
			if (!isRectWithinScreenBorders(this->currentScreenSize, body->getRigidBodyRect()))
			{
				continue;
			}
			std::unordered_set<int> newCellIds = getCellIds(body->getRigidBodyRect());

			std::string idString = "[ ";
			for (int id : newCellIds)
			{
				idString += std::to_string(id) + ", ";
				this->cells[id].addRigidBody(body);
			}
			idString += " ]";
		}
		/*else
		{
			for (int i = 0; i < this->hashedRigidBodies[body->getBodyType()].size(); i++)
			{
				auto rigidBody = this->hashedRigidBodies[body->getBodyType()][i];
				if (rigidBody == body)
				{
					std::cout << std::format("ERASE HERE, found rigid body at {} using address {}", rigidBody, body) << std::endl;
				}
			}
		}*/
	}


}

void RigidBodyGrid::checkCollision()
{
	// loop thru cells
	for (int i = 0; i < this->cells.size(); i++)
	{
		auto cell = cells[i];
		// if cell has more than 2 rigid bodies then check for possible collision
		if (cell.getRigidBodies().size() > 1)
		{
			//std::cout << std::format("Possible Collision Detected at ({}, {})", cell.getXPos(), cell.getYPos()) << std::endl;
			// call cell's check possible collision func
			cell.checkPossibleCollision();
		}
	}
}

void RigidBodyGrid::addRigidBody(std::shared_ptr<RigidBody> rigidBody)
{
	rigidBodies.push_back(rigidBody);
	//this->hashedRigidBodies[rigidBody->getBodyType()].push_back(rigidBody);
}

// Calculates index of cell in my 1-D vector of cells
std::unordered_set<int> RigidBodyGrid::getCellIds(SDL_FRect bodyRect)
{
	std::unordered_set<int> collisionPoints;

	if (!isRectWithinScreenBorders(this->currentScreenSize, bodyRect))
	{
		std::cout << "Rect not in screen in getCellsIds function" << std::endl;
		return collisionPoints;
	}

	// get inbound x and y, if out of bound else set to bodyRect position
	float startingX = bodyRect.x;
	float endingX = (bodyRect.x + bodyRect.w) > currentScreenSize.w ? currentScreenSize.w - 1 : (bodyRect. x + bodyRect.w);

	float startingY = bodyRect.y < 0 ? 0 : bodyRect.y;
	float endingY = (bodyRect.y + bodyRect.h) > currentScreenSize.h ? currentScreenSize.h - 1 : (bodyRect.y + bodyRect.h);

	// Calculate index of cell in my 1-D vector of cells
	int topLeftX = startingX / this->cellWidth;
	int topLeftY = startingY / this->cellHeight;
	int startingCell = topLeftY * this->gridSize + topLeftX;

	int topRightX = endingX / this->cellWidth;
	int topRightY = startingY / this->cellHeight;
	int topRightID = topRightY * this->gridSize + topRightX;

	int bottomLeftX = startingX / this->cellWidth;
	int bottomLeftY = endingY / this->cellHeight;
	int bottomLeftID = bottomLeftY * this->gridSize + bottomLeftX;

	int bottomRightX = endingX / this->cellWidth;
	int bottomRightY = endingY / this->cellHeight;
	int bottomRightID = bottomRightY * this->gridSize + bottomRightX;
	
	int numOfRows = bottomLeftY - topLeftY;
	for (int currentRow = 0; currentRow <= numOfRows; currentRow++)
	{
		int rowStart = startingCell + (this->gridSize * currentRow);
		int rowMax = topRightID + (this->gridSize * currentRow);
		while (rowStart <= rowMax)
		{
			if (rowStart >= this->cells.size())
			{
				break;
			}
			collisionPoints.insert(rowStart);

			rowStart++;
		}
	}

	return collisionPoints;
}

int RigidBodyGrid::getGridSize()
{
	return this->gridSize;
}

int RigidBodyGrid::getCellWidth()
{
	return this->cellWidth;
}

int RigidBodyGrid::getCellHeight()
{
	return this->cellHeight;
}

void RigidBodyGrid::clearGrid()
{
	this->rigidBodies.clear();
}