/**
 * @file canvas.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */
#include "canvas.hpp"
#include "items.hpp"

//window title and game name
static const sf::String title = "Space Invaders";
//default size for coordinates for main view
static const float default_x_size = 1000.f;
static const float default_y_size = 1000.f;

//invaders constants for grid and speed
static const float default_invader_speed = 100.f;
static const int   invaders_in_row       = 16;
static const int   rows_with_invaders    = 4;
static const float grid_row_step         = 50.f;

Canvas::Canvas(const unsigned int width, unsigned int height, const unsigned int framerate)
{
    this->framerate = framerate;
    this->p_window  = new sf::RenderWindow(sf::VideoMode(width, height), title);
    this->p_graphic_thread  = new std::thread(&Canvas::graphicThreadHandler,this);
    //disable window control (it will be controlled in p_thread)
    this->p_window->setActive(false); 
}

Canvas::~Canvas()
{
    delete p_graphic_thread;
    delete p_window;
}

void Canvas::windowEventHandler()
{
    using namespace std::chrono_literals;
    
    // we will exit this function only if window was requested to be closed
    while (this->p_window->isOpen())
    {
        sf::Event event;
        bool stat = true;
        while(stat == true)
        {
            stat = this->p_window->pollEvent(event);
            if(stat == true)
            {
                this->eventExecutor(event);
            }
        }
        std::this_thread::sleep_for(50ms);
    }
    this->p_graphic_thread->join();
}

void Canvas::spawnEnemies()
{
    //enemies
    Invader invader(0.f,0.f,default_invader_speed,true);
    float init_x   = 50.f;
    float init_y   = 50.f;
    float offset_y = 0.f;

    for(auto j = 0; j < rows_with_invaders; j++)
    {
        float offset_x = 0.f;
        for(auto i = 0; i < invaders_in_row; i++)
        {
            invader.setPosition((init_x + offset_x),(init_y + offset_y));
            this->enemies.push_back(invader);
            offset_x += grid_row_step;
        }
        offset_y += grid_row_step;
    }
}

void Canvas::updateCanvas()
{
    //update enemies
    for(auto i = 0; i < this->enemies.size(); i++)
    {
        this->p_window->draw(this->enemies[i].getSprite());
    }

}

void Canvas::updateItemsPosition()
{
    //update enemies
    for(auto i = 0; i < this->enemies.size(); i++)
    {
        if(this->enemies[i].isVisible() == true)
        {
            this->enemies[i].moveAlongTrajectory(this->framerate);
        }
    }
}

void Canvas::updateFramerate(const unsigned int framerate)
{
    if(framerate <= MAX_FRAMERATE)
    {
        this->framerate = framerate;
    }
    else
    {
        this->framerate = MAX_FRAMERATE;
    }
}

void Canvas::graphicThreadHandler()
{
    //obtain control on window
    if(!this->p_window->setActive(true)){return;}
    else
    {
        // all GUI drawings are here
        while (this->p_window->isOpen())
        {
            sf::View view(sf::FloatRect(0.f, 0.f, default_x_size, default_y_size));
            this->p_window->setView(view);
            this->p_window->clear(sf::Color::Black); 
            this->updateCanvas();
            this->p_window->display();
            this->updateItemsPosition();
            unsigned int delay_per_frame = 1000/this->framerate;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_per_frame));
        }
    }
}

void Canvas::gameEventHandler()
{
}

void Canvas::eventExecutor(const sf::Event &event)
{
    switch (event.type)
    {

    case sf::Event::Closed:
        this->p_window->close();
        break;
    
    default:
        break;
    }
}
