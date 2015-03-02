#pragma once

#ifndef G_TEXTURE_H__
#define G_TEXTURE_H__

#include "gTools.h"
#include "lodepng.h"

class gTexture {
public:
	GLuint textureID;

	gTexture(char *fileName) {
		std::vector<unsigned char> image;
		unsigned width, height;
		unsigned error = lodepng::decode(image, width, height, fileName);


		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
};


#endif