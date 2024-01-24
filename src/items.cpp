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

//default invader dimensions
constexpr int invader_width       = 20;
constexpr int invader_height      = 20;
//default shell dimensions
constexpr int shell_width         = 5;
constexpr int shell_height        = 5;
//default invader ship dimensions
constexpr int invader_ship_width  = 40;
constexpr int invader_ship_height = 20;
//default player ship dimensions
constexpr int player_ship_width   = 30;
constexpr int player_ship_height  = 20;

Invader::Invader(float x, float y, float speed, bool visible)
{
    position_counter = 0; 
    setPosition(x,y);
    setParameters(speed,visible);
    setTexture("path to the texture");
    setSpriteRectangle(sf::IntRect(0, 0, invader_width, invader_height));
}

void Invader::moveAlongTrajectory(unsigned int framerate)
{
    // Invader trajectory:
    //  10 steps
    // --------------->
    // |              |   
    // |              | 2 steps
    // <--------------|
    const int step_x = 10;
    const int step_y = 10;

    float distance = 0.f;
    if(framerate != 0)
    {
        distance = this->speed/framerate;
    }    
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

Shell::Shell(float x, float y, float speed, ShellType shell_type)
{
    this->shell_type = shell_type;
    this->setPosition(x,y);
    setParameters(speed,true); 
    setTexture("path to the texture");
    setSpriteRectangle(sf::IntRect(0, 0, shell_width, shell_height));
}

ShellType Shell::getShellType()
{
    return shell_type;
}

void Shell::setShellType(const ShellType new_type)
{
    shell_type = new_type;
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
        case ShellType::ENEMY:
            this->sprite.move(0.f,distance);
            break;
        case ShellType::PLAYER:
            this->sprite.move(0.f,distance* (-1.f));
            break;
        default:
            break;
    }
}

InvaderShip::InvaderShip(float x, float y, float speed, bool visible, float range)
{
    //start with move right
    this->direction = ItemDirection::RIGHT;
    this->range   = range;
    this->setPosition(x,y);
    setParameters(speed,visible); 
    this->setPosition(x,y);
    setTexture("path to the texture");
    setSpriteRectangle(sf::IntRect(0, 0, invader_ship_width, invader_ship_height));
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
    setTexture("path to the texture");
    setSpriteRectangle(sf::IntRect(0, 0, player_ship_width, player_ship_height));
}

void PlayerShip::setDirection(ItemDirection direction)
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
