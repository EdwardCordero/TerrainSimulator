#pragma once
#include "Enums/LimitEnum.h"
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <SDL.h>
#include <nlohmann\json.hpp>

int getCenterX(int parentWidth, int width);
int getCenterY(int parentHeight, int height);
int randNumBetween(int min, int max);
float randFloatBetween(float min, float max);
bool checkCollision(SDL_FRect a, SDL_FRect b);
float shiftPointsWithinLimit(std::array<float, 2>& currentPos, std::array<float, 2>& destination, std::array<float, 2>& startPos, Limit limitHit, float width, float height, int screenWidth, int screenHeight);
Limit isOutOfBound(float x, float y, float topOffset, float rightOffset, float bottomOffset, float leftOffset, int screenWidth, int screenHeight);
Limit whichSideCollided(SDL_FRect a, SDL_FRect b);
float lerp(float start, float end, float t);
std::array<std::array<float, 2>, 4> getCornersOfRect(SDL_FRect rect);
std::array<float, 2> MoveRectBetweenPoints(float deltaTime, SDL_FRect rect, std::array<float, 2>& startPos, std::array<float, 2>& destination, float& movementProgress, float speed);
std::array<float, 2> MoveRectAroundTarget(float deltaTime, SDL_FRect rect, std::array<std::array<float, 2>, 4>& targetCorners, std::array<float, 2>& startPos, std::array<float, 2>& destination, float& movementProgress, float speed, bool clockwise);
int getCornerNumber(std::array<std::array<float, 2>, 4> targetCorners, std::array<float, 2> destination);
float CosStrafe(float delatTime, float& angle, float anchor, float speed);
float SinStrafe(float delatTime, float& angle, float anchor, float speed);
float calculateDistance(std::array<float, 2> start, std::array<float, 2> end);
float calculateDistance(float x1, float x2, float y1, float y2);
float getClosestDistance(SDL_FRect objectRect, SDL_FRect targetRect);
float randNumInFloatVector(std::vector<float> numbers);
int randNumInVector(std::vector<int> numbers);
std::string FormatUint32(Uint32 ticks);
std::vector<std::string> getFileContents(std::string pathToFile);
void writeToLeaderboard(std::string game, std::string str);
std::string padString(std::string& str, int targetSize);
bool isRectWithinScreenBorders(SDL_Rect screenSize, SDL_FRect rect);
std::string jsonToString(const nlohmann::json& val);
std::vector<std::string> splitString(const std::string& str, char charToSplitOn);