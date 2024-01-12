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

//game event tick
static const int game_event_tick_ms = 50;

//max 20 items per one row
static const float grid_row_step = default_x_size/20.f;

//invaders constants for grid and speed
static const int   invaders_in_row       = 18;
static const int   rows_with_invaders    = 2;

//speed setup
static const float default_invader_speed = (default_x_size + default_y_size)/(2.f * 10.f);
static const float default_ship_speed    = (default_x_size + default_y_size)/(2.f * 4.f);
static const float default_shell_speed   = (default_x_size + default_y_size)/(2.f * 2.f) ;

Canvas::Canvas(const unsigned int width, unsigned int height, const unsigned int framerate)
{
    this->framerate = framerate;
    this->grid_x = default_x_size;
    this->grid_y = default_y_size;
    this->p_window  = new sf::RenderWindow(sf::VideoMode(width, height), title);
    if(this->p_window->isOpen() == true)
    {
        this->game_in_progress.store(true,std::memory_order_relaxed);
        this->p_graphic_thread    = new std::thread(&Canvas::graphicThreadHandler,this);
        //disable window control (it will be controlled in p_graphic_thread)
        this->p_window->setActive(false); 
    }
}

Canvas::~Canvas()
{
    delete this->p_window;
    delete this->p_graphic_thread;
    delete this->p_game_event_thread;
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
        std::this_thread::sleep_for(std::chrono::milliseconds(game_event_tick_ms));
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
    //update enemy ships
    for(auto i = 0; i < this->enemyShips.size(); i++)
    {
        if(this->enemyShips[i].isVisible() == true)
        {
            this->p_window->draw(this->enemyShips[i].getSprite());
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
    //update enemy ships
    for(auto i = 0; i < this->enemyShips.size(); i++)
    {
        if(this->enemyShips[i].isVisible() == true)
        {
            this->enemyShips[i].moveAlongTrajectory(this->framerate);
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
    if(framerate <= MAX_FRAMERATE){this->framerate = framerate;}
    else{this->framerate = MAX_FRAMERATE;}
}

void Canvas::graphicThreadHandler()
{
    //obtain control on window
    if(this->p_window->setActive(true) == true)
    {
        this->p_game_event_thread = new std::thread(&Canvas::gameEventGenerator,this);
        // all graphic drawings are here
        while (this->game_in_progress.load(std::memory_order_relaxed) == true)
        {
            sf::View view(sf::FloatRect(0.f, 0.f, default_x_size, default_y_size));
            this->p_window->setView(view);
            this->p_window->clear(sf::Color::Black); 
            while(this->game_context_control.try_lock() == false){}
            this->updateCanvas();
            this->p_window->display();
            this->controlItemsPosition();
            this->updateItemsPosition();
            this->game_context_control.unlock();
            unsigned int delay_per_frame = 1000/this->framerate;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay_per_frame));
        }
        this->p_window->setActive(false);
    }
}

void Canvas::gameEventGenerator()
{
    static int counter = 0;   
    // we will exit this function only if window was requested to be closed
    while (this->game_in_progress.load(std::memory_order_relaxed) == true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(game_event_tick_ms));
        counter++;
        if((counter % 20) == 0)
        {
            counter = 0;
            while(this->game_context_control.try_lock() == false){}
            auto index = rand() % this->enemies.size();
            std::cout<<"vector size : "<<this->bullets.size()<<std::endl;
            this->invaderShot(this->enemies[index]);
            this->game_context_control.unlock();
        }
    }
}

void Canvas::eventExecutor(const sf::Event &event)
{
    switch (event.type)
    {

    case sf::Event::Closed:
        // game stop, threads stop, window closed
        this->game_in_progress.store(false,std::memory_order_relaxed);
        this->p_graphic_thread->join();
        this->p_game_event_thread->join();
        this->p_window->setActive(true);
        this->p_window->close();
        break;
    
    case sf::Event::KeyPressed:
        // game control
        std::cout<<"key type : "<<event.key.code<<std::endl;
        
        break;
    
    default:
        break;
    }
}

void Canvas::spawnEnemies()
{
    //enemies
    Invader invader(0.f,0.f,default_invader_speed,true);
    const float init_x = 50.f;
    const float init_y = 100.f;
    
    while(this->game_context_control.try_lock() == false){}
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
    // enemy ships
    InvaderShip ship(10.f,10.f,default_ship_speed,true,this->grid_x - 10.f);
    this->enemyShips.push_back(ship);

    this->game_context_control.unlock();
}

void Canvas::invaderShot(Invader &invader)
{
    auto  rectangle = invader.getRectangle();
    //we are going to shut from the middle of invader     
    float x         = rectangle.getPosition().x + rectangle.width/2.0f;
    float y         = rectangle.getPosition().y + rectangle.height/2.0f;
    
    int shell_index = -1;
    //check if we have available shells in array(that was already created and executed)
    for(auto i = 0; i < this->bullets.size(); i++)
    {
        if(this->bullets[i].isVisible() == false)
        {
            shell_index = i;
            break;
        }
    }
    if(shell_index != -1)
    {
        this->bullets[shell_index].setShellType(ShellTypes::ENEMY_SHELL);
        this->bullets[shell_index].setPosition(x,y);
        this->bullets[shell_index].setVisible();
    }
    else
    {
        Shell shell(x,y,default_shell_speed,ShellTypes::ENEMY_SHELL);    
        this->bullets.push_back(shell);
    }
}
