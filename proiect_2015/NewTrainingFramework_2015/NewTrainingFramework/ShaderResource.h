#pragma once
#include "../Utilities/Math.h"
#include "Globals.h"
#include <string>
#include <vector>

struct ShaderResource {
    int id;
    std::string vs;
    std::string fs;
};

class ShaderR
{
public:
    ShaderResource* sr;
    GLuint programId,vertexShader, fragmentShader;
    GLint positionAttribute;
    GLint colorAttribute;
    GLint matrixUniform;
    GLint cameraUniform;
    GLint textureUniform[4];
    GLint uvAttribute;
    GLfloat Nuniform;
    GLint heightsUniform;
    GLint deplasament;
    GLint u_cube_texture;
    GLint s_r;
    GLint b_r;
    GLint fog_color;
    GLint dispMax;
    GLint u_Time;
    GLint camera_pos;
    GLint modelMatrix;
    GLint normAttribute;
    GLint amb_color;
    GLint amb_ratio;
    GLint diff_color;
    GLint spec_color;
    GLint c_diff;
    GLint c_spec;
    GLint light_direction;
    GLint specPower;
    ShaderR() : sr(nullptr), programId(0) {}
    ~ShaderR() { delete sr; }
    int Load();
};

