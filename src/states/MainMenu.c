#include <string.h>
#include "../Application.h"

#define GET_APP struct Application* app = GetApp()

#define RENDER_SELECTION_TIMEOUT 75

#define MENU_LINE_SPACING 4

struct Menu
{
    struct MenuItem
    {
        char label[32];
        void (*ExecAction)();
    } *items;
    int itemCount;
    int itemSelected;

    int x, y; // relative to center
} startMenu, mainMenu, optionsMenu, * currentMenu;

bool renderSelection;
int menuSelectionTimer;

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
    GetApp()->shouldClose = true;
}

void InitMainMenuState()
{
    // start
    startMenu.itemCount = 1;
    startMenu.itemSelected = 0;
    startMenu.items = malloc(startMenu.itemCount * sizeof(*startMenu.items));
    // item
    strcpy(startMenu.items[0].label, "press any key to start");
    startMenu.items[0].ExecAction = ToMainMenu;
    // position
    startMenu.x = 112;
    startMenu.y = 128;

    // main
    mainMenu.itemCount = 3;
    mainMenu.itemSelected = 0;
    mainMenu.items = malloc(mainMenu.itemCount * sizeof(*mainMenu.items));
    // item 0
    strcpy(mainMenu.items[0].label, "play");
    mainMenu.items[0].ExecAction = ToGameplayState;
    // item 1
    strcpy(mainMenu.items[1].label, "options");
    mainMenu.items[1].ExecAction = ToOptionsMenu;
    // item 2
    strcpy(mainMenu.items[2].label, "quit");
    mainMenu.items[2].ExecAction = QuitGame;
    // position
    mainMenu.x = 112;
    mainMenu.y = 128;

    // options
    optionsMenu.itemCount = 1;
    optionsMenu.itemSelected = 0;
    optionsMenu.items = malloc(optionsMenu.itemCount * sizeof(*optionsMenu.items));
    // item 0
    strcpy(optionsMenu.items[0].label, "nothing");
    optionsMenu.items[0].ExecAction = ToMainMenu;
    // position
    optionsMenu.x = 112;
    optionsMenu.y = 128;

    renderSelection = false;
    menuSelectionTimer = 0;
    currentMenu = &mainMenu;
}

void DestroyMainMenuState()
{
}

void UpdateMainMenuState()
{
    GET_APP;

    menuSelectionTimer += app->frameTime;
    if (menuSelectionTimer > RENDER_SELECTION_TIMEOUT)
    {
        renderSelection = !renderSelection;
        menuSelectionTimer = 0;
    }

    SDL_Event event = app->event;
    if (event.type == SDL_KEYDOWN && !event.key.repeat)
    {
        SDL_KeyCode key = event.key.keysym.sym;
        switch (key)
        {
        case SDLK_UP:
            if (--currentMenu->itemSelected < 0)
                currentMenu->itemSelected = currentMenu->itemCount - 1;
            break;
        case SDLK_DOWN:
            currentMenu->itemSelected = ++currentMenu->itemSelected % currentMenu->itemCount;
            break;
        case SDLK_RETURN:
            currentMenu->items[currentMenu->itemSelected].ExecAction();
            break;
        }
    }
}

void RenderMainMenuState()
{
    GET_APP;

    int x = currentMenu->x;
    int y = currentMenu->y - 4*currentMenu->itemCount - 2*(currentMenu->itemCount - 1);
    for (int i = 0; i < currentMenu->itemCount; i++)
    {
        int xShiftWidth = 4*strlen(currentMenu->items[i].label);

        if (i == currentMenu->itemSelected)
            RenderText(x - xShiftWidth, y, currentMenu->items[i].label, true);
        else
            RenderText(x - xShiftWidth, y, currentMenu->items[i].label, false);

        y += APP_FONT_PTSIZE + MENU_LINE_SPACING;
    }
}