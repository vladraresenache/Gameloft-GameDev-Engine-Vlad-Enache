#include "stdafx.h"
#include "TextureResource.h"
#include "ModelResource.h"
#include <iostream>
#include "../Utilities/TGA.h"

void TextureR::Load()
{
    int width, height, bpp;
    char* pixelArray = LoadTGA(tr->file.c_str(), &width, &height, &bpp);

    glGenTextures(1, &texId);
    if (tr->type == "2d")
    {
        glBindTexture(GL_TEXTURE_2D, texId);
        if (bpp == 32)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelArray);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, strcmp(tr->wrap_s.c_str(), "CLAMP_TO_EDGE") == 0 ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, strcmp(tr->wrap_t.c_str(), "CLAMP_TO_EDGE") == 0 ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, strcmp(tr->mag_filter.c_str(), "LINEAR") == 0 ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, strcmp(tr->min_filter.c_str(), "LINEAR") == 0 ? GL_LINEAR : GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else if (tr->type == "cube")
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

        int faceWidth = width / 4;
        int faceHeight = height / 3;

        int offsets[6][2] = {
            {2, 1},  // POSITIVE_X
            {0, 1},  // NEGATIVE_X
            {1, 2},  // POSITIVE_Y
            {1, 0},  // NEGATIVE_Y
            {1, 1},  // POSITIVE_Z
            {3, 1}   // NEGATIVE_Z
        };

        for (int i = 0; i < 6; i++)
        {
            int offsetX = offsets[i][0] * faceWidth;
            int offsetY = offsets[i][1] * faceHeight;
            char* subBuffer = new char[faceWidth * faceHeight * (bpp / 8)];

            for (int y = 0; y < faceHeight; y++)
            {
                for (int x = 0; x < faceWidth; x++)
                {
                    int srcIndex = ((y + offsetY) * width + (x + offsetX)) * (bpp / 8);
                    int destIndex = ((faceHeight - 1 - y) * faceWidth + x) * (bpp / 8); // Flip vertically
                    for (int k = 0; k < bpp / 8; k++)
                    {
                        subBuffer[destIndex + k] = pixelArray[srcIndex + k];
                    }
                }
            }

            if (bpp == 32)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, faceWidth, faceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, subBuffer);
            else
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, faceWidth, faceHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, subBuffer);

            delete[] subBuffer;
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    delete[] pixelArray;

    int err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}
