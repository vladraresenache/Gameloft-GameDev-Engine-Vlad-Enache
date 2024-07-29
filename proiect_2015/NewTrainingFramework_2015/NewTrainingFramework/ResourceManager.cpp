#include "stdafx.h"
#include <map>
#include "ResourceManager.h"
#include "../Include/rapidxml.hpp"
#include "../Include/rapidxml_utils.hpp"
#include "../Include/rapidxml_iterators.hpp"
#include "../Include/rapidxml_print.hpp"
#include <iostream>
using namespace rapidxml;

ResourceManager* ResourceManager::spInstance = NULL;

ResourceManager* ResourceManager::getInstance()
{
	if (!spInstance)
	{
		spInstance = new ResourceManager;
	}
	return spInstance;
}


void ResourceManager::Init(const std::string& filePath)
{
    xml_document<> doc;
    xml_node<>* root_node = nullptr;

    std::ifstream theFile(filePath);

    if (!theFile.is_open()) {
        std::cerr << "Error: Could not open file" << std::endl;
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

    xml_node<>* models_node = root_node->first_node("models");
    if (models_node) {
        xml_node<>* folder_node = models_node->first_node("folder");
        std::string folder = folder_node->first_attribute("path")->value();
            for (xml_node<>* model_node = folder_node->first_node("model"); model_node; model_node = model_node->next_sibling("model")) {
                ModelResource* model = new ModelResource;
                model->id = std::atoi(model_node->first_attribute("id")->value());
                model->file = folder;
                model->file.append(model_node->first_node("file")->value());
                std::cout << model->id << "   " << model->file << std::endl;
                mm[model->id] = model;

            }
    }

    xml_node<>* shaders_node = root_node->first_node("shaders");
    if (shaders_node) {
        xml_node<>* folder_node = shaders_node->first_node("folder");
        std::string folder = folder_node->first_attribute("path")->value();
        {
            for (xml_node<>* shader_node = folder_node->first_node("shader"); shader_node; shader_node = shader_node->next_sibling("shader")) {
                ShaderResource* shader = new ShaderResource;
                shader->id = std::atoi(shader_node->first_attribute("id")->value());
                shader->vs = folder;
                shader->vs.append(shader_node->first_node("vs")->value()); 
                shader->fs = folder;
                shader->fs.append(shader_node->first_node("fs")->value());
                sm[shader->id] = shader;
            }
        }
    }

    xml_node<>* textures_node = root_node->first_node("textures");
    if (textures_node) {
        xml_node<>* folder_node = textures_node->first_node("folder");
        std::string folder = folder_node->first_attribute("path")->value();
        for (xml_node<>* texture_node = folder_node->first_node("texture"); texture_node; texture_node = texture_node->next_sibling("texture")) {
            TextureResource* texture = new TextureResource;
            texture->id = std::atoi(texture_node->first_attribute("id")->value());
            texture->type = texture_node->first_attribute("type")->value();
            texture->file = folder;
            texture->file.append(texture_node->first_node("file")->value());
            texture->min_filter = texture_node->first_node("min_filter")->value();
            texture->mag_filter = texture_node->first_node("mag_filter")->value();
            texture->wrap_s = texture_node->first_node("wrap_s")->value();
            texture->wrap_t = texture_node->first_node("wrap_t")->value();
            tm[texture->id] = texture;
        }
    }
}

TextureR* ResourceManager::loadTexture(int dest_id) 
{
	if (loaded_tm.find(dest_id) == loaded_tm.end()) 
	{
		TextureResource* a = tm.find(dest_id)->second; 
        TextureR* nt = new TextureR;
        nt->tr = a; 
		nt->Load();  
		loaded_tm[dest_id] = nt;
	}
    return loaded_tm[dest_id];
}

ShaderR* ResourceManager::loadShader(int dest_id)
{
	if (loaded_sm.find(dest_id) == loaded_sm.end())
	{
		ShaderResource* a = sm.find(dest_id)->second;
        ShaderR* ns = new ShaderR;
        ns->sr = a;
		ns->Load();
		loaded_sm[dest_id] = ns;
	}
    return loaded_sm[dest_id];
}

ModelR* ResourceManager::loadModel(int dest_id)
{
	if (loaded_mm.find(dest_id) == loaded_mm.end())
	{
		ModelResource* a = mm.find(dest_id)->second;
        ModelR* nm = new ModelR;
        nm->mr = a;
		nm->Load();
		loaded_mm[dest_id] = nm;
	}
    return loaded_mm[dest_id];
}

void ResourceManager::freeResources()
{
	mm.clear();
	sm.clear();
	tm.clear();
}

void ResourceManager::showEverything()
{
    std::map<int, ModelResource*>::iterator it_m=mm.begin(); 
    std::map<int, ShaderResource*>::iterator it_s=sm.begin(); 
    std::map<int, TextureResource*>::iterator it_t=tm.begin();

    std::cout << "Model" << std::endl;
    while (it_m != mm.end())
    {
        std::cout << "Key: " << it_m->first << ", Value: " << it_m->second->file<< std::endl;
        ++it_m;
    }
    std::cout << "Shader" << std::endl;
    while (it_s != sm.end())
    {
        std::cout << "Key: " << it_s->first << ", Value: " << it_s->second->vs << "   " << it_s->second->fs << std::endl;
        ++it_s;
    }
    std::cout << "Texture" << std::endl;
    while (it_t != tm.end())
    {
        std::cout << "Key: " << it_t->first << ", Value: " << it_t->second->file << std::endl;
        ++it_t;
    }



    std::map<int, ModelR*>::iterator it_lm = loaded_mm.begin();
    std::map<int, ShaderR*>::iterator it_ls = loaded_sm.begin();
    std::map<int, TextureR*>::iterator it_lt = loaded_tm.begin();



    std::cout << "Model" << std::endl;
    while (it_lm != loaded_mm.end())
    {
        std::cout << "Key: " << it_lm->first << ", Value: " << it_lm->second->mr->file << std::endl;
        ++it_lm;
    }
    std::cout << "Shader" << std::endl;
    while (it_ls != loaded_sm.end())
    {
        std::cout << "Key: " << it_ls->first << ", Value: " << it_ls->second->sr->vs << "   " << it_ls->second->sr->fs << std::endl;
        ++it_ls;
    }
    std::cout << "Texture" << std::endl;
    while (it_lt != loaded_tm.end())
    {
        std::cout << "Key: " << it_lt->first << ", Value: " << it_lt->second->tr->file << std::endl;
        ++it_lt;
    }


}

ResourceManager::ResourceManager()
{
}