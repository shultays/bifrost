#pragma once

#ifndef G_RESOURCES_H__
#define G_RESOURCES_H__

#include "gSharedPtr.h"
#include "gShader.h"
#include "gTools.h"

typedef gSharedPtr<gShader> gShaderShr;

class gResources : public gCustomDeallocator<gShader> {

	void objectFreed(gShader* object) override {
		std::string ID = shaderID(object->getVertexShaderFileName(), object->getPixelShaderFileName());
		std::unordered_map<std::string, gShaderShr>::iterator got = shaders.find(ID);
		assert(got != shaders.end());
		got->second.setCustomDeallocator(nullptr);
		shaders.erase(got);
	}

	std::unordered_map<std::string, gShaderShr> shaders;
	std::string shaderID(const char* vertexShaderFile, const char* pixelShaderFile) const {
		std::string ID = vertexShaderFile;
		ID += pixelShaderFile;
		return ID;
	}
public:

	gShaderShr loadShader(char* vertexShaderFile, char* pixelShaderFile) {
		std::string ID = shaderID(vertexShaderFile, pixelShaderFile);
		std::unordered_map<std::string, gShaderShr>::iterator got = shaders.find(ID);
		if (got != shaders.end()) {
			return got->second;
		}

		gShaderShr shader = new gShader();
		shader.setCustomDeallocator(this);

		shader->loadFromFile(vertexShaderFile, pixelShaderFile);
		shaders[ID] = shader;
		return shader;
	}


};

#endif