#ifndef MENU_H
#define MENU_H

#include "Function.h"

struct Menu
{
    struct MenuItem
    {
        char label[32];
        Function DoAction;
    }* items;
    int itemCount;
    int itemCurrent;

    int x, y;

    Function Update;
    Function Render;
};


#endif // MENU_H
