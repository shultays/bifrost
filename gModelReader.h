#pragma once

#ifndef G_MODEL_READER_H__
#define G_MODEL_READER_H__
#include "gVec.h"
#include <iostream>
#include <fstream>

class gMDLReader {
	std::ifstream myfile;

	std::string line;
	std::string token;
public:

	class GeoSet {
	public:
		std::vector<Vec3> vertices;
		std::vector<Vec3> normals;
		std::vector<Vec2> textureCoors;
		std::vector<int> faces;
		int materialID;
	};

	std::vector<GeoSet> geosets;
	std::vector<std::string> textures;
	std::vector<int> materialTextures;

	bool getToken() {
		myfile >> token;
		if (myfile.eof()) {
			return false;
		}
		if (token == "//") {
			getline(myfile, line);
			return getToken();
		}
		return true;
	}

	void readMDLFile(const char *fileName) {
		myfile.open(fileName);
		if (!myfile.is_open()) {
			return;
		}
		while (getToken()) {
			if (token == "Geoset") {
				readGeoSet();
			} else if (token == "Textures") {
				readTextures();
			} else if (token == "Materials") {
				readMaterials();
			} else {
				ignoreToken();
			}
		}
	}

	void readMaterials() {
		int materialCount;
		myfile >> materialCount;
		getToken();//{

		int pCount = 1;
		while (pCount) {
			getToken();
			if (token == "TextureID") {
				int materialTexture;
				myfile >> materialTexture;
				materialTextures.push_back(materialTexture);
			} else if (token == "{") {
				pCount++;
			} else if (token == "}") {
				pCount--;
			}
		}


	}
	void readTextures() {
		int textureCount;
		myfile >> textureCount;

		getToken();//{

		for (int i = 0; i < textureCount; i++) {

			getToken();//Bitmap 
			getToken();//{
			getToken();//Image
			getline(myfile, line);

			line = line.substr(2, line.length() - 8);
			unsigned found = line.find_last_of("/\\");
			if (found != std::string::npos) {
				found++;
				line = line.substr(found, line.length() - found);
			}
			textures.push_back(line);
			getToken();//}
		}
		getToken();//}
	}

	void readGeoSet() {
		GeoSet geoset;
		geoset.materialID = -1;
		getToken();//{
		while (getToken()) {
			if (token == "}") break;

			if (token == "Vertices") {
				readVec3Array(geoset.vertices);
			} else if (token == "Normals") {
				readVec3Array(geoset.normals);
			} else if (token == "TVertices") {
				readVec2Array(geoset.textureCoors);
			} else if (token == "Faces") {
				int one;
				myfile >> one;
				assert(one == 1);
				int count;
				myfile >> count;
				getToken(); //{
				getToken(); getToken();//Triangles {
				getToken(); // {
				geoset.faces.clear();
				geoset.faces.resize(count);
				for (int i = 0; i < count; i++) {
					int index;
					myfile >> index;
					geoset.faces[i] = index;
					getToken(); //, 
				}
				getToken(); //}
				getToken(); //}
			} else if (token == "MaterialID") {
				myfile >> geoset.materialID;
				getToken(); //,
			} else {
				ignoreToken();
			}
		}
		geosets.push_back(geoset);
	}

	void ignoreToken() {
		getline(myfile, line);
		int pCount = 0;

		for (unsigned i = 0; i < line.length(); i++) {
			if (line[i] == '{') pCount++;
			if (line[i] == '}') pCount--;
		}

		char c;
		while (pCount) {
			myfile >> c;
			if (c == '{') pCount++;
			if (c == '}') pCount--;
		}
	}

	void readVec3Array(std::vector<Vec3>& arr) {
		arr.clear();
		int count;
		myfile >> count;
		arr.resize(count);
		getToken();//{
		for (int i = 0; i < count; i++) {
			getToken();//{
			float x, y, z;
			myfile >> x;
			getToken();//,
			myfile >> y;
			getToken();//,
			myfile >> z;
			getToken();//},
			arr[i] = Vec3(x, y, z);
		}
		getToken();//{

	}


	void readVec2Array(std::vector<Vec2>& arr) {
		arr.clear();
		int count;
		myfile >> count;
		arr.resize(count);
		getToken();//{
		for (int i = 0; i < count; i++) {
			getToken();//{
			float x, y;
			myfile >> x;
			getToken();//,
			myfile >> y;
			getToken();//},
			arr[i] = Vec2(x, y);
		}
		getToken();//{

	}
};


class gBLPReader {

public:
	struct blp2header {
		byte    ident[4];
		unsigned compression;
		unsigned flags;
		unsigned width;
		unsigned height;
		unsigned pictureType;
		unsigned pictureSubType;
		unsigned mipMapOffset[16];
		unsigned mipMapSize[16];
	};

	blp2header header;

	std::ifstream myfile;

	void readBLPFile(const char* filename) {

		myfile.open(filename, std::ios::binary);

		if (myfile.is_open() == false) return;

		myfile.read((char*)&header, sizeof(header));

	}
};


#endif