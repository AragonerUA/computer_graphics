#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <string>
#include <map>
#include <iostream>

class TextureLoader {
private:
    static std::map<std::string, GLuint> textureCache;
    
public:
    static GLuint createProceduralTexture(const std::string& patternType = "checkerboard") {
        if (textureCache.find(patternType) != textureCache.end()) {
            return textureCache[patternType];
        }
        
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        const int width = 64;
        const int height = 64;
        GLubyte checkImage[width][height][4];
        
        if (patternType == "checkerboard") {
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    int c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
                    checkImage[i][j][0] = (GLubyte) c;
                    checkImage[i][j][1] = (GLubyte) c;
                    checkImage[i][j][2] = (GLubyte) c;
                    checkImage[i][j][3] = (GLubyte) 255; // Alpha
                }
            }
        } 
        else if (patternType == "gradient") {
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    checkImage[i][j][0] = (GLubyte) (255 * i / width);
                    checkImage[i][j][1] = (GLubyte) (255 * j / height);
                    checkImage[i][j][2] = (GLubyte) 128;
                    checkImage[i][j][3] = (GLubyte) 255;
                }
            }
        }
        else if (patternType == "brick") {
            for (int i = 0; i < width; i++) {
                for (int j = 0; j < height; j++) {
                    bool isBrick = ((i % 16 < 15) && (j % 8 < 7)) || 
                                   ((i % 16 > 7) && (j % 16 < 15) && (j % 16 > 7));
                    checkImage[i][j][0] = (GLubyte) (isBrick ? 156 : 200);
                    checkImage[i][j][1] = (GLubyte) (isBrick ? 56 : 70);
                    checkImage[i][j][2] = (GLubyte) (isBrick ? 28 : 35);
                    checkImage[i][j][3] = (GLubyte) 255;
                }
            }
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
        
        textureCache[patternType] = textureID;
        
        return textureID;
    }
    
    static void cleanup() {
        for (auto& pair : textureCache) {
            glDeleteTextures(1, &pair.second);
        }
        textureCache.clear();
    }
};

std::map<std::string, GLuint> TextureLoader::textureCache;

#endif