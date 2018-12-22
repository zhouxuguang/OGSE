//
//  NoiseTexture.cpp
//  GLUtilBox
//
//  Created by Zhou,Xuguang on 17/3/26.
//  Copyright © 2017年 zhouxuguang. All rights reserved.
//

#include "NoiseTexture.h"
#include "GLPrecompile.h"
#include "noise.h"
#include "interp.h"

int Generate2DTex(bool seamless, float baseFreq, int w, int h)
{
    
    int width = w;
    int height = h;
    
    // Base frequency for octave 1.
    noise::module::Perlin perlinNoise;
    perlinNoise.SetFrequency(baseFreq);
    
    GLubyte *data = new GLubyte[ width * height * 4 ];
    
    double xRange = 1.0;
    double yRange = 1.0;
    double xFactor = xRange / width;
    double yFactor = yRange / height;
    for( int oct = 0; oct < 4; oct++ )
    {
        perlinNoise.SetOctaveCount(oct+1);
        // Do something about octaves here
        for( int i = 0; i < width; i++ )
        {
            for( int j = 0 ; j < height; j++ )
            {
                double x = xFactor * i;
                double y = yFactor * j;
                double z = 0;
                float val = 0.0f;
                
                double a = perlinNoise.GetValue(x, y, z);
                double b = perlinNoise.GetValue(x + xRange, y, z);
                double c = perlinNoise.GetValue(x, y+yRange, z);
                double d = perlinNoise.GetValue(x+xRange, y+yRange, z);
                
                double xmin = 1.0 - x / xRange;
                double ymin = 1.0 - y / yRange;
                double x1 = noise::LinearInterp(a , b ,xmin);
                double x2 = noise::LinearInterp(c , d, xmin);
                
                val = noise::LinearInterp(x1 , x2, ymin);
                
                // Scale to roughly between 0 and 1
                val = (val + 1.0f) * 0.5f;
                
                // Clamp strictly between 0 and 1
                val = val > 1.0f ? 1.0f : val;
                val = val < 0.0f ? 0.0f : val;
                
                // Store in texture
                data[((j * width + i) * 4) + oct] = (GLubyte) ( val * 255.0f );
            }
        }
    }
    
    GLuint texID;
    glGenTextures(1, &texID);
    
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    delete [] data;
    return texID;
}
