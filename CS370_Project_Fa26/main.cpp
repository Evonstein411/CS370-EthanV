// CS370 Final Project
// Fall 2026 Ethan VonStein


#include "app/desertfox.h"

int main(int argc, char**argv)
{
    DesertFox app;
    if (!app.initialize()) return 1;
    app.run();
    return 0;
}














