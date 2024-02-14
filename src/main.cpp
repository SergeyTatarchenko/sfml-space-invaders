/**
 * @file main.cpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 *
 */

#include "canvas.hpp"

constexpr unsigned int framerate = 60;

int main()
{
    Canvas canvas(framerate);
    canvas.runEventLoop();
    return 0;
}