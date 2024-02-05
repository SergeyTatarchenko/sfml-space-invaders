/**
 * @file main.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#include "canvas.hpp"

//initial game window setup
constexpr unsigned int width     = 500;
constexpr unsigned int height    = 500;
constexpr unsigned int framerate = 60;

int main()
{
    Canvas canvas(width,height,framerate);
    //run game until window closed
    canvas.gameTask();
    return 0;
}