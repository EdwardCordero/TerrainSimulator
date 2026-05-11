#pragma once
#include "Enemies\Enemy.h"
#include "Environments\Debris.h"
#include "Physics\CollisionManager.h"
#include "Settings\SettingsManager.h"
#include "Graphics\GraphicsManager.h"
#include <iostream>
#include <vector>
#include <memory>

class Player;
class EnemyFactory {
	public:
		EnemyFactory();
		EnemyFactory(std::shared_ptr<CollisionManager> collisionManager, std::shared_ptr<SettingsManager> settingsManager, std::shared_ptr<GraphicsManager> graphicsManager);
		~EnemyFactory();
		void setPlayer(std::shared_ptr<Player> player);
		void update();
		void refresh();
		void updateScreenSize();
		void renderEnemies();
		void createAsteroid();
		void createPawn(int x, int y);
		void createDebris(int x, int y, int debrisCount);
		void updatePawns();
		void updateDebris();
		bool checkCollisionOfPawns(SDL_FRect bullet, int dmg);
		void setSpawnBoundaries();
		int getEnemyPoolSize();
		//void spawnAtRandomAvailableLocation(std::shared_ptr<Enemy> enemy);
		std::shared_ptr<Enemy> getEnemy(int id);
	private:
		//std::map<int, std::vector<float>> availableSpawnLocations;
		std::shared_ptr<CollisionManager> collisionManager;
		std::shared_ptr<GraphicsManager> graphicsManager;
		std::vector<std::shared_ptr<Enemy>> enemyPool;
		std::vector<std::shared_ptr<Debris>> debris;
		std::shared_ptr<Player> player;
		std::shared_ptr<SettingsManager> settingsManager;
		
		SDL_Rect currentScreenRect;
};