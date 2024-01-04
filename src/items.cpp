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

Invader::Invader(const float x, const float y)
{
    this->def_x = x;
    this->def_y = y;

    this->sprite.setPosition(x,y);
    this->sprite.setTexture(this->texture);
    
    sprite.setTextureRect(sf::IntRect(0, 0, 20, 20));
    this->sprite.setColor(sf::Color::White);
    this->visible = true;
    this->speed   = 100.f; 
}

void Invader::moveAlongTrajektory(unsigned int framerate)
{
    static unsigned int counter = 0;
    static const int step_x = 50;
    static const int step_y = 50;

    float distance = this->speed/framerate;

    // Invader trajektory:
    //  10 steps
    // --------------->
    // |              |   
    // |              | 2 steps
    // <--------------|
    
    if(counter < step_x){this->sprite.move(distance,0.f);}
    else if (counter < (step_x + step_y)){this->sprite.move(0.f,distance);}
    else if (counter < (2*step_x + step_y)){this->sprite.move(distance * (-1.f),0.f);}
    else if (counter < (2*step_x + 2*step_y)){this->sprite.move(0.f,distance* (-1.f));}
    counter++;
    if(counter == (2*step_x + 2*step_y))
    {
        this->sprite.setPosition(this->def_x,this->def_y);
        counter = 0;
    }
}   
