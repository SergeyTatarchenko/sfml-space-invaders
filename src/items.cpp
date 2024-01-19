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

#define INVADER_WIDTH        (int)(20)
#define INVADER_HEIGHT       (int)(20)

#define INVADER_SHIP_WIDTH   (int)(40)
#define INVADER__SHIP_HEIGHT (int)(20)

#define PLAYER_SHIP_WIDTH   (int)(30)
#define PLAYER__SHIP_HEIGHT (int)(20)

#define SHELL_WIDTH          (int)(5)
#define SHELL_HEIGHT         (int)(5)

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

InvaderShip::InvaderShip(float x, float y, float speed, bool visible, float range)
{
    this->visible = visible;
    this->speed   = speed;
    this->range   = range;
    //start with move right
    this->direction = ItemDirection::RIGHT;
    this->setPosition(x,y);

    this->sprite.setTexture(this->texture);
    sprite.setTextureRect(sf::IntRect(0, 0, INVADER_SHIP_WIDTH, INVADER__SHIP_HEIGHT));
    this->sprite.setColor(sf::Color::White);
}

void InvaderShip::moveAlongTrajectory(unsigned int framerate)
{
    // Invader ship trajectory:
    //  n steps, depends on this->range
    // --------------->
    // <--------------|
    float distance = 0.f;
    auto rectangle = this->getRectangle(); 
    auto position  = rectangle.getPosition();
    
    if(framerate != 0){distance = this->speed/framerate;}
    else{return;}
    
    // update direction 
    if((position.x + distance + rectangle.width) > (this->def_x + this->range))
    {
        //time to turn direction to left
        this->direction = ItemDirection::LEFT;
    }
    else if((position.x - distance) < this->def_x)
    {
        //time to turn direction to right again
        this->direction = ItemDirection::RIGHT;
    }

    switch(this->direction)
    {
        case ItemDirection::RIGHT:
            this->sprite.move(distance,0.f);
            break;
        case ItemDirection::LEFT:
            this->sprite.move(distance * (-1.f),0.f);
            break;
        default:
            break;
    }    
}

void InvaderShip::setDirection(const ItemDirection direction)
{
    this->direction = direction;
}

PlayerShip::PlayerShip(float x, float y, float speed, float limit_x, float limit_y)
{
    this->visible = true;
    this->speed   = speed;
    this->limit_x = limit_x;
    this->limit_y = limit_y;
    this->invincible   = true;
    this->shot_request = false;
    this->direction = ItemDirection::NONE;
    
    this->setPosition(x,y);
    this->sprite.setTexture(this->texture);
    sprite.setTextureRect(sf::IntRect(0, 0, PLAYER_SHIP_WIDTH, PLAYER__SHIP_HEIGHT));
    this->sprite.setColor(sf::Color::White);
}

void PlayerShip::setDicection(ItemDirection direction)
{
    this->direction = direction;
}

void PlayerShip::moveAlongTrajectory(unsigned int framerate)
{
    float distance = 0.f;
    auto rectangle = this->getRectangle(); 
    auto position  = rectangle.getPosition();
    
    if(framerate != 0){distance = this->speed/framerate;}
    else{return;}

    switch(this->direction)
    {
        case ItemDirection::RIGHT:
            if((position.x + distance + rectangle.width) < (this->def_x + this->limit_x))
            {
                this->sprite.move(distance,0.f);
            }
            break;

        case ItemDirection::LEFT:
            if((position.x - distance) > this->def_x)
            {
                this->sprite.move(distance * (-1.f),0.f);
            }
            break;

        default:
            break;
    }
}

void PlayerShip::setShotRequest(bool state)
{
    this->shot_request = state;
}

bool PlayerShip::getShotRequest()
{
    return this->shot_request;
}
