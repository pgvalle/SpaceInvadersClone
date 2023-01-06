#include "core/app.h"

int main(int argc, char** args)
{
    app_options_t options = {
        .fullscreen = false,
        .volume = 4,
        .scale = 1
    };
    app_init(&options);
    app_run();
    app_destroy();

    return 0;
}
