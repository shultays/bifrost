#pragma once

#ifndef G_MODEL_READER_H__
#define G_MODEL_READER_H__
#include "gVec.h"
#include <iostream>
#include <fstream>
using namespace std;

class gModelReader {
public:
	ifstream myfile;

	string line;
	string token;

	class GeoSet {
	public:
		vector<Vec3> vertices;
		vector<Vec3> normals;
		vector<Vec2> textureCoors;
		vector<int> faces;
	};

	vector<GeoSet> geosets;

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

	void readMDLFile(char *fileName) {
		myfile.open(fileName);
		if (!myfile.is_open()) {
			return;
		}
		while (getToken()) {
			if (token == "Geoset") {
				readGeoSet();
			} else {
				ignoreToken();
			}
		}
	}

	void readGeoSet() {
		GeoSet geoset;
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
			} else {
				ignoreToken();
			}
		}
		geosets.push_back(geoset);
	}

	void ignoreToken() {

		printf("ignored %s\n", token.c_str());

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

	void readVec3Array(vector<Vec3>& arr) {
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


	void readVec2Array(vector<Vec2>& arr) {
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



#endif