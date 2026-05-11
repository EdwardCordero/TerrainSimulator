#include "Factories\EnemyFactory.h"
#include "GameLogic\HelperFunctions.h"
#include "Enemies\Enemy.h"
#include "Enemies\Pawn.h"
#include "Enemies\Asteroid.h"
#include "Player\Player.h"
#include "Settings\SettingsManager.h"
#include <memory>
#include <format>

EnemyFactory::EnemyFactory()
{
    this->player = NULL;	
    this->currentScreenRect = { 0,0,0,0 };
    this->collisionManager = nullptr;
    this->settingsManager = nullptr;
    this->graphicsManager = nullptr;
}

EnemyFactory::EnemyFactory(std::shared_ptr<CollisionManager> collisionManager, std::shared_ptr<SettingsManager> settingsManager, std::shared_ptr<GraphicsManager> graphicsManager)
{
    this->collisionManager = collisionManager;
    this->settingsManager = settingsManager;
    this->graphicsManager = graphicsManager;
    this->currentScreenRect = { 0,0,0,0 };
    this->setSpawnBoundaries();
    this->graphicsManager->cacheTexture("Asteroid");
    this->graphicsManager->cacheTexture("DefenderEnemy");
}

EnemyFactory::~EnemyFactory()
{
    
}

void EnemyFactory::setPlayer(std::shared_ptr<Player> player)
{
    this->player = player;
}

void EnemyFactory::update()
{
    this->updatePawns();
    this->updateDebris();
}


void EnemyFactory::refresh()
{
    this->setSpawnBoundaries();

    /*for (auto& enemy : this->enemyPool)
    {
        enemy->spawn(this->currentScreenRect);
    }*/
    this->enemyPool.clear();
    this->debris.clear();
}

void EnemyFactory::renderEnemies()
{
    for (auto& enemy : enemyPool)
    {
        if (enemy->isActive())
        {
            //enemy->render();
            SDL_FRect enemyRigidBody = enemy->getRigidBodyRect();
            this->graphicsManager->renderTextureEx(enemy->getTexture(), NULL, &enemyRigidBody, enemy->getRotateDeg(), SDL_FLIP_NONE);
        }
    }

    for (auto& singleDebris : debris)
    {
        singleDebris->render();
        this->graphicsManager->renderDrawPoint(singleDebris->getX(), singleDebris->getY(), singleDebris->getDebriColor());
    }

    SDL_Rect topBorder = { 0,0,this->currentScreenRect.w,10 };
    SDL_Rect rightBorder = { this->currentScreenRect.w - 10,0,10,this->currentScreenRect.h };
    SDL_Rect leftBorder = {0,0,10,this->currentScreenRect.h };
    SDL_Rect bottomBorder = { 0,this->currentScreenRect.h - 10,this->currentScreenRect.w,10 };
    SDL_Color borderColor = { 255, 255, 255, 255 };
    this->graphicsManager->renderFillRect(&topBorder, borderColor);
    this->graphicsManager->renderFillRect(&rightBorder, borderColor);
    this->graphicsManager->renderFillRect(&leftBorder, borderColor);
    this->graphicsManager->renderFillRect(&bottomBorder, borderColor);
}

void EnemyFactory::createAsteroid()
{
    auto newAsteroid = std::make_shared<Asteroid>();
    newAsteroid->setTexture(this->graphicsManager->getCachedTexture("Asteroid"));
    enemyPool.emplace_back(newAsteroid);
}

void EnemyFactory::createPawn(int x, int y)
{
    if (enemyPool.size() < 1)
    {
        auto newPawn = std::make_shared<Pawn>();
        newPawn->setTexture(this->graphicsManager->getCachedTexture("DefenderEnemy"));
        enemyPool.emplace_back(newPawn);
    }
}

void EnemyFactory::createDebris(int x, int y, int debrisCount)
{
    for (int i = 0; i < debrisCount; i++)
    {
        float angle = i * ((2 * M_PI) / debrisCount);
        std::shared_ptr<Debris> newDebris = std::make_shared<Debris>(x, y, angle);
        debris.emplace_back(newDebris);
    }
}

void EnemyFactory::updatePawns() 
{
    //std::vector<int> sizes = { 1, 2, 4, 10 };
    std::vector<int> sizes = { 10 };
    for (auto& enemy : enemyPool)
    {   
        if (enemy->isActive())
        {
            enemy->update(player->getRigidBodyRect(), this->graphicsManager->getDeltaTime());
        }
        else
        {
            if (enemy->getHealth() <= 0)
            {
                std::cout << std::format("Enemy at ({}, {}) is dead", enemy->getXPos(), enemy->getYPos()) << std::endl;
                int newPlayerScore = enemy->getPoints() + this->player->getScore();
                player->setScore(newPlayerScore);
                SDL_FRect enemyRect = enemy->getRigidBodyRect();
                //TODO: change 5 to match sizes of asteroid
                createDebris(enemyRect.x + (enemyRect.w / 2), enemyRect.y + (enemyRect.h / 2), 5);
            }
            enemy->spawn(this->currentScreenRect);
            //int size = randNumInVector(sizes);
        }
    }
}

void EnemyFactory::updateDebris()
{
    for(int i = 0; i < debris.size() && (debris.size() != 0); i++)
    {
        auto& singleDebris = debris[i];
        if (singleDebris->isDead())
        {
            debris.erase(debris.begin() + i);
        }
        else
        {
            singleDebris->update(this->graphicsManager->getDeltaTime());
        }
    }
}

/// <summary>
/// Returns true as soon as it gets a hit because the bullet should only hit 1 thing... for now.
/// </summary>
/// <param name="bulletRect"></param>
/// <param name="dmg"></param>
/// <returns></returns>
bool EnemyFactory::checkCollisionOfPawns(SDL_FRect bulletRect, int dmg)
{
    for (auto& enemy : enemyPool)
    {
        if (!enemy->isActive())
        {
            continue;
        }
        if (checkCollision(bulletRect, enemy->getRigidBodyRect()))
        {
            enemy->takeDmg(dmg);
            return true;
        }
    }

    return false;
}

void EnemyFactory::updateScreenSize()
{
    this->setSpawnBoundaries();

    for (auto& enemy : enemyPool)
    {
        SDL_FRect enemyRigidBody = enemy->getRigidBodyRect();
        this->graphicsManager->scaleRect(enemyRigidBody);
        enemy->setRigidBodyRect(enemyRigidBody);

        SDL_FRect enemyStartPos = { enemy->getStartPos()[0],enemy->getStartPos()[1],0,0 };
        this->graphicsManager->scaleRect(enemyStartPos);
        enemy->setStartPos(enemyStartPos.x, enemyStartPos.y);

        SDL_FRect enemyDestination = { enemy->getDestination()[0], enemy->getDestination()[1], 0,0 };
        this->graphicsManager->scaleRect(enemyDestination);
        enemy->setDestination(enemyDestination.x, enemyDestination.y);
    }

    for (auto& singleDebris : debris)
    {
        SDL_Rect debrisRect;
        debrisRect.x = singleDebris->getX();
        debrisRect.y = singleDebris->getY();
        this->graphicsManager->scaleRect(debrisRect);
        singleDebris->updatePosition(debrisRect.x, debrisRect.y);
    }
}

void EnemyFactory::setSpawnBoundaries()
{
    this->currentScreenRect = this->graphicsManager->getScreenRect();
}

int EnemyFactory::getEnemyPoolSize()
{
    return this->enemyPool.size();
}

std::shared_ptr<Enemy> EnemyFactory::getEnemy(int id)
{
    return this->enemyPool[id];
}