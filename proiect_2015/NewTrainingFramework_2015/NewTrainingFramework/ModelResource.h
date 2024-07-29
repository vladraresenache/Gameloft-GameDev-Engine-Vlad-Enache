#pragma once
#include "../Utilities/Math.h"
#include "Globals.h"
#include <string>
#include <vector>
#include "Vertex.h"

struct ModelResource {
    int id;
    std::string file;
};

class ModelR
{
public:
    ModelResource* mr;
    GLuint iboId, wiredboId, vboId;

    int nrIndici, nrIndiciWired;
    ModelR() : mr(nullptr), iboId(0), wiredboId(0), vboId(0), nrIndici(0), nrIndiciWired(0) {}
    ~ModelR() { delete mr; }
    void readNfg(const char* caleNfg, std::vector <Vertex>* VertexVV, std::vector <unsigned short>* IndexVV);
    float contentPicker(std::string line, char c, int& index);
    void Load();
    void generateModel(int nr_celule, int dim_cel, int offsetY, std::vector<Vertex> *TVert, std::vector<unsigned short int> *TInd);
};

