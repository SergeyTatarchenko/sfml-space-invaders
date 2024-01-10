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
    this->visible          = visible;
    this->speed            = speed;
    this->position_counter = 0; 
    this->setPosition(x,y);

    this->sprite.setTexture(this->texture);
    sprite.setTextureRect(sf::IntRect(0, 0, INVADER_WIDTH, INVADER_HEIGHT));
    this->sprite.setColor(sf::Color::White);
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
    this->shell_type = shell_type;
    this->visible    = true;
    this->speed      = speed; 
    this->setPosition(x,y);
    
    this->sprite.setTexture(this->texture);
    sprite.setTextureRect(sf::IntRect(0, 0, SHELL_WIDTH, SHELL_HEIGHT));
    this->sprite.setColor(sf::Color::White);
}

const ShellTypes Shell::getShellType()
{
    return this->shell_type;
}

void Shell::setSpeed(const float speed)
{
    this->speed = speed;
}

void Shell::setShellType(const ShellTypes new_type)
{
    this->shell_type = new_type;
}

void Shell::moveAlongTrajectory(unsigned int framerate)
{
    float distance = 0.f;
    if(framerate != 0)
    {
        distance = this->speed/framerate;
    }

    // Shell trajectory:
    // /|\             |
    //  |     OR       |   
    //  |             \|/ 

    switch(this->shell_type)
    {
        case ShellTypes::ENEMY_SHELL:
            this->sprite.move(0.f,distance);
            break;
        case ShellTypes::PLAYER_SHELL:
            this->sprite.move(0.f,distance* (-1.f));
            break;
        default:
            break;
    }
}

EnemyShip::EnemyShip(float x, float y, float speed, bool visible)
{

}

void EnemyShip::moveAlongTrajectory(unsigned int framerate)
{
    
}
