/**
 * @file items.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */
#include "items.hpp"

Invader::Invader(float x, float y, float speed, bool visible)
{
    this->setPosition(x,y);
    this->sprite.setTexture(this->texture);
    
    sprite.setTextureRect(sf::IntRect(0, 0, 20, 20));
    this->sprite.setColor(sf::Color::White);
    
    this->visible          = visible;
    this->speed            = speed;
    this->position_counter = 0; 
}

void Invader::moveAlongTrajectory(unsigned int framerate)
{
    static const int step_x = 10;
    static const int step_y = 10;
    
    float distance = 0.f;
    if(framerate != 0)
    {
        distance = this->speed/framerate;
    }
    // Invader trajectory:
    //  10 steps
    // --------------->
    // |              |   
    // |              | 2 steps
    // <--------------|
    
    if(this->position_counter < step_x){this->sprite.move(distance,0.f);}
    else if (this->position_counter < (step_x + step_y)){this->sprite.move(0.f,distance);}
    else if (this->position_counter < (2*step_x + step_y)){this->sprite.move(distance * (-1.f),0.f);}
    else if (this->position_counter < (2*step_x + 2*step_y)){this->sprite.move(0.f,distance* (-1.f));}
    this->position_counter++;
    if(this->position_counter == (2*step_x + 2*step_y))
    {
        this->sprite.setPosition(this->def_x,this->def_y);
        this->position_counter = 0;
    }
}

Shell::Shell(float x, float y, float speed, ShellTypes shell_type)
{

}

void Shell::moveAlongTrajectory(unsigned int framerate)
{

}
