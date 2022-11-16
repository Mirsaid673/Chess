#include <iostream>

#include "test/my_app.h"
#include "chess/app.h"

Application *app;

int main()
{
    Chess mp;
    app = &mp;
    return app->run();
}