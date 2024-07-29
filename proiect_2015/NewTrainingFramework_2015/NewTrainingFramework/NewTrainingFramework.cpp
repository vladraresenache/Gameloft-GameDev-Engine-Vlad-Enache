// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include <iostream>
#include "Camera.h"
#include <vector>
#include <fstream>
#include <istream>
#include <string>
#include <cstring>
#include "ResourceManager.h"
#include "SceneManager.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include


#define PI 3.1415

GLuint vboId;
GLuint modelVboId;
GLuint modelIboId;
GLuint idTexture;
GLuint lineVboId;
Shaders myShaders;
Shaders lineShaders;
Shaders modelShaders;
std::vector <Vertex>  VertexV;
std::vector <unsigned short> IndexV;
float angle = 0.0, step = 0.01;
Camera c;
float totalTime;


float contentPickerr(std::string line, char c, int& index)
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

void readNfgg(const char* caleNfg)
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
		v.pos.x = contentPickerr(line, ',', k);
		v.pos.y = contentPickerr(line, ',', k);
		v.pos.z = contentPickerr(line, ']', k);
		k += 7; //norm

		v.norm.x = contentPickerr(line, ',', k);
		v.norm.y = contentPickerr(line, ',', k);
		v.norm.z = contentPickerr(line, ']', k);

		k += 9; //binorm

		v.binorm.x = contentPickerr(line, ',', k);
		v.binorm.y = contentPickerr(line, ',', k);
		v.binorm.z = contentPickerr(line, ']', k);

		k += 6;

		v.tgt.x = contentPickerr(line, ',', k);
		v.tgt.y = contentPickerr(line, ',', k);
		v.tgt.z = contentPickerr(line, ']', k);

		k += 5;

		v.uv.x = contentPickerr(line, ',', k);
		v.uv.y = contentPickerr(line, ']', k);

		VertexV.push_back(v);
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
		int index = contentPickerr(line, ',', k);
		IndexV.push_back(index);

		index = contentPickerr(line, ',', k);
		IndexV.push_back(index);

		index = contentPickerr(line, NULL, k);
		IndexV.push_back(index);
	}

}




int Init ( ESContext *esContext )
{
	
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	/*int width, height, bpp;
	char* pixelArray;
	pixelArray = LoadTGA("../../NewResourcesPacket/Textures/Croco.tga", &width, &height, &bpp);
    */

	/*std::vector <Vertex> VertexV;
	std::vector <unsigned short> IndexV;

	readNfg("../NewResourcesPacket/Models", VertexV, IndexV);*/
	
	//-----------------------------------------------------------------------------------------------------------


	//triangle data (heap)
	/*
	Vertex verticesData[6];

	verticesData[0].pos.x =  0.5f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f; 
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y =  0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  -0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  0.0f;


	verticesData[0].color.x = 1.0f; verticesData[0].color.y = 0.0f; verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f; verticesData[1].color.y = 1.0f; verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f; verticesData[2].color.y = 0.0f; verticesData[2].color.z = 1.0f;

	verticesData[3].pos.x =  0.5f;  verticesData[3].pos.y = 0.5f;   verticesData[3].pos.z = 0.0f;
	verticesData[4].pos.x =  0.5f;  verticesData[4].pos.y =  -0.5f;   verticesData[4].pos.z = 0.0f;
	verticesData[5].pos.x = -0.5f;  verticesData[5].pos.y = -0.5f;  verticesData[5].pos.z = 0.0f;


	verticesData[3].color.x = 1.0f; verticesData[3].color.y = 0.0f; verticesData[3].color.z = 0.0f;
	verticesData[4].color.x = 1.0f; verticesData[4].color.y = 1.0f; verticesData[4].color.z = 0.0f;
	verticesData[5].color.x = 0.0f; verticesData[5].color.y = 0.0f; verticesData[5].color.z = 1.0f;

	Vertex lineVerticesData[2];
	lineVerticesData[0].pos.x = 0.0f; lineVerticesData[0].pos.y = 1.5f; lineVerticesData[0].pos.y = 0.0f;
	lineVerticesData[1].pos.x = 0.0f; lineVerticesData[1].pos.y = -1.5f; lineVerticesData[0].pos.y = 0.0f;

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &lineVboId);
	glBindBuffer(GL_ARRAY_BUFFER, lineVboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticesData), lineVerticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//creation of shaders and program 
	lineShaders.Init("../Resources/Shaders/LineShaderVS.vs", "../Resources/Shaders/LineShaderFS.fs");*/

	//-----------------------------------------------------------------------------------------------------------

	/*readNfgg("../../NewResourcesPacket/Models/Croco.nfg");
	glGenBuffers(1, &modelVboId);
	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	glBufferData(GL_ARRAY_BUFFER, VertexV.size() * sizeof(Vertex), VertexV.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &modelIboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexV.size()* sizeof(unsigned short), IndexV.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	if (bpp == 32)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
	else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelArray);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glBindTexture(GL_TEXTURE_2D, 0);*/
	
	ResourceManager::getInstance()->Init("../Resources/resourceManager.xml");
	SceneManager::getInstance()->Init("../Resources/sceneManager.xml");
	
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	return 0;
	

	//return modelShaders.Init("../Resources/Shaders/ModelShaderVS.vs", "../Resources/Shaders/ModelShaderFS.fs");
	//myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/*glUseProgram(myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	

	
	if(myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (myShaders.colorAttribute!= -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	}

	if (myShaders.cameraUniform != -1)
	{
		glUniformMatrix4fv(myShaders.cameraUniform, 1, GL_FALSE, (float*)(c.viewMatrix*c.perspectiveMatrix).m);
	}


	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glUseProgram(lineShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, lineVboId);

	if (myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	
	if (myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	}

	glDrawArrays(GL_LINES, 0, 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	//---------------------------------------------------------------------------------------------------


	/*
	glUseProgram(modelShaders.program);
	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);

	glBindTexture(GL_TEXTURE_2D, idTexture);

	if (modelShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(modelShaders.positionAttribute);
		glVertexAttribPointer(modelShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (modelShaders.cameraUniform != -1)
	{
		glUniformMatrix4fv(modelShaders.cameraUniform, 1, GL_FALSE, (float*)(c.viewMatrix * c.perspectiveMatrix).m);
	}

	if (modelShaders.textureUniform != -1)
	{
		glUniform1i(modelShaders.textureUniform, 0);
	}
	if (modelShaders.uvAttribute != -1)
	{
		glEnableVertexAttribArray(modelShaders.uvAttribute); 
		glVertexAttribPointer(modelShaders.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Vector3)));
	}

	//glDrawElements(GL_TRIANGLES, IndexV.size(), GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	*/

	
	SceneManager::getInstance()->Draw(); 



	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	angle += step;
	if (angle > 2 * (float)PI)
	{
		angle -= 2 * (float)PI;
	}
	totalTime += deltaTime;
	if (totalTime > Globals::frameTime)
	{
		totalTime -= Globals::frameTime;
		SceneManager::getInstance()->getActiveCamera()->setDeltaTime(Globals::frameTime);
		SceneManager::getInstance()->Update();
	}


}



void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	std::cout << key;
	switch (key) {
		//W
	case 'W':
		SceneManager::getInstance()->getActiveCamera()->moveOz(-1);
		break;
	case 'A':
		SceneManager::getInstance()->getActiveCamera()->moveOx(-1);
		break;
	case 'S':
		SceneManager::getInstance()->getActiveCamera()->moveOz(1);
		break;
	case 'D':
		SceneManager::getInstance()->getActiveCamera()->moveOx(1);
		break;
	case 'Q':
		SceneManager::getInstance()->getActiveCamera()->moveOy(1);
		break;
	case 'E':
		SceneManager::getInstance()->getActiveCamera()->moveOy(-1);
		break;
	case VK_UP:
		SceneManager::getInstance()->getActiveCamera()->rotateOx(1);
		break;
	case VK_DOWN:
		SceneManager::getInstance()->getActiveCamera()->rotateOx(-1);
		break; 
	case VK_LEFT:
		SceneManager::getInstance()->getActiveCamera()->rotateOy(1);
		break;
	case VK_RIGHT:
		SceneManager::getInstance()->getActiveCamera()->rotateOy(-1);
		break;
	case 'O':
		SceneManager::getInstance()->getActiveCamera()->rotateOz(1);
		break;
	case 'P':
		SceneManager::getInstance()->getActiveCamera()->rotateOz(-1);
		break;
	}
}

void Mouse(ESContext* esContext, enum MouseButtons btn, enum MouseEvents event, int x, int y)
{
	if (event == MOUSE_CLICK)
	{
		if (x > Globals::screenWidth / 2)
			c.rotateOz(1);
		else
			c.rotateOz(-1);
	}
	else
		if (event == MOUSE_DOUBLE_CLICK)
		{
			if (x > Globals::screenWidth / 2)
				c.rotateOz(10);
			else
				c.rotateOz(-10);
		}
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	


	printf("Press any key...\n");
	_getch();
	
	return 0;
}

