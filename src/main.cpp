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

constexpr unsigned int framerate = 60;

int main()
{
    Canvas canvas(framerate);
    //run game until window closed
    canvas.gameTask();
    return 0;
}