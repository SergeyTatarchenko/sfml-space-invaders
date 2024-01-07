/**
 * @file main.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#include <iostream> 
#include "canvas.hpp"

int main()
{
    Canvas canvas(500,500,60);
    canvas.spawnEnemies();
    
    canvas.windowEventHandler();
    canvas.~Canvas();
    return 0;
}