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

#define INVADER_WIDTH (int)(20)
#define INVADER_HEIGHT (int)(20)

#define SHELL_WIDTH (int)(5)
#define SHELL_HEIGHT (int)(5)

enum class ShellTypes
{
    ENEMY_SHELL,
    PLAYER_SHELL
};

class Invader : public Object
{
    public:
        /// @brief default constructor
        /// @param x initial coordinate for x asis
        /// @param y initial coordinate for y asis
        /// @param speed object speed on canvas
        /// @param visible object visibility on canvas
        Invader(float x, float y, float speed, bool visible);
        /// @brief change object position according to internal trajektory function
        /// @param framerate  canvas framerate
        void moveAlongTrajectory(unsigned int framerate);
    
    private:
        /// @brief object position counter, used in moveAlongTrajectory function 
        int position_counter;
};

class Shell : public Object
{
    public:
        /// @brief default constructor
        /// @param x initial coordinate for x asis
        /// @param y initial coordinate for y asis
        /// @param speed object speed on canvas
        /// @param shell_type shell type (who shot)
        Shell(float x, float y, float speed, ShellTypes shell_type);
        /// @brief check instance shell type
        /// @return enum class with ShellTypes
        const ShellTypes getShellType();
        /// @brief change object position according to internal trajektory function
        /// @param framerate  canvas framerate
        void moveAlongTrajectory(unsigned int framerate);
    
    private:
        /// @brief shell type (who shot this shell)
        ShellTypes shell_type;
};

#endif //ITEMS_H
