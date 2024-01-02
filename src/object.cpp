/**
 * @file object.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */
#include <cmath>
#include "object.hpp"

void Object::setPosition(const double x, const double y)
{
    this->coordinates.x = x;
    this->coordinates.y = y;
}

void Object::getPosition(Coordinates &coordinates)
{
    coordinates.x = this->coordinates.x;
    coordinates.y = this->coordinates.y;
}

bool Object::getObjectStatus()
{
    return this->visible;
}

void Object::setVisible()
{
    this->visible = true;
}

void Object::setUnvisible()
{
    this->visible = false;
}

double Circle::getRadius()
{
    return this->radius;
}

double Circle::functionAbscissus(const double x)
{
    // function : (x - a)^2  + (y - b)^2 = r^2
    double y = sqrt(pow(this->radius,2) - pow((x - this->coordinates.x),2)) - this->coordinates.y;
    return y;
}

double Circle::functionOrdinatus(const double y)
{
    // function : (x - a)^2  + (y - b)^2 = r^2
    double x = sqrt(pow(this->radius,2) - pow((y - this->coordinates.y),2)) - this->coordinates.x;
    return x;
}
