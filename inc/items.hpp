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

        Invader(const float x = 0.f, const float y = 0.f);
        void moveAlongTrajektory(unsigned int framerate);
    
    private:
    
        float def_x;
        float def_y;
};

#endif //ITEMS_H
