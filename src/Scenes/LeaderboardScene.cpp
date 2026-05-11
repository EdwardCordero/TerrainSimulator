#include "Scenes\LeaderboardScene.h"
#include "Scenes\SceneManager.h"
#include "Player\Player.h"
#include "GameLogic\HelperFunctions.h"
#include "globalConstants.h"
#include <SDL.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <utility>
#include <memory>

namespace fs = std::filesystem;

LeaderboardScene::LeaderboardScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName) : Scene(sceneManager, sceneName)
{
	this->mLeadboardIndex = 0;
	this->mLeaderboardFilesPath = ".\\Resources\\SavedData\\Leaderboards\\";
	this->mFontPath = ".\\Resources\\Fonts\\heavy_data.ttf";
}

LeaderboardScene::~LeaderboardScene()
{

}

void LeaderboardScene::initialize()
{
	this->loadLeaderboards();
}

void LeaderboardScene::restart()
{
	std::cout << "Leaderboard Scene restart not implimented yet" << std::endl;
}

void LeaderboardScene::updateRects()
{

}

void LeaderboardScene::updateLeaderboard(std::string game, std::string playerName, int score)
{
	writeToLeaderboard(game, std::format("{},{}", playerName, score));
}

void LeaderboardScene::loadLeaderboards()
{
	// populate mLeaderboards from files
	for (const auto file : std::filesystem::directory_iterator(mLeaderboardFilesPath))
	{
		std::string filePath = file.path().string();
		// Get name of game using file name
		std::string game = getSubstringFromString(file.path().filename().string(), '_');
		// leadboard pair to sort by highest score
		std::vector<std::pair<std::string, int>> leaderboard;

		std::ifstream fileStream(filePath);
		if (fileStream.is_open())
		{
			std::string line;
			while (std::getline(fileStream, line)) {
				std::string playerName = getSubstringFromString(line, ',');
				std::string scoreStr = line.substr(playerName.size() + 1);
				int score = std::stoi(scoreStr);
				leaderboard.emplace_back(playerName, score);
			}
			fileStream.close();
		}
		else
		{
			std::cerr << "Error openning file " << filePath << std::endl;
		}
		// sort the strings by highest score
		this->sortLeaderboard(leaderboard);
		// Once sorted convert vector of pairs into 1 string seperated by \n to load onto single texture;
		std::string leaderboardString = "";
		for (int i = 0; i < leaderboard.size(); i++)
		{
			std::string record = padString(leaderboard[i].first, 5) + "    " +std::to_string(leaderboard[i].second) + "\n";
			leaderboardString += record;
		}

		// push leaderboard along with name of game as a pair to member variable holding all leaderboards.
		mLeaderboards.emplace_back(game, leaderboardString);
	}

	// Once mLeaderboards has been populated, generate textures for each pair of <gameName, leaderboard>
	for (int i = 0; i < mLeaderboards.size(); i++)
	{
		std::string game = mLeaderboards[i].first;
		std::string leaderboard = mLeaderboards[i].second;

		generateLeaderboardTextures(game, leaderboard);
	}
}

void LeaderboardScene::generateLeaderboardTextures(std::string gameName, std::string leaderboard)
{
	//SDL_Color white = { 255, 255, 255 };
	//Texture* leaderboardTitleText = new Texture();
	//leaderboardTitleText->loadFontFromFile(mFontPath, gameName, white, 100);
	//mTitleTextures.emplace_back(leaderboardTitleText);
	////leaderboardTitleText.free();

	//Texture* leaderboardText = new Texture();
	//leaderboardText->loadFontFromFile(mFontPath, leaderboard, white, 60);
	//mLeaderboardTextures.emplace_back(leaderboardText);
	//leaderboardText.free();
}

void LeaderboardScene::sortLeaderboard(std::vector<std::pair<std::string, int>>& leaderboard)
{
	std::sort(leaderboard.begin(), leaderboard.end(), [](const auto& a, const auto& b) {
		return a.second > b.second;
		});
}

std::string LeaderboardScene::getSubstringFromString(std::string str, char breakOnChar)
{
	std::string subString = "";
	// Get name of game
	for (int i = 0; i < str.length(); i++)
	{
		char currentChar = str[i];
		if (currentChar == breakOnChar)
		{
			break;
		}
		subString += currentChar;
	}

	return subString;
}

void LeaderboardScene::handleEvents(SDL_Event& event)
{

}
void LeaderboardScene::update(const Uint8* keyState)
{

}
void LeaderboardScene::render()
{
	/*SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gRenderer);

	int margin = 20;
	int titleWidth = mTitleTextures[mLeadboardIndex]->getWidth();
	int titleHeight = mTitleTextures[mLeadboardIndex]->getHeight();
	mTitleTextures[mLeadboardIndex]->render(getCenterX(SCREEN_WIDTH, titleWidth), margin);

	int leaderboardWidth = mLeaderboardTextures[mLeadboardIndex]->getWidth();
	mLeaderboardTextures[mLeadboardIndex]->render(getCenterX(SCREEN_WIDTH, leaderboardWidth), (2*margin) + titleHeight);

	SDL_RenderPresent(gRenderer);*/
}
void LeaderboardScene::display()
{
	this->mActive = true;
}
void LeaderboardScene::hide()
{
	this->mActive = false;
}
void LeaderboardScene::changeLeaderboard(int newIndex)
{
	this->mLeadboardIndex = newIndex;
}