#include "GameLogic\HelperFunctions.h"
#include "globalConstants.h"
#include <random>
#include <GameLogic/Enums/LimitEnum.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string>
#include <iostream>
#include <nlohmann\json.hpp>

int randNumBetween(int min, int max)
{
	std::uniform_int_distribution<> dis(min, max);
	return dis(gRandGen);
}

float randFloatBetween(float min, float max)
{
	std::uniform_int_distribution<> dis(min, max);
	return dis(gRandGen);
}

float randNumInFloatVector(std::vector<float> numbers)
{
	std::uniform_int_distribution<> dis(0, numbers.size() - 1);
	return numbers[dis(gRandGen)];
}

int randNumInVector(std::vector<int> numbers)
{
	std::uniform_int_distribution<> dis(0, numbers.size() - 1);
	return numbers[dis(gRandGen)];
}

std::string FormatUint32(Uint32 ticks)
{
	Uint32 totalSeconds = ticks / 1000;  // Convert milliseconds to seconds
	Uint32 hours = totalSeconds / 3600;   // Extract hours
	Uint32 minutes = (totalSeconds % 3600) / 60;  // Extract remaining minutes
	Uint32 seconds = totalSeconds % 60;   // Extract remaining seconds

	return std::format("{:02}:{:02}:{:02}", hours, minutes, seconds);
}

int getCenterX(int parentWidth, int width)
{
	return (parentWidth / 2) - (width / 2);
}

int getCenterY(int parentHeight, int height)
{
	return (parentHeight / 2 - height);
}

bool checkCollision(SDL_FRect a, SDL_FRect b)
{
	float leftA, topA, rightA, bottomA;
	float leftB, topB, rightB, bottomB;

	// sides of A rect
	leftA = a.x;
	topA = a.y;
	rightA = a.x + a.w;
	bottomA = a.y + a.h;

	// sides of B rect
	leftB = b.x;
	topB = b.y;
	rightB = b.x + b.w;
	bottomB = b.y + b.h;

	if (leftA >= rightB || topA >= bottomB || rightA <= leftB || bottomA <= topB)
	{
		return false;
	}

	return true;
}

Limit whichSideCollided(SDL_FRect a, SDL_FRect b)
{
	int leftA, topA, rightA, bottomA;
	int leftB, topB, rightB, bottomB;

	// sides of A rect
	leftA = a.x;
	topA = a.y;
	rightA = a.x + a.w;
	bottomA = a.y + a.h;

	// sides of B rect
	leftB = b.x;
	topB = b.y;
	rightB = b.x + b.w;
	bottomB = b.y + b.h;
	int minDiff;
	//std::string side = "none";
	Limit side;
	if (leftA <= rightB) //113
	{
		minDiff = abs(leftA - rightB);
		//side = "left";
		side = LEFT_BORDER;
	}
	if (topA <= bottomB) //44
	{
		if (minDiff > abs(topA - bottomB))
		{
			minDiff = abs(topA - bottomB);
			//side = "top";
			side = TOP_BORDER;
		}
	}
	if (rightA >= leftB) // 1
	{
		if (minDiff > abs(rightA - leftB))
		{
			minDiff = abs(rightA - leftB);
			//side = "right";
			side = RIGHT_BORDER;
		}
	}
	if (bottomA >= topB) // 52
	{
		if (minDiff > abs(bottomA - topB))
		{
			minDiff = abs(bottomA - topB);
			//side = "bottom";
			side = BOTTOM_BORDER;
		}
	}

	return side;
}

float shiftPointsWithinLimit(std::array<float, 2>& currentPos, std::array<float, 2>& destination, std::array<float, 2>& startPos, Limit limitHit, float width, float height, int screenWidth, int screenHeight)
{
	/* need to know what limit was hit left right or up down ?
	BC: tells us what math to do 
		OutOfBoundDiff = (point/destination - TOP_OF_SCREEN)
		topLimit/leftLimit = ADD OutOfBoundDiff to point that is destination & ADD OutOfBoundDiff to other point & UPDATE destination with updated point
		lowLimit/rightimit = SUBTRACT OutOfBoundDiff to point that is destination & SUBTRACT OutOfBoundDiff to other point & UPDATE destination with updated point	
	/*/
	int diff = 0;
	int offset = 0;
	if (limitHit == TOP_BORDER)
	{
		diff = currentPos[1] - destination[1];
		destination[1] = 0;// += abs(diff) + offset;
		startPos[1] += abs(diff);
		currentPos[1] += offset;
	}
	else if (limitHit == LEFT_BORDER)
	{
		diff = currentPos[0] - destination[0];
		destination[0] = 0;// += abs(diff) + offset;
		startPos[0] += abs(diff);
		currentPos[0] += offset;
	}
	else if (limitHit == BOTTOM_BORDER)
	{
		diff = (destination[1] - currentPos[1]) - height;
		destination[1] = screenHeight - height;// abs(diff) + offset;
		startPos[1] -= abs(diff);
		currentPos[1] -= offset;
	}
	else if (limitHit == RIGHT_BORDER)
	{
		diff = (destination[0] - currentPos[0]) - width;
		destination[0] = (screenWidth - width);//abs(diff) + offset;
		startPos[0] -= abs(diff);
		currentPos[0] -= offset;
	}

	return diff;
}

Limit isOutOfBound(float x, float y, float topOffset, float rightOffset, float bottomOffset, float leftOffset, int screenWidth, int screenHeight)
{
	if (y - topOffset <= 0)
	{
		std::cout << "Enemy hit TOP_BORDER" << std::endl;
		return TOP_BORDER;
	}
	else if (y + bottomOffset >= screenHeight)
	{
		std::cout << "Enemy hit BOTTOM_BORDER" << std::endl;
		return BOTTOM_BORDER;
	}

	if (x - leftOffset <= 0)
	{
		std::cout << "Enemy hit LEFT_BORDER" << std::endl;
		return LEFT_BORDER;
	}
	else if (x + rightOffset >= screenWidth)
	{
		std::cout << "Enemy hit RIGHT_BORDER" << std::endl;
		return RIGHT_BORDER;
	}

	return NO_BORDER;
}

float lerp(float start, float end, float t)
{
	float result = start + (end - start) * t;
	return result;
}

/*
* Returns corners of a rect [topLeft, topRight, bottomRight, bottomLeft]
*/
std::array<std::array<float, 2>, 4> getCornersOfRect(SDL_FRect rect)
{
	std::array<std::array<float, 2>, 4> corners = { { {0, 0}, {0, 0}, {0, 0}, {0, 0} } };

	float topLeftX = rect.x;
	float topLeftY = rect.y;

	float topRightX = rect.x + rect.w;
	float topRightY = rect.y;

	float bottomLeftX = rect.x;
	float bottomLeftY = rect.y + rect.h;

	float bottomRightX = topRightX;
	float bottomRightY = bottomLeftY;

	corners[0][0] = topLeftX;
	corners[0][1] = topLeftY;

	corners[1][0] = topRightX;
	corners[1][1] = topRightY;

	corners[2][0] = bottomRightX;
	corners[2][1] = bottomRightY;

	corners[3][0] = bottomLeftX;
	corners[3][1] = bottomLeftY;
	
	return corners;
}

std::array<float,2> MoveRectBetweenPoints(float deltaTime, SDL_FRect rect, std::array<float, 2>& startPos, std::array<float, 2>& destination, float& movementProgress, float speed)
{
	std::array<float, 2> newPosition = { rect.x, rect.y };
	float distanceLeft = calculateDistance(startPos, destination);
	float totalTime = distanceLeft / speed;
	if (movementProgress < 1.0f)
	{
		movementProgress += deltaTime / totalTime;
		if (movementProgress > 1.0f) movementProgress = 1.0f;
		float newX = lerp(startPos[0], destination[0], movementProgress);
		float newY = lerp(startPos[1], destination[1], movementProgress);
		newPosition[0] = newX;
		newPosition[1] = newY;
	}
	else
	{
		movementProgress = 0.0f;
		newPosition = destination;
		std::array<float, 2> originalStartPosition = startPos;
		startPos = newPosition;
		destination = originalStartPosition;
	}
	/*int anchor = 0;
	if (isVertical)
	{
		anchor = enemy.getAnchorX();
	}
	else
	{
		anchor = enemy.getAnchorY();
	}*/
	return newPosition;
	//SinStrafe(enemy, anchor);
}

float calculateDistance(std::array<float, 2> start, std::array<float, 2> end)
{
	return sqrt((end[0] - start[0]) * (end[0] - start[0]) + (end[1] - start[1]) * (end[1] - start[1]));
}

float calculateDistance(float x1, float x2, float y1, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float getClosestDistance(SDL_FRect objectRect, SDL_FRect targetRect)
{
	float closestDistance = 0;

	/* Distance between Object Rect Top ----> Target Rect Bottom  */
	float objTopX = objectRect.x + (objectRect.w / 2);
	float objTopY = objectRect.y;

	float tarBottomX = targetRect.x + (targetRect.w / 2);
	float tarBottomY = targetRect.y + targetRect.h;

	closestDistance = calculateDistance(objTopX, tarBottomX, objTopY, tarBottomY);

	/* Distance between Object Rect Right  ----> Target Rect Left  */
	float objRightX = objectRect.x + objectRect.w;
	float objRightY = objectRect.y + (objectRect.h / 2);

	float tarLeftX = targetRect.x;
	float tarLeftY = targetRect.y + (objectRect.h / 2);

	float rightToLeftDistance = calculateDistance(objRightX, tarLeftX, objRightY, tarLeftY);
	closestDistance = rightToLeftDistance < closestDistance ? rightToLeftDistance : closestDistance;

	/* Distance between Object Rect Bottom ----> Target Rect Top  */
	float objBottomX = objectRect.x + (objectRect.w / 2);
	float objBottomY = objectRect.y + objectRect.h;

	float tarTopX = targetRect.x + (objectRect.w / 2);
	float tarTopY = targetRect.y;

	float bottomToTopDistance = calculateDistance(objBottomX, tarTopX, objBottomY, tarTopY);
	closestDistance = bottomToTopDistance < closestDistance ? bottomToTopDistance : closestDistance;

	/* Distance between Object Rect Left ----> Target Rect Right  */
	float objLeftX = objectRect.x;
	float objLeftY = objectRect.y + (objectRect.h / 2);

	float tarRightX = targetRect.x + targetRect.w;
	float tarRightY = targetRect.y + (targetRect.h / 2);

	float leftToRightDistance = calculateDistance(objLeftX, tarRightX, objLeftY, tarRightY);
	closestDistance = leftToRightDistance < closestDistance ? leftToRightDistance : closestDistance;

	return closestDistance;
}


std::array<float, 2> MoveRectAroundTarget(float deltaTime, SDL_FRect rect, std::array<std::array<float, 2>, 4>& targetCorners, std::array<float, 2>& startPos, std::array<float, 2>& destination, float& movementProgress, float speed, bool clockwise)
{
	std::array<float, 2> newPosition = { rect.x, rect.y };
	std::array<float, 2> originalDestination = destination;
	int destinationCornerNum = getCornerNumber(targetCorners, destination);

	float distanceLeft = calculateDistance(startPos, destination);
	float totalTime = distanceLeft / speed;
	// Logic below checks if destination was reached, if so update destination to next corner and set start pos to current location.
	if (movementProgress < 1.0f)
	{
		movementProgress += deltaTime / totalTime;
		if (movementProgress > 1.0f) movementProgress = 1.0f;
		float newX = lerp(startPos[0], destination[0], movementProgress);
		float newY = lerp(startPos[1], destination[1], movementProgress);
		//std::cout << std::format("Delta time: {}, movementProgress: {}, X pos: {}, Y pos: {}", gTimeDeltaSeconds, movementProgress, newX, newY) << std::endl;
		newPosition[0] = newX;
		newPosition[1] = newY;
	}
	else
	{
		movementProgress = 0;
		startPos = destination;
		newPosition = destination;

		if (clockwise)
		{
			int newDestinationCorner = destinationCornerNum + 1 > 3 ? 0 : destinationCornerNum + 1;
			destination = targetCorners[newDestinationCorner];
		}
		else
		{
			int newDestinationCorner = destinationCornerNum - 1 < 0 ? 3 : destinationCornerNum - 1;
			destination = targetCorners[newDestinationCorner];
		}
	}

	return newPosition;
}

int getCornerNumber(std::array<std::array<float, 2>, 4> targetCorners, std::array<float, 2> destination)
{
	if (destination == targetCorners[0])
	{
		return 0;
	}
	else if (destination == targetCorners[1])
	{
		return 1;
	}
	else if (destination == targetCorners[2])
	{
		return 2;
	}
	else if (destination == targetCorners[3])
	{
		return 3;
	}
	return -1;
}


float CosStrafe(float delatTime, float& angle, float anchor, float speed)
{
	float freq = (2 * M_PI) * speed;
	float amp = 50.0f;
	angle += freq * delatTime;
	if (angle > 2 * M_PI)
	{
		angle -= 2 * M_PI;
	}
	float offset = amp * cos(angle);
	return anchor + offset;
}

float SinStrafe(float delatTime, float& angle, float anchor, float speed)
{
	float freq = (2 * M_PI) * speed;
	float amp = 50.0f;
	angle += freq * delatTime;
	if (angle > 2 * M_PI)
	{
		angle -= 2 * M_PI;
	}
	float offset = amp * sin(angle);
	return anchor + offset;
}

std::vector<std::string> WrapTextWithinWidth(TTF_Font* font, std::string text, int wrapSize, bool center)
{
	std::vector<std::string> lines;
	std::string currentLine;
	std::string word;
	std::istringstream stream(text);

	while (stream >> word)
	{
		int width, height;
		std::string tempString = currentLine.empty() ? word : currentLine + ' ' + word;
		TTF_SizeText(font, tempString.c_str(), &width, &height);
		bool outOfBounds = false;
		if (center)
		{
			int startX = (wrapSize / 2) - (width / 2);
			outOfBounds = (startX + width) > wrapSize;
		}
		else
		{
			outOfBounds = width > wrapSize;
		}
		if (outOfBounds)
		{
			lines.push_back(currentLine);
			currentLine = word;
		}
		else
		{
			currentLine += ' ' + word;
		}
	}
	if (!currentLine.empty())
	{
		lines.push_back(currentLine);
	}

	return lines;
}

std::vector<std::string> getFileContents(std::string pathToFile)
{
	std::vector<std::string> lines;
	std::ifstream file(pathToFile);

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
		file.close();
	}
	else
	{
		std::cerr << "Error openning file " << pathToFile << std::endl;
	}

	return lines;
}

std::string padString(std::string& str, int targetSize) {
	if (str.size() >= targetSize) return str;

	return str + std::string(targetSize - str.size(), ' ');
}

void writeToLeaderboard(std::string game, std::string str)
{
	std::string pathToFile = std::format("{}_Leaderboard.txt", game);
	std::ofstream outFile(pathToFile, std::ios::app);
	if (outFile.is_open())
	{
		outFile << str;
		outFile.close();
	}
	else
	{
		std::cerr << "Error writing to file " << pathToFile << std::endl;
	}
}

bool isRectWithinScreenBorders(SDL_Rect screenSize, SDL_FRect rect)
{
	// if widest part of rect is before screen, invalid
	if (rect.x + rect.w < screenSize.x)
	{
		return false;
	}
	// if furthest back of rect is after screen, invalid
	if (rect.x > (screenSize.x + screenSize.w))
	{
		return false;
	}

	// if lowest point of rect is above screen, invlaid
	if (rect.y + rect.h < screenSize.y)
	{
		return false;
	}

	// if tallest point of rect is below screen, invalid
	if (rect.y > (screenSize.y + screenSize.h))
	{
		return false;
	}
	return true;
}

std::string jsonToString(const nlohmann::json& val)
{
	if (val.is_null()) {
		return "null";
	}
	if (val.is_boolean()) {
		return val.get<bool>() ? "true" : "false";
	}
	if (val.is_number_integer()) {
		return std::to_string(val.get<int>());
	}
	if (val.is_number_unsigned()) {
		return std::to_string(val.get<unsigned int>());
	}
	if (val.is_number_float()) {
		return std::to_string(val.get<double>());
	}
	if (val.is_string()) {
		return val.get<std::string>();
	}
	if (val.is_array() || val.is_object()) {
		// Serialize nested JSON back to string
		return val.dump();
	}

	return "";
}

std::vector<std::string> splitString(const std::string& str, char charToSplitOn)
{
	std::vector<std::string> result;
	std::string tempStr;

	for (char ch : str)
	{
		if (ch == charToSplitOn)
		{
			if (!tempStr.empty())
			{
				result.push_back(tempStr);
				tempStr.clear();
			}
		}
		else
		{
			tempStr += ch;
		}
	}

	if (!tempStr.empty())
	{
		result.push_back(tempStr);
	}

	return result;
}