#include "stdafx.h"
#include "ShaderResource.h"
#include "../Include/rapidxml.hpp"
#include "../Include/rapidxml_utils.hpp"
#include "../Include/rapidxml_iterators.hpp"
#include "../Include/rapidxml_print.hpp"
#include <iostream>
#include "Shaders.h"
#include "ModelResource.h"
using namespace rapidxml;

void readShaders(const std::string& filePath, ShaderResource* shaders,int dest_id)
{
    xml_document<> doc;
    xml_node<>* root_node = nullptr;

    std::ifstream theFile(filePath);

    if (!theFile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    try {
        doc.parse<0>(&buffer[0]);
    }
    catch (const parse_error& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return;
    }

    root_node = doc.first_node("resourceManager");
    if (!root_node) {
        std::cerr << "Error: No root node found." << std::endl;
        return;
    }

    xml_node<>* shaders_node = root_node->first_node("shaders");
    if (shaders_node) {
        for (xml_node<>* folder_node = shaders_node->first_node("folder"); folder_node; folder_node = folder_node->next_sibling("folder")) {
            for (xml_node<>* shader_node = folder_node->first_node("shader"); shader_node; shader_node = shader_node->next_sibling("shader")) {
                ShaderResource shader;
                shader.id = std::atoi(shader_node->first_attribute("id")->value());
                if (shader.id == dest_id)
                {
                    shaders->id == shader.id;
                    shaders->vs = shader_node->first_node("vs")->value();
                    shaders->fs = shader_node->first_node("fs")->value();
                }
            }
        }
    }
}

int ShaderR::Load()
{

    //readShaders(filePath, sr, dest_id);

    char* v = new char[sr->vs.length() + 1];
    strcpy(v, sr->vs.data());
    vertexShader = esLoadShader(GL_VERTEX_SHADER, v);
    
    if (vertexShader == 0)
        return -1;

    char* f = new char[sr->fs.length() + 1];
    strcpy(f, sr->fs.data());
    fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, f);

    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return -2;
    }

    programId = esLoadProgram(vertexShader, fragmentShader);

    //finding location of uniforms / attributes
    positionAttribute = glGetAttribLocation(programId, "a_posL");
    colorAttribute = glGetAttribLocation(programId, "a_color");
    matrixUniform = glGetUniformLocation(programId, "u_rotation");
    cameraUniform = glGetUniformLocation(programId, "uMVP");
    Nuniform = glGetUniformLocation(programId, "N");
    textureUniform[0] = glGetUniformLocation(programId, "u_texture0");
    textureUniform[1] = glGetUniformLocation(programId, "u_texture1");
    textureUniform[2] = glGetUniformLocation(programId, "u_texture2");
    textureUniform[3] = glGetUniformLocation(programId, "u_texture3");
    heightsUniform = glGetUniformLocation(programId, "u_height");
    uvAttribute = glGetAttribLocation(programId, "a_uv");
    u_cube_texture = glGetUniformLocation(programId, "u_cube_texture");
    deplasament = glGetUniformLocation(programId, "deplasament");
    s_r = glGetUniformLocation(programId, "s_r");
    b_r = glGetUniformLocation(programId, "b_r");
    fog_color = glGetUniformLocation(programId, "fog_color");
    dispMax = glGetUniformLocation(programId, "u_dispMax");
    u_Time = glGetUniformLocation(programId, "u_Time");
    camera_pos = glGetUniformLocation(programId, "u_cam_pos");
    modelMatrix = glGetUniformLocation(programId, "uModel");
    normAttribute = glGetAttribLocation(programId, "a_norm");
    c_diff = glGetUniformLocation(programId, "c_diff_light");
    amb_color = glGetUniformLocation(programId, "u_amb_color");
    amb_ratio = glGetUniformLocation(programId, "u_amb_ratio");
    c_spec = glGetUniformLocation(programId, "c_spec_light");
    light_direction = glGetUniformLocation(programId, "u_light_direction");
    specPower = glGetUniformLocation(programId, "specPower");
    return 0;
}
