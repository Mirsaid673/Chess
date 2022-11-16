#pragma once

#include "../Engine/Core/application.h"
#include <list>

class AvailableCells : public Node
{
private:
public:
    std::list<glm::ivec2> coords;

    void draw() override
    {
        Renderer::setMaterial(material);
        Transform2d transform;
        for (const auto &coord : coords)
        {
            transform.origin = coord;
            Renderer::setTransform2d(parent->getTransform2d() * transform.getMatrix());
            Renderer::drawVAO(quad);
        }
    }

    void clear()
    {
        coords.clear();
    }

    void push_back(const glm::ivec2& coord)
    {
        coords.push_back(coord);
    }

    static RID quad;
    static Material material;
};
