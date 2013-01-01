//
//  Texture.cpp
//  COMP3004-C2
//
//  Created by Dan Palmer on 24/12/2012.
//  Copyright (c) 2012 Dan Palmer. All rights reserved.
//

#include "Texture.h"

#include "utils.h"

Texture::Texture(char const *filename, int flags, bool repeat) {
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_2D,textureID);
	
	if(glfwLoadTexture2D(filename, flags)) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		exit(EXIT_FAILURE);
	}
	
	glBindTexture(GL_TEXTURE_2D, NULL);
	check("Loaded Texture");
}

void Texture::bind() {
	bind(GL_TEXTURE0);
}

void Texture::bind(int textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
	check("Bound Texture");
}

void Texture::unbind() {
	unbind(GL_TEXTURE0);
}

void Texture::unbind(int textureUnit) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, NULL);
	check("Unbound Texture");
}
