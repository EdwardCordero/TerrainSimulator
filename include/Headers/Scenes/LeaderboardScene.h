#pragma once
#include "Graphics\Texture.h"
#include "Scenes\Scene.h"
#include <SDL.h>
#include <string>
#include <vector>
#include <utility>

class SceneManager;
class LeaderboardScene : public Scene
{
	public:
		LeaderboardScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName);
		~LeaderboardScene() override;
		void handleEvents(SDL_Event& event) override;
		void update(const Uint8* keyState) override;
		void render() override;
		void display() override;
		void hide() override;
		void initialize() override;
		void restart() override;
		void updateRects() override;
		void loadLeaderboards();
		void updateLeaderboard(std::string game, std::string playerName, int score);
		void generateLeaderboardTextures(std::string gameName, std::string leaderboard);
		std::string getSubstringFromString(std::string str, char breakOnChar);
		void sortLeaderboard(std::vector<std::pair<std::string, int>>& leaderboard);
		void changeLeaderboard(int newIndex);
	private:
		int mLeadboardIndex;
		std::string mFontPath;
		std::string mLeaderboardFilesPath;
		// vector that contains a pair "Game Name", "Leaderboard" where "Leaderboard" is a pair of <string, int> aka <player_name, score>
		std::vector<std::pair<std::string, std::string>> mLeaderboards;

		std::vector<Texture*> mLeaderboardTextures;
		std::vector<Texture*> mTitleTextures;

		Texture mRetryText;
		SDL_Rect mRetryRect;
		Texture mQuitText;
		SDL_Rect mQuitRect;
		Texture mShowAllText;
		SDL_Rect mShowAllRect;
};