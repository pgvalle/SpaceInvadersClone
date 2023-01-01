#ifndef MENU_H
#define MENU_H

struct MenuItem
{
    char label[32];
    Function CauseEffect;
};
typedef struct MenuItem MenuItem;

struct Menu
{
    int x, y;

    MenuItem* items;
    int itemSelected;
};
typedef struct Menu Menu;


#endif // MENU_H
