/**
 * @file canvas.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#include <iostream>
#include <cstdlib>

#include "canvas.hpp"
#include "items.hpp"

//window title and game name
static const sf::String title = "Space Invaders";

//default size for coordinates for main view
static const float default_x_size = 1000.f;
static const float default_y_size = 1000.f;

//invaders constants for grid and speed
static const int   invaders_in_row       = 18;
static const int   rows_with_invaders    = 4;
//canvas cross in 10s
static const float default_invader_speed = (default_x_size + default_y_size)/(2.f * 10.f) ;
//canvas cross in 2s
static const float default_shell_speed = (default_x_size + default_y_size)/(2.f * 2.f) ;
//max 20 items per one row
static const float grid_row_step = default_x_size/20.f;

Canvas::Canvas(const unsigned int width, unsigned int height, const unsigned int framerate)
{
    this->framerate = framerate;
    this->grid_x = default_x_size;
    this->grid_y = default_y_size;
    this->p_window  = new sf::RenderWindow(sf::VideoMode(width, height), title);
    this->p_graphic_thread  = new std::thread(&Canvas::graphicThreadHandler,this);
    this->p_game_event_thread = new std::thread(&Canvas::gameEventHandler,this);
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
    const float init_x = 50.f;
    const float init_y = 50.f;
    
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
        if(this->enemies[i].isVisible() == true)
        {
            this->p_window->draw(this->enemies[i].getSprite());
        }
    }
    //update bullets
    for(auto i = 0; i < this->bullets.size(); i++)
    {
        if(this->bullets[i].isVisible() == true)
        {
            this->p_window->draw(this->bullets[i].getSprite());
        }
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
    
    //update bullets
    for(auto i = 0; i < this->bullets.size(); i++)
    {
        if(this->bullets[i].isVisible() == true)
        {
            this->bullets[i].moveAlongTrajectory(this->framerate);
        }
    }
}

void Canvas::controlItemsPosition()
{
    //bullets control
    for(auto i = 0; i < this->bullets.size(); i++)
    {
        auto position = this->bullets[i].getRectangle().getPosition();
        if((position.x > this->grid_x) || (position.x < 0.f) ||
           (position.y > this->grid_y) || (position.y < 0.f)
          )
        {
            this->bullets[i].setInvisible();
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
            this->controlItemsPosition();
            this->updateItemsPosition();
            unsigned int delay_per_frame = 1000/this->framerate;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_per_frame));
        }
    }
}

void Canvas::gameEventHandler()
{
    using namespace std::chrono_literals;
    // we will exit this function only if window was requested to be closed
    while (this->p_window->isOpen())
    {
        std::this_thread::sleep_for(1000ms);
        auto index = rand() % this->enemies.size();
        std::cout<<"enemy with id : "<<index<<std::endl;
        this->invaderShot(this->enemies[index]);
    }
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

void Canvas::invaderShot(Invader &invader)
{
    auto position = invader.getRectangle().getPosition();
    float x = position.x + (float)(INVADER_WIDTH/2);
    float y = position.y + (float)(INVADER_HEIGHT/2);
    Shell shell(x,y,default_shell_speed,ShellTypes::ENEMY_SHELL);    
    this->bullets.push_back(shell);
}
