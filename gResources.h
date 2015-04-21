#pragma once

#ifndef G_RESOURCES_H__
#define G_RESOURCES_H__

#include "gSharedPtr.h"
#include "gShader.h"
#include "gTexture.h"
#include "gTools.h"

typedef gSharedPtr<gShader> gShaderShr;
typedef gSharedPtr<gTexture> gTextureShr;

class gResources : public gCustomDeallocator<gShader>, public gCustomDeallocator<gTexture> {


	void objectFreed(gShader* object) override {
		std::string ID = shaderID(object->getVertexShaderFileName(), object->getPixelShaderFileName());
		std::unordered_map<std::string, gShaderShr>::iterator got = shaders.find(ID);
		assert(got != shaders.end());
		got->second.setCustomDeallocator(nullptr);
		shaders.erase(got);
	}

	void objectFreed(gTexture* object) override {
		std::string ID = textureID(object->getName());
		std::unordered_map<std::string, gTextureShr>::iterator got = textures.find(ID);
		assert(got != textures.end());
		got->second.setCustomDeallocator(nullptr);
		textures.erase(got);
	}

	std::unordered_map<std::string, gShaderShr> shaders;
	std::unordered_map<std::string, gTextureShr> textures;

	std::string shaderID(const char* vertexShaderFile, const char* pixelShaderFile) const {
		std::string ID = vertexShaderFile;
		ID += pixelShaderFile;
		return ID;
	}

	std::string textureID(const char* textureName) const {
		std::string ID = textureName;
		return ID;
	}

public:

	gResources(){

	}
	~gResources(){
		for ( unsigned i = 0; i < shaders.bucket_count(); ++i) {
			for ( auto local_it = shaders.begin(i); local_it != shaders.end(i); ++local_it )
			{
				local_it->second.setCustomDeallocator(nullptr);
			}
		}

		for ( unsigned i = 0; i < textures.bucket_count(); ++i) {
			for ( auto local_it = textures.begin(i); local_it != textures.end(i); ++local_it )
			{
				local_it->second.setCustomDeallocator(nullptr);
			}
		}
	}
	gShaderShr getShader(const char* vertexShaderFile, const char* pixelShaderFile) {
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

	gTextureShr getTexture(const char* textureName) {
		std::string ID = textureID(textureName);
		std::unordered_map<std::string, gTextureShr>::iterator got = textures.find(ID);
		if (got != textures.end()) {
			return got->second;
		}

		gTextureShr texture = new gTexture(textureName);
		texture.setCustomDeallocator(this);

		textures[ID] = texture;
		return texture;
	}
};

#endif