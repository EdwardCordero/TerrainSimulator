#include "Armory\Weapon.h"
#include "GlobalConstants.h"
#include "Player\Player.h"
#include "Factories\EnemyFactory.h"
#include "Armory\WeaponTypeEnum.h"
#include "GameLogic\HelperFunctions.h"
#include <iostream>
#include <vector>
#include <memory>

Weapon::Weapon(std::shared_ptr<Player> player)
{
	this->ammoTexture = nullptr;
	this->ammoTextureRect = { 0, 0, 0, 0 };
	this->dmg = 0;
	this->projectileVelocity = 0;
	this->rateOfFire = 0;
	this->weaponCoolDown = 0;
	this->player = player;
}

Weapon::~Weapon()
{
	//this->ammoTexture.free();
}

// TODO: Return object/Bullet to add to collision manager
void Weapon::shoot(float x, float y, float rotateDeg)
{
	if (this->weaponCoolDown <= 0)
	{
		this->weaponCoolDown = rateOfFire;
		SDL_FRect bulletRect = { x + (ammoTextureRect.w / 4), y + (ammoTextureRect.h / 5), ammoTextureRect.w / 5, ammoTextureRect.h / 2 };
		std::shared_ptr<Bullet> newBullet = std::make_shared<Bullet>(bulletRect, rotateDeg);
		bullets.emplace_back(newBullet);
		this->collisionManager->addRigidBody(newBullet);
	}
}

void Weapon::update(float deltaTime, SDL_Rect screenSize)
{
	this->updateBullets(deltaTime, screenSize);
	if (this->weaponCoolDown > 0)
	{
		this->weaponCoolDown -= deltaTime;
	}
}

void Weapon::updateBullets(float deltaTime, SDL_Rect screenSize)
{
	for(int i = 0; i < bullets.size(); i++)
	{
		std::shared_ptr<Bullet> bullet = bullets[i];
		if (bullet->isActive() && isRectWithinScreenBorders(screenSize, bullet->getRigidBodyRect()))
		{
			int newX = bullet->getXPos() + (projectileVelocity * cos(bullet->getRotateDeg() * M_PI / 180) * deltaTime);
			bullet->setXPos(newX);

			int newY = bullet->getYPos() + (projectileVelocity * sin(bullet->getRotateDeg() * M_PI / 180) * deltaTime);
			bullet->setYPos(newY);
		}
		else
		{
			bullet->setActive(false);
			bullets.erase(bullets.begin() + i);
			i--;
			continue;
		}
	}
}

void Weapon::setWeaponType(WeaponTypeEnum weaponType, std::shared_ptr<Texture> ammoTexture)
{
	switch (weaponType)
	{
		case MINI_GUN:
			this->ammoTexture = ammoTexture;
			ammoTextureRect = { 0, 0, 90, 25 };
			dmg = 10;
			projectileVelocity = 1000;
			rateOfFire = 0.2;
			break;
		default:
			std::cout << "Invaild Weapon Type" << std::endl;
			break;
	}
}

void Weapon::setPlayer(std::shared_ptr<Player> player)
{
	this->player = player;
}

void Weapon::setCollisionManager(std::shared_ptr<CollisionManager> collisionManager)
{
	this->collisionManager = collisionManager;
}

std::vector<std::shared_ptr<Bullet>> Weapon::getBullets()
{
	return this->bullets;
}


std::shared_ptr<Texture> Weapon::getAmmoTexture()
{
	return this->ammoTexture;
}