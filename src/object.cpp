/**
 * @file object.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */
#include "object.hpp"

void Object::setPosition(const float x, const float y)
{
    this->def_x = x;
    this->def_y = y;
    this->sprite.setPosition(x,y);
}

void Object::setVisible()
{
    this->visible = true;
}

void Object::setInvisible()
{
    this->visible = false;
}

bool Object::isVisible()
{
    return this->visible;
}

sf::FloatRect Object::getRectangle()
{
    return this->sprite.getGlobalBounds();
}

const sf::Sprite &Object::getSprite()
{
    return this->sprite;
}
