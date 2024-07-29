#pragma once
#include "../Utilities/Math.h"
#include "Globals.h"
#include <string>
#include <vector>
#include "ModelResource.h"



struct TextureResource {
    int id;
    std::string type;
    std::string file;
    std::string min_filter;
    std::string mag_filter;
    std::string wrap_s;
    std::string wrap_t;
};


class TextureR
{
public:
    TextureResource* tr;
    GLuint texId;
    TextureR() : tr(nullptr), texId(0){}
    ~TextureR() { delete tr; }
    void Load();
};
