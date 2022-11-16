#pragma once

#include "Core/global.h"
#include <spdlog/spdlog.h>

class Application
{
protected:
    virtual void init(){};
    virtual void update(){};
    virtual void draw(){};
    virtual void determinate(){};
    virtual void on_resize(){};

public:
    void set_scene(Scene& scene);

private:
    void app_init();
    void app_determinate();

public:
    bool running = true;

    Application();
    int run();
};