#ifndef MENU_H
#define MENU_H

// Quick alias for void function()
typedef void (*Function)();

struct Menu
{
    struct MenuItem
    {
        char label[32];
        Function action;
    }* items;
    int itemCount;
    int itemCurrent;

    int x, y;

    Function Update;
    Function Render;
};


#endif // MENU_H
