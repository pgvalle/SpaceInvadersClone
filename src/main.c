#include "core/app.h"

int main(int argc, char** args)
{
    app_init();
    app_run();
    app_destroy();

    return 0;
}