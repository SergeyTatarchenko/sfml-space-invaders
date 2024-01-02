/**
 * @file object.hpp
 *
 * @brief 
 *
 * @author Siarhei Tatarchanka
 * Contact: zlojdigger@gmail.com
 *
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <cstdint>


struct Coordinates
{
    double x;
    double y;
};

class Object
{
    public:

        void setPosition(const double x, const double y);
        void setVisible();
        void setUnvisible();
        void getPosition(Coordinates& coordinates);
        bool getObjectStatus();
        double functionAbscissus(const double x)
        {
            (void)x;
            return 0.0;
        }
        double functionOrdinatus(const double y)
        {
            (void)x;
            return 0.0;
        }
        
    protected:
        /// @brief object state flag, whether it will be displayed in coordinates or not.
        bool visible;
        /// @brief  object coordinates in Cartesian coordinate system
        Coordinates coordinates;
        /// @brief dx/dt for the object traektory
        double speed;
};

class Circle : public Object
{
    public:
        Circle(double radius = 0.0)
        {
            this->radius = radius;
        }
        double getRadius();
        double functionAbscissus(const double x);
        double functionOrdinatus(const double y);

    protected:
        double radius;
};

#endif //OBJECT_H
