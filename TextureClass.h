#pragma once
#include <string>
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>

class TextureClass {
public:
	unsigned int textureId;
	char* textureLocation;
	GLenum renderType;

	TextureClass(char* textureLoc, GLenum rndrtype) {
		textureLocation = textureLoc;
		renderType = rndrtype;
		//Setting default settings
		TextureParameters(renderType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
		TextureParameters(renderType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
		TextureParameters(renderType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
		TextureParameters(renderType, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		TextureParameters(renderType, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

		int width, height, nrChannels;
		unsigned char* data = stbi_load(textureLocation, &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture - " << textureLocation << std::endl;
		}
		stbi_image_free(data);
	}
	/*
	Specifies the target to which the texture is bound. 
	Must be one of:
	GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, 
	GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, 
	GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BUFFER, 
	GL_TEXTURE_2D_MULTISAMPLE or GL_TEXTURE_2D_MULTISAMPLE_ARRAY.
	*/
	void BindTexture() {
		if(!textureId)
			glGenTextures(1, &textureId);
		glBindTexture(renderType, textureId);
	}

	/*
	Target:
	Must be one of:
	GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, 
	GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D_ARRAY, GL_TEXTURE_RECTANGLE, 
	GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BUFFER, 
	GL_TEXTURE_2D_MULTISAMPLE or GL_TEXTURE_2D_MULTISAMPLE_ARRAY.

	ParameterName:
	Specifies the symbolic name of a single-valued texture parameter. 
	pname can be one of the following: 
	GL_DEPTH_STENCIL_TEXTURE_MODE, GL_TEXTURE_BASE_LEVEL, GL_TEXTURE_COMPARE_FUNC,
	GL_TEXTURE_COMPARE_MODE, GL_TEXTURE_LOD_BIAS, GL_TEXTURE_MIN_FILTER,
	GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_LOD, GL_TEXTURE_MAX_LOD,
	GL_TEXTURE_MAX_LEVEL, GL_TEXTURE_SWIZZLE_R, GL_TEXTURE_SWIZZLE_G,
	GL_TEXTURE_SWIZZLE_B, GL_TEXTURE_SWIZZLE_A, GL_TEXTURE_WRAP_S,
	GL_TEXTURE_WRAP_T, or GL_TEXTURE_WRAP_R.

	Value:
    GL_REPEAT: The default behavior for textures. Repeats the texture image.
    GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat.
    GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
    GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user-specified border color.
	GL_LINEAR & GL_NEAREST are the two options for filtering
	*/
	void TextureParameters(GLenum Target, GLenum ParameterName, GLint Value) {
		glTexParameteri(Target, ParameterName, Value);
	}

private:

};
