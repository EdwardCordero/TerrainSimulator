#include "Scenes\SimulatorScene.h"
#include "Graphics\Shapes.h"
#include <SDL.h>
#include <memory>

SimulatorScene::SimulatorScene(std::shared_ptr<SceneManager> sceneManager, std::string sceneName) : Scene(sceneManager, sceneName)
{

}

SimulatorScene::~SimulatorScene()
{

}
void SimulatorScene::handleEvents(SDL_Event& event)
{

}
void SimulatorScene::update(const Uint8* keyState)
{

}
void SimulatorScene::render()
{
    this->graphicsManager->preRender();

	SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);

	std::vector<SDL_Point> points;
    for(auto c : this->nodes)
    {
        this->graphicsManager->renderDrawPoints(c->points.data(), c->points.size());
    }

    SDL_Rect test = {0, this->screenHeight - 150, 100, 100};
    //SDL_RenderFillRect(this->renderer, &test);

    this->graphicsManager->render();
}
void SimulatorScene::display()
{

}
void SimulatorScene::hide()
{

}
void SimulatorScene::initialize()
{
    this->graphicsManager->setWindowFullScreen();
    this->renderer = this->graphicsManager->getRenderer();
    this->screenWidth = this->graphicsManager->getWindowRect().w;
    this->screenHeight = this->graphicsManager->getWindowRect().h;

    int baseRadius = 20;
    int rows = 8;
    int cols = 8;
    int nodeWidth = baseRadius * 2;
    int rowYGap = (this->screenHeight - 1 - nodeWidth) / (rows - 1);
    int rowWidth = (this->screenWidth - 1 - nodeWidth) / (cols - 1);

    for(int row = 0; row < rows; row++)
    {
        // intialize points
        int yStart = (row * rowYGap) + baseRadius;
        //int yStart = (baseRadius + row * (nodeWidth + rowYGap)) * -1;
        int margin = 500;
        for(int col = 0; col <= cols; col++)
        {
            int xStart = (col * rowWidth) + baseRadius;
            std::shared_ptr<Circle> c = std::make_shared<Circle>();
            c->radius = baseRadius;
            c->centerX = xStart;// + (c->radius * 2 * (i + 1)) + (margin * i);
            c->centerY = yStart;//- (c->radius * 2);
            for(int y = c->radius * -1; y <= c->radius; y++)
            {        
                for(int x = c->radius * -1; x <= c->radius; x++)
                {
                    if(x*x + y*y <= c->radius * c->radius)
                    {
                        c->points.push_back({c->centerX + x, c->centerY + y});
                    }
                }
            }

            this->nodes.push_back(c);
        }
    }
}
void SimulatorScene::restart()
{

}
void SimulatorScene::updateRects()
{

}