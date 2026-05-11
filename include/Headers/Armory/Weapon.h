#pragma once
#include "Graphics\Texture.h"
#include "Armory\Bullet.h"
#include "Armory\WeaponTypeEnum.h"
#include "Physics\CollisionManager.h"
#include <iostream>
#include <vector>
#include <memory>

/* 
* Weapon holds an ammo type which dictates what
* the member values are set to, and also the visual of the bullet/projectile
* the weapon shoots. 
*/
class Player;
class EnemyFactory;
class Weapon
{
	public:
		/* Functions */
		Weapon(std::shared_ptr<Player> player = nullptr);
		~Weapon();

		void update(float deltaTime, SDL_Rect screenSize);
		void updateBullets(float deltaTime, SDL_Rect screenSize);
		void shoot(float x, float y, float rotateDeg);
		void setWeaponType(WeaponTypeEnum weaponType, std::shared_ptr<Texture> ammoTexture);
		void setPlayer(std::shared_ptr<Player> player);
		void setCollisionManager(std::shared_ptr<CollisionManager> collisionManager);
		std::vector<std::shared_ptr<Bullet>> getBullets();
		std::shared_ptr<Texture> getAmmoTexture();

		/* Variables */
		SDL_Rect ammoTextureRect;
		float dmg;
		float projectileVelocity;
		float rateOfFire;
		float weaponCoolDown;
	private:
		std::shared_ptr<Texture> ammoTexture;
		std::vector<std::shared_ptr<Bullet>> bullets;
		std::shared_ptr<Player> player;
		std::shared_ptr<CollisionManager> collisionManager;
};