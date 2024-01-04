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

const bool Object::getObjectStatus()
{
    return this->visible;
}

const sf::FloatRect &Object::getRectangle()
{
    this->outline = this->sprite.getGlobalBounds();
    return this->outline;
}
