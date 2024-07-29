#include "stdafx.h"
#include "ModelResource.h"
#include "../Include/rapidxml.hpp"
#include "../Include/rapidxml_utils.hpp"
#include "../Include/rapidxml_iterators.hpp"
#include "../Include/rapidxml_print.hpp"
#include <iostream>
#include "Vertex.h"
using namespace rapidxml;



float ModelR::contentPicker(std::string line, char c, int& index)
{
	std::string placeholder;
	while (line[index] != c)
	{
		placeholder = placeholder + line[index];
		index++;
	}
	index += 2;
	return std::stof(placeholder);
}

void ModelR::readNfg(const char* caleNfg, std::vector <Vertex>  *VertexVV, std::vector <unsigned short> *IndexVV)
{
	std::ifstream fin(caleNfg);
	std::string line;
	std::string placeholder;
	std::getline(fin, line);
	int i = 12;
	while (line[i] != NULL)
	{
		placeholder = placeholder + line[i];
		i++;
	}
	int n_v = std::stoi(placeholder);
	placeholder.erase();
	Vertex v;
	for (i = 0; i < n_v; i++)
	{
		int k = 11;
		std::getline(fin, line);
		v.pos.x = contentPicker(line, ',', k);
		v.pos.y = contentPicker(line, ',', k);
		v.pos.z = contentPicker(line, ']', k);
		k += 7; //norm

		v.norm.x = contentPicker(line, ',', k);
		v.norm.y = contentPicker(line, ',', k);
		v.norm.z = contentPicker(line, ']', k);

		k += 9; //binorm

		v.binorm.x = contentPicker(line, ',', k);
		v.binorm.y = contentPicker(line, ',', k);
		v.binorm.z = contentPicker(line, ']', k);

		k += 6;

		v.tgt.x = contentPicker(line, ',', k);
		v.tgt.y = contentPicker(line, ',', k);
		v.tgt.z = contentPicker(line, ']', k);

		k += 5;

		v.uv.x = contentPicker(line, ',', k);
		v.uv.y = contentPicker(line, ']', k);

		VertexVV->push_back(v);
	}
	std::getline(fin, line);

	i = 11;
	while (line[i] != NULL)
	{
		placeholder = placeholder + line[i];
		i++;
	}
	int n_i = std::stoi(placeholder);
	placeholder.erase();

	int k = 7;
	for (i = 0; i < n_i / 3; i++)
	{
		int k = 7;
		std::getline(fin, line);
		int index = contentPicker(line, ',', k);
		IndexVV->push_back(index);

		index = contentPicker(line, ',', k);
		IndexVV->push_back(index);

		index = contentPicker(line, NULL, k);
		IndexVV->push_back(index);
	}

}

void readModels(const std::string& filePath, ModelResource* models, int dest_id)
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

    xml_node<>* models_node = root_node->first_node("models");
    if (models_node) {
        for (xml_node<>* folder_node = models_node->first_node("folder"); folder_node; folder_node = folder_node->next_sibling("folder")) {
            for (xml_node<>* model_node = folder_node->first_node("model"); model_node; model_node = model_node->next_sibling("model")) {
                ModelResource model;
                model.id = std::atoi(model_node->first_attribute("id")->value());
				if (model.id == dest_id)
				{
					models->id = dest_id;
					models->file = model_node->first_node("file")->value();
				}
					
            }
        }
    }
}


void ModelR::Load()
{
	    std::vector <Vertex>  VertexVV;
	    std::vector <unsigned short> IndexVV;
        readNfg(mr->file.c_str(), &VertexVV, &IndexVV);

        
        glGenBuffers(1, &vboId);
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
        glBufferData(GL_ARRAY_BUFFER, VertexVV.size() * sizeof(Vertex), VertexVV.data(), GL_STATIC_DRAW);

        
        glGenBuffers(1, &iboId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexVV.size() * sizeof(unsigned short), IndexVV.data(), GL_STATIC_DRAW);

		
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		nrIndici = IndexVV.size();

		int err;
		err = glGetError();
}

void ModelR::generateModel(int nr_cel, int dim_cel, int offsetY, std::vector<Vertex>* TVert, std::vector<unsigned short int>* TInd)
{
    Vertex v;
    int half_cells = nr_cel / 2;

    
    TVert->clear();
    TInd->clear();

    
    for (int i = -half_cells, i1=0; i <= half_cells; ++i, ++i1) {
        for (int j = -half_cells, j1=0; j <= half_cells; ++j, ++j1) {
            v.pos.x = i * dim_cel;
            v.pos.y = offsetY;
			v.pos.z = j * dim_cel;

           
           v.uv.x = (float)j1;
           v.uv.y = (float)i1;

            TVert->push_back(v);
        }
    }

    for (int i = 0; i < nr_cel; ++i) {
        for (int j = 0; j < nr_cel; ++j) {
            int start_index = (i * (nr_cel + 1)) + j;

            // First triangle
			TInd->push_back(start_index);
			TInd->push_back(start_index + nr_cel + 1);
			TInd->push_back(start_index + nr_cel + 2);
            

            // Second triangle
			TInd->push_back(start_index);
			TInd->push_back(start_index + nr_cel + 2);
			TInd->push_back(start_index + 1);
        }
    }



	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, TVert->size() * sizeof(Vertex), TVert->data(), GL_STATIC_DRAW);


	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, TInd->size() * sizeof(unsigned short), TInd->data(), GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	nrIndici = TInd->size();

	int err;
	err = glGetError();

}
