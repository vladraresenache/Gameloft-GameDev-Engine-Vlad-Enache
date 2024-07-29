#include "stdafx.h"
#include "Camera.h"
#include "SceneManager.h"
#include "TerrainObject.h"
#include "SkyBox.h"
#include "ResourceManager.h"
#include "fireObject.h"
#include <iostream>

using namespace rapidxml;

SceneManager* SceneManager::spInstance = NULL;

SceneManager* SceneManager::getInstance()
{
    if (!spInstance)
    {
        spInstance = new SceneManager;
    }
    return spInstance;
}

void SceneManager::Init(const std::string& xmlFile)
{
    xml_document<> doc;
    xml_node<>* root_node = nullptr;

    std::ifstream theFile(xmlFile);

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

    root_node = doc.first_node("sceneManager");
    if (!root_node) {
        std::cerr << "Error: No root node found." << std::endl;
        return;
    }


    xml_node<>* gameNameNode = root_node->first_node("gameName");
    if (gameNameNode) {
        gameName = gameNameNode->value();
    }


    xml_node<>* screenNode = root_node->first_node("defaultScreenSize");
    if (screenNode) {
        screenProp.width = std::stoi(screenNode->first_node("width")->value());
        screenProp.height = std::stoi(screenNode->first_node("height")->value());
    }


    xml_node<>* backgroundColorNode = root_node->first_node("backgroundColor");
    if (backgroundColorNode) {
        screenProp.r = std::stof(backgroundColorNode->first_node("r")->value());
        screenProp.g = std::stof(backgroundColorNode->first_node("g")->value());
        screenProp.b = std::stof(backgroundColorNode->first_node("b")->value());
    }

    xml_node<>* fogNode = root_node->first_node("fog"); 
    if (fogNode) {



        s_r = std::stof(fogNode->first_node("s_r")->value());
        b_r = std::stof(fogNode->first_node("b_r")->value());

        // Parsing fog color
        xml_node<>* colorNode = fogNode->first_node("color");
        if (colorNode) {
            fog_color.x = std::stof(colorNode->first_node("r")->value());
            fog_color.y = std::stof(colorNode->first_node("g")->value());
            fog_color.z = std::stof(colorNode->first_node("b")->value());
        }
        else {
            std::cerr << "Error: <color> node missing inside <fog>." << std::endl;
            return;
        }
    }


    xml_node<>* controlsNode = root_node->first_node("controls");
    if (controlsNode) {
        std::vector<Control> controlsVec;
        for (xml_node<>* controlNode = controlsNode->first_node("control"); controlNode; controlNode = controlNode->next_sibling("control")) {
            Control* control = new Control;
            control->key = controlNode->first_node("key")->value();
            control->action = controlNode->first_node("action")->value();
            screenProp.controls.push_back(control);
        }
    }


    xml_node<>* actCamera = root_node->first_node("activeCamera");
    activeCamera = std::stof(actCamera->value());
    xml_node<>* camerasNode = root_node->first_node("cameras");
    if (camerasNode)
    {
        for (xml_node<>* cameraNode = camerasNode->first_node("camera"); cameraNode; cameraNode = cameraNode->next_sibling("camera"))
        {
            Camera* cam = new Camera;
            cam->id = std::stoi(cameraNode->first_attribute("id")->value());
            cam->position = Vector3(
                std::stof(cameraNode->first_node("position")->first_node("x")->value()),
                std::stof(cameraNode->first_node("position")->first_node("y")->value()),
                std::stof(cameraNode->first_node("position")->first_node("z")->value())
            );
            cam->target = Vector3(
                std::stof(cameraNode->first_node("target")->first_node("x")->value()),
                std::stof(cameraNode->first_node("target")->first_node("y")->value()),
                std::stof(cameraNode->first_node("target")->first_node("z")->value())
            );
            cam->up = Vector3(
                std::stof(cameraNode->first_node("up")->first_node("x")->value()),
                std::stof(cameraNode->first_node("up")->first_node("y")->value()),
                std::stof(cameraNode->first_node("up")->first_node("z")->value())
            );
            cam->moveSpeed = std::stof(cameraNode->first_node("translationSpeed")->value());
            cam->rotateSpeed = std::stof(cameraNode->first_node("rotationSpeed")->value());
            cam->fov = std::stof(cameraNode->first_node("fov")->value());
            cam->nearPlane = std::stof(cameraNode->first_node("near")->value());
            cam->farPlane = std::stof(cameraNode->first_node("far")->value());
            Cameras[cam->id] = cam;
        }
    }

    xml_node<>* objectsNode = root_node->first_node("objects");
    if (objectsNode) {
        std::vector<SceneObject> objectsVec;
        for (xml_node<>* objectNode = objectsNode->first_node("object"); objectNode; objectNode = objectNode->next_sibling("object")) {

            std::vector<Vertex> TVert;
            std::vector<unsigned short int> TInd;
            std::string modell = objectNode->first_node("type")->value();
            if (modell == "terrain")
            {
                TerrainObject* object = new TerrainObject;
                object->id = std::stoi(objectNode->first_attribute("id")->value());
                object->type = objectNode->first_node("type")->value();

                object->position = Vector3(
                    std::stof(objectNode->first_node("position")->first_node("x")->value()),
                    std::stof(objectNode->first_node("position")->first_node("y")->value()),
                    std::stof(objectNode->first_node("position")->first_node("z")->value()));
                object->rotation = Vector3(
                    std::stof(objectNode->first_node("rotation")->first_node("x")->value()),
                    std::stof(objectNode->first_node("rotation")->first_node("y")->value()),
                    std::stof(objectNode->first_node("rotation")->first_node("z")->value()));
                object->scale = Vector3(
                    std::stof(objectNode->first_node("scale")->first_node("x")->value()),
                    std::stof(objectNode->first_node("scale")->first_node("y")->value()),
                    std::stof(objectNode->first_node("scale")->first_node("z")->value()));
                object->nr_cel = std::stof(objectNode->first_node("nr_cel")->value());
                object->dimCel = std::stof(objectNode->first_node("dim_cel")->value());
                object->offsetY = std::stof(objectNode->first_node("offsetY")->value());
                xml_node<>* heightNode = objectNode->first_node("heights");
                object->heights.x = std::stof(heightNode->first_node("r")->value());
                object->heights.y = std::stof(heightNode->first_node("g")->value());
                object->heights.z = std::stof(heightNode->first_node("b")->value());
                object->model = new ModelR;
                object->shader = new ShaderR;
                object->model->generateModel(object->nr_cel, object->dimCel, object->offsetY, &TVert, &TInd);
                object->shader = ResourceManager::getInstance()->loadShader(std::stoi(objectNode->first_node("shader")->value()));





                xml_node<>* texturesNode = objectNode->first_node("textures");
                if (texturesNode) {

                    for (xml_node<>* textureNode = texturesNode->first_node("texture"); textureNode; textureNode = textureNode->next_sibling("texture")) {
                        TextureR* texture;
                        texture = ResourceManager::getInstance()->loadTexture(std::stoi(textureNode->first_attribute("id")->value()));
                        object->textures.push_back(texture);
                    }
                }
                objects[object->id] = object;
            }
            else
                if (modell == "skybox")
                {
                    SkyBox* object = new SkyBox;
                    object->id = std::stoi(objectNode->first_attribute("id")->value());
                    object->type = objectNode->first_node("type")->value();
                    object->position = Vector3(
                        std::stof(objectNode->first_node("position")->first_node("x")->value()),
                        std::stof(objectNode->first_node("position")->first_node("y")->value()),
                        std::stof(objectNode->first_node("position")->first_node("z")->value()));
                    object->rotation = Vector3(
                        std::stof(objectNode->first_node("rotation")->first_node("x")->value()),
                        std::stof(objectNode->first_node("rotation")->first_node("y")->value()),
                        std::stof(objectNode->first_node("rotation")->first_node("z")->value()));
                    object->scale = Vector3(
                        std::stof(objectNode->first_node("scale")->first_node("x")->value()),
                        std::stof(objectNode->first_node("scale")->first_node("y")->value()),
                        std::stof(objectNode->first_node("scale")->first_node("z")->value()));

                    xml_node<>* followingCameraNode = objectNode->first_node("followingcamera");
                    if (followingCameraNode) {
                        object->followingCamera = Vector3(0, 0, 0);
                        if (followingCameraNode->first_node("ox")) {
                            object->followingCamera.x = 1;
                        }
                        if (followingCameraNode->first_node("oz")) {
                            object->followingCamera.z = 1;
                        }
                    }

                    try {
                        object->model = ResourceManager::getInstance()->loadModel(std::stoi(objectNode->first_node("model")->value()));
                        object->shader = ResourceManager::getInstance()->loadShader(std::stoi(objectNode->first_node("shader")->value()));
                    }
                    catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid argument: " << e.what() << std::endl;
                    }

                    xml_node<>* texturesNode = objectNode->first_node("textures");
                    if (texturesNode) {

                        for (xml_node<>* textureNode = texturesNode->first_node("texture"); textureNode; textureNode = textureNode->next_sibling("texture")) {
                            TextureR* texture;
                            texture = ResourceManager::getInstance()->loadTexture(std::stoi(textureNode->first_attribute("id")->value()));
                            object->textures.push_back(texture);
                        }
                    }
                    objects[object->id] = object;
                }
                else
                    if (modell == "fire")
                    {
                        fireObject* object = new fireObject;
                        object->id = std::stoi(objectNode->first_attribute("id")->value());
                        object->type = objectNode->first_node("type")->value();
                        object->position = Vector3(
                            std::stof(objectNode->first_node("position")->first_node("x")->value()),
                            std::stof(objectNode->first_node("position")->first_node("y")->value()),
                            std::stof(objectNode->first_node("position")->first_node("z")->value()));
                        object->rotation = Vector3(
                            std::stof(objectNode->first_node("rotation")->first_node("x")->value()),
                            std::stof(objectNode->first_node("rotation")->first_node("y")->value()),
                            std::stof(objectNode->first_node("rotation")->first_node("z")->value()));
                        object->scale = Vector3(
                            std::stof(objectNode->first_node("scale")->first_node("x")->value()),
                            std::stof(objectNode->first_node("scale")->first_node("y")->value()),
                            std::stof(objectNode->first_node("scale")->first_node("z")->value()));

                        object->displacementMax = std::stof(objectNode->first_node("dispMax")->value());
                        

                        try {
                            object->model = ResourceManager::getInstance()->loadModel(std::stoi(objectNode->first_node("model")->value()));
                            object->shader = ResourceManager::getInstance()->loadShader(std::stoi(objectNode->first_node("shader")->value()));
                        }
                        catch (const std::invalid_argument& e) {
                            std::cerr << "Invalid argument: " << e.what() << std::endl;
                        }

                        xml_node<>* texturesNode = objectNode->first_node("textures");
                        if (texturesNode) {

                            for (xml_node<>* textureNode = texturesNode->first_node("texture"); textureNode; textureNode = textureNode->next_sibling("texture")) {
                                TextureR* texture;
                                texture = ResourceManager::getInstance()->loadTexture(std::stoi(textureNode->first_attribute("id")->value()));
                                object->textures.push_back(texture);
                            }
                        }
                        objects[object->id] = object;
                    }
                    else
                {
                    SceneObject* object = new SceneObject;
                    object->id = std::stoi(objectNode->first_attribute("id")->value());
                    object->type = objectNode->first_node("type")->value();

                    object->position = Vector3(
                        std::stof(objectNode->first_node("position")->first_node("x")->value()),
                        std::stof(objectNode->first_node("position")->first_node("y")->value()),
                        std::stof(objectNode->first_node("position")->first_node("z")->value()));
                    object->rotation = Vector3(
                        std::stof(objectNode->first_node("rotation")->first_node("x")->value()),
                        std::stof(objectNode->first_node("rotation")->first_node("y")->value()),
                        std::stof(objectNode->first_node("rotation")->first_node("z")->value()));
                    object->scale = Vector3(
                        std::stof(objectNode->first_node("scale")->first_node("x")->value()),
                        std::stof(objectNode->first_node("scale")->first_node("y")->value()),
                        std::stof(objectNode->first_node("scale")->first_node("z")->value()));

                    try {
                        object->model = ResourceManager::getInstance()->loadModel(std::stoi(objectNode->first_node("model")->value()));
                        object->shader = ResourceManager::getInstance()->loadShader(std::stoi(objectNode->first_node("shader")->value()));
                    }
                    catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid argument: " << e.what() << std::endl;
                    }

                    xml_node<>* texturesNode = objectNode->first_node("textures");
                    if (texturesNode) {

                        for (xml_node<>* textureNode = texturesNode->first_node("texture"); textureNode; textureNode = textureNode->next_sibling("texture")) {
                            TextureR* texture;
                            texture = ResourceManager::getInstance()->loadTexture(std::stoi(textureNode->first_attribute("id")->value()));
                            object->textures.push_back(texture);
                        }
                    }
                    objects[object->id] = object;
                }
        }
    }
    
    xml_node<>* ambientalColorNode = root_node->first_node("ambientalColor");
    if (ambientalColorNode)
    {
        ambientalColor = Vector3(
            std::stof(ambientalColorNode->first_node("r")->value()),
            std::stof(ambientalColorNode->first_node("g")->value()),
            std::stof(ambientalColorNode->first_node("b")->value()));
    }
    ambientalRatio = std::stof(root_node->first_node("ambientalRatio")->value());


    xml_node<>* lightsNode = root_node->first_node("lights");
    if (lightsNode)
    {
        Lights* light = new Lights;
        light->light_id = std::stoi(lightsNode->first_node("light")->first_attribute("id")->value());
        xml_node<>* lightNode = lightsNode->first_node("light");
        light->direction = Vector3(
            std::stof(lightNode->first_node("direction")->first_node("x")->value()),
            std::stof(lightNode->first_node("direction")->first_node("y")->value()),
            std::stof(lightNode->first_node("direction")->first_node("z")->value()));
        light->diffuseColor = Vector3(
            std::stof(lightNode->first_node("diffuseColor")->first_node("r")->value()),
            std::stof(lightNode->first_node("diffuseColor")->first_node("g")->value()),
            std::stof(lightNode->first_node("diffuseColor")->first_node("b")->value()));
        light->specularColor = Vector3(
            std::stof(lightNode->first_node("specularColor")->first_node("r")->value()),
            std::stof(lightNode->first_node("specularColor")->first_node("g")->value()),
            std::stof(lightNode->first_node("specularColor")->first_node("b")->value()));
        light->object_id = std::stoi(lightNode->first_node("associatedObject")->value());
        light->type = lightNode->first_node("type")->value();
        light->specPower = std::stof(lightNode->first_node("specPower")->value());
        lights[light->light_id] = light;
    }
    



        /*

        xml_node<>* debugSettingsNode = root_node->first_node("debugSettings");
        if (debugSettingsNode) {

        }*/
    }

void SceneManager::Draw()
{
    std::map<int, SceneObject*>::iterator i = objects.begin();
    while (i != objects.end())
    {
        i->second->Draw();
        ++i;
    }
    
}
void SceneManager::Update()
{
    std::map<int, SceneObject*>::iterator i = objects.begin(); 
    while (i != objects.end()) 
    {
        i->second->Update(); 
        ++i; 
    }
}

Camera* SceneManager::getActiveCamera()
{
    return Cameras[activeCamera];
}


void SceneManager::showEverything()
{
    /*
    std::cout << "Nume: " << gameName << std::endl;
    std::cout << "Ecran: " << "Width - " << screenProp.width << " Height - " << screenProp.height << " Colors RGB- " << screenProp.r << " " << screenProp.g << " " << screenProp.b << std::endl;
    std::cout << "Camera activa: " << activeCamera << std::endl;
    std::cout << "Active camera: " << "Position " << Cameras[activeCamera]->position.y << " " << Cameras[activeCamera]->position.y << " " << Cameras[activeCamera]->position.z << std::endl;
    std::map<int, SceneObject*>::iterator i = objects.begin();
    std::cout << "Obiecte: " << std::endl;
    while (i != objects.end())
    {
        //std::cout << i->first << " " << i->second->position.x << " " << i->second->model.mr->id << std::endl;
        ++i;
    }*/

}

SceneManager::SceneManager(){}