/**
 * @file items.hpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#ifndef ITEMS_H
#define ITEMS_H

#include "object.hpp"


class Invader: public Object
{
    public:

        Invader(float x, float y, float speed, bool visible);
        void moveAlongTrajectory(unsigned int framerate);
    
    private:
        int position_counter;
};

#endif //ITEMS_H
