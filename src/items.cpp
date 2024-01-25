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

constexpr float invader_trajectory_length = 160.f;

Invader::Invader(sf::Vector2f position, float speed, bool visible)
{
    setPosition(position);
    setSpeed(speed);
    if(visible == true){setVisible();}
    else{setInvisible();}

    setTexture("path to the texture");
    setSpriteRectangle(sf::IntRect(0, 0, invader_width, invader_height));
}


void Invader::updatePosition(std::chrono::milliseconds &time)
{
    if(isVisible() == true)
    {

    }
}

Shell::Shell(sf::Vector2f position, float speed, ShellType shell_type)
{
    this->shell_type = shell_type;
    setPosition(position);
    setSpeed(speed);
    setVisible();

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

InvaderShip::InvaderShip(float x, float y, float speed, bool visible, float range)
{
    //start with move right
    this->direction = ItemDirection::RIGHT;
    this->range   = range;
    setTexture("path to the texture");
    setSpriteRectangle(sf::IntRect(0, 0, invader_ship_width, invader_ship_height));
}



void InvaderShip::setDirection(const ItemDirection direction)
{
    this->direction = direction;
}

PlayerShip::PlayerShip(float x, float y, float speed, float limit_x, float limit_y)
{

    this->limit_x = limit_x;
    this->limit_y = limit_y;
    this->invincible   = true;
    this->shot_request = false;
    this->direction = ItemDirection::NONE;
    
    setTexture("path to the texture");
    setSpriteRectangle(sf::IntRect(0, 0, player_ship_width, player_ship_height));
}

void PlayerShip::setDirection(ItemDirection direction)
{
    this->direction = direction;
}

void PlayerShip::setShotRequest(bool state)
{
    this->shot_request = state;
}

bool PlayerShip::getShotRequest()
{
    return this->shot_request;
}
