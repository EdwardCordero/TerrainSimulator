#include "Armory\Bullet.h"
#include "Physics\RigidBody.h"
#include "GlobalConstants.h"
#include <SDL.h>
#include <format>
#include <memory>

Bullet::Bullet(SDL_FRect bulletRect, float degree) : RigidBody()
{
	/* Rigid Body stuff */
	this->setRigidBodyRect(bulletRect);
	this->setRotateDeg(degree);
	this->setBodyName("Bullet");
	this->setBodyType(BulletBody);
	this->setActive(true);
	this->setCollidable(true);
	/* Bullet stats */
	this->velocity = 200;
	this->dmg = 10;
}

Bullet::~Bullet()
{
	//this->bulletTexture.free();
}

void Bullet::onCollision(std::shared_ptr<RigidBody> bodyCollidingInto)
{
	std::cout << std::format("Bulet hit {}", bodyCollidingInto->getBodyName()) << std::endl;
	if (bodyCollidingInto->getBodyType() == PlayerBody)
	{
		return;
	}
	if (bodyCollidingInto->getBodyType() == EnemyBody)
	{
		std::shared_ptr<Enemy> enemyCollidingInto = std::dynamic_pointer_cast<Enemy>(bodyCollidingInto);
		if (enemyCollidingInto->isActive())
		{
			enemyCollidingInto->takeDmg(this->dmg);
		}
		else
		{
			std::cout << "Bulet hit an inactive astroid" << std::endl;
			enemyCollidingInto->setColor({ 255, 0, 0, 0 });
		}
	}
	this->setActive(false);
}

float Bullet::getVelocity()
{
	return this->velocity;
}
float Bullet::getDmg()
{
	return dmg;
}

void Bullet::setVelocity(float newVelocity)
{
	this->velocity = newVelocity;
}