#include <string.h>
#include "../Application.h"

#define GET_APP struct Application* app = GetAppInstance()

struct MenuItem
{
    char label[32];
    void (*doAction)();
};

struct Menu
{
    struct MenuItem* items;
    int itemCount;
    int itemSelected;
} startMenu, mainMenu, optionsMenu, *currentMenu;

void ToMainMenu()
{
    currentMenu = &mainMenu;
}

void ToGameplayState()
{
    
}

void ToOptionsMenu()
{
    currentMenu = &optionsMenu;
}

void QuitGame()
{
    GetAppInstance()->shouldClose = true;
}

void InitMainMenuState()
{
    // start
    startMenu.itemCount = 1;
    startMenu.itemSelected = 0;
    startMenu.items = malloc(startMenu.itemCount * sizeof(*startMenu.items));
    // item
    strcpy(startMenu.items[0].label, "press any key to start");
    startMenu.items[0].doAction = ToMainMenu;

    // main
    mainMenu.itemCount = 3;
    mainMenu.itemSelected = 0;
    mainMenu.items = malloc(mainMenu.itemCount * sizeof(*mainMenu.items));
    // item 0
    strcpy(mainMenu.items[0].label, "play");
    mainMenu.items[0].doAction = ToGameplayState;
    // item 1
    strcpy(mainMenu.items[1].label, "options");
    mainMenu.items[1].doAction = ToOptionsMenu;
    // item 2
    strcpy(mainMenu.items[2].label, "quit");
    mainMenu.items[2].doAction = QuitGame;

    // options
    optionsMenu.itemCount = 1;
    optionsMenu.itemSelected = 0;
    optionsMenu.items = malloc(optionsMenu.itemCount * sizeof(*optionsMenu.items));
    // item 0
    strcpy(optionsMenu.items[0].label, "nothing");
    optionsMenu.items[0].doAction = ToMainMenu;

    currentMenu = &startMenu;
}

void DestroyMainMenuState()
{
}

int flickTimer = 0;
int display = false;

void UpdateMainMenuState()
{
    GET_APP;

    if (app->event.type == SDL_KEYUP)
    {
        SDL_KeyCode key = app->event.key.keysym.sym;
        if (key == SDLK_UP)
        {
            currentMenu->itemSelected--;
            if (currentMenu->itemSelected < 0)
                currentMenu->itemSelected = currentMenu->itemCount - 1;
            flickTimer = 0;
            display = true;
        }
        else if (key == SDLK_DOWN)
        {
            currentMenu->itemSelected = (currentMenu->itemSelected + 1) % currentMenu->itemCount;
            flickTimer = 0;
            display = true;
        }
        else if (key == SDLK_RETURN)
        {
            currentMenu->items[currentMenu->itemSelected].doAction();
            flickTimer = 0;
            display = true;
        }
        printf("\x1b[2J\r%d", currentMenu->itemSelected);
        fflush(stdout);
    }
}



void RenderMainMenuState()
{
    GET_APP;
    int y = 0;

    flickTimer += app->frameTime;
    for (int i = 0; i < currentMenu->itemCount; i++)
    {
        if (flickTimer >= 100 && i == currentMenu->itemSelected)
        {
            flickTimer = 0;
            display = !display;
        }

        if (i == currentMenu->itemSelected)
        {
            if (display)
                RenderText(0, y, currentMenu->items[i].label);
            y += 8;
            continue;
        }

        RenderText(0, y, currentMenu->items[i].label);
        y += 8;
    }
}