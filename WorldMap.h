#pragma once

#ifndef WORLD_MAP_H__
#define WORLD_MAP_H__

#include "gVec.h"
#include "PerlinMap.h"
#include "Grid.h"

#include "gTools.h"
#include "gGame.h"
#include "PngExporter.h"
#include <vector>

#include <stdlib.h>


class WorldMap : public gRenderable {
	typedef struct {
		Vec3 pos;
		Vec3 normal;
	}Vertex;
	PerlinMap perlinMap;
	float mapSize;
	float gridSize;
	int edgeCount;
	Grid<float> heightMap;
	Grid<Vec3> normalMap;
	GLuint vertexBuffer;
	GLuint indexBuffer;
public:
	WorldMap(float mapSize, int edgeCount) :heightMap(edgeCount), perlinMap(mapSize), normalMap(edgeCount) {
		this->mapSize = mapSize;
		this->edgeCount = edgeCount;
		this->gridSize = mapSize / edgeCount;

		//perlinMap.addPerlinShell(6, 0.0, 400.0f, 0.95f);


		perlinMap.addPerlinShell(5, 0.0, 100.0f);
		perlinMap.addPerlinShell(9, 0.0, 50.0f);
		perlinMap.addPerlinShell(18, 0.0, 25.0f);
		perlinMap.addPerlinShell(10, 0.0, 90.0f, 0.85f);
		perlinMap.addPerlinShell(11, 0.0, 90.0f, 0.85f);
		perlinMap.addPerlinShell(12, 0.0, 90.0f, 0.85f);
		perlinMap.addPerlinShell(13, 0.0, 90.0f, 0.85f);


		perlinMap.addPerlinShell(5, 0.0, 100.0f);
		perlinMap.addPerlinShell(9, 0.0, 50.0f);
		perlinMap.addPerlinShell(18, 0.0, 25.0f);
		perlinMap.addPerlinShell(37, 0.0, 12.0f);
		perlinMap.addPerlinShell(85, 0.0, 5.0f);
		perlinMap.addPerlinShell(127, 0.0, 2.0f);
		perlinMap.addPerlinShell(255, 0.0, 1.0f);
	}

	void build() {
		buildHeightMap();

		buildNormalMap();
		buildBuffer();

		PngExporter::writeGridToPng("images/normalMap.png", normalMap, ExportTypeVec3AsNormal);
		PngExporter::writeGridToPng("images/heightMap.png", heightMap, ExportTypeVec3ZOnly);

	}

	void buildHeightMap() {
		heightMap.setAll(0.0f);

		for (int i = 0; i < edgeCount; i++) {
			for (int j = 0; j < edgeCount; j++) {
				heightMap[i][j] = perlinMap.getHeightAt(Vec2(i*gridSize, j*gridSize));
			}
		}
	}

	void buildNormalMap() {
		for (int i = 1; i < edgeCount - 1; i++) {
			for (int j = 1; j < edgeCount - 1; j++) {


				float dt = 1.0f*gridSize;
				float sx = perlinMap.getHeightAt(Vec2(i*gridSize + dt, j*gridSize)) - perlinMap.getHeightAt(Vec2(i*gridSize - dt, j*gridSize));
				float sy = perlinMap.getHeightAt(Vec2(i*gridSize, j*gridSize + dt)) - perlinMap.getHeightAt(Vec2(i*gridSize, j*gridSize - dt));



				normalMap[i][j] = Vec3(-sx, sy, 2.0f*dt);
				normalMap[i][j].normalize();
			}
		}
		for (int i = 0; i < edgeCount; i++) {
			normalMap[i][0] = Vec3(0.0f, 0.0f, 1.0f);
			normalMap[i][edgeCount - 1] = Vec3(0.0f, 0.0f, 1.0f);

			normalMap[edgeCount - 1][i] = Vec3(0.0f, 0.0f, 1.0f);
			normalMap[0][i] = Vec3(0.0f, 0.0f, 1.0f);
		}
	}

	void buildBuffer() {


		Vertex* vertices = new Vertex[edgeCount*edgeCount];

		for (int i = 0; i < edgeCount; i++) {
			for (int j = 0; j < edgeCount; j++) {
				vertices[i + j*edgeCount].pos = Vec3((i - edgeCount / 2.0f)*gridSize, (j - edgeCount / 2.0f)*gridSize, heightMap[i][j]);
				vertices[i + j*edgeCount].normal = normalMap[i][j];
			}
		}

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, edgeCount*edgeCount*sizeof(Vertex), vertices, GL_STATIC_DRAW);
		unsigned int *indices = new unsigned int[(edgeCount - 1)*(edgeCount - 1) * 6];


		for (int i = 0; i < (edgeCount - 1); i++) {
			for (int j = 0; j < (edgeCount - 1); j++) {

				indices[6 * (i + j*(edgeCount - 1)) + 0] = i + j*edgeCount;
				indices[6 * (i + j*(edgeCount - 1)) + 1] = (i + 1) + j*edgeCount;
				indices[6 * (i + j*(edgeCount - 1)) + 2] = i + (j + 1)*edgeCount;

				indices[6 * (i + j*(edgeCount - 1)) + 3] = (i + 1) + j*edgeCount;
				indices[6 * (i + j*(edgeCount - 1)) + 4] = (i + 1) + (j + 1)*edgeCount;
				indices[6 * (i + j*(edgeCount - 1)) + 5] = i + (j + 1)*edgeCount;
			}
		}


		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (edgeCount - 1)*(edgeCount - 1) * 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		delete indices;
		delete vertices;
	}


	virtual void render() override {
		gears.game->shader.setColor(Vec4(1.0, 1.0f, 1.0f, 1.0f));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(gears.game->shader.getPositionAttrib(), 3, GL_FLOAT, false, sizeof(Vertex), 0);
		glVertexAttribPointer(gears.game->shader.getNormalAttrib(), 3, GL_FLOAT, true, sizeof(Vertex), (void*)sizeof(Vec3));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		//glPolygonMode(GL_FRONT, GL_LINE);
		glDrawElements(
			GL_TRIANGLES,
			(edgeCount - 1)*(edgeCount - 1) * 6,
			GL_UNSIGNED_INT,
			(void*)0
			);


		/*
		Mat4 projection = Mat4::perspective(100.0f, 2500.0f, degreeToRadian(90.0f), (float)gears.width, (float)gears.height);
		Mat4 world = gears.game->activeCamera->getLookAt();

		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(0);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(projection.data);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
		glLoadMatrixf(world.data);


		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);


		glColor3f(1.0f, 1.0f, 1.0f);
		for (int i = 1; i < edgeCount - 1; i += 10) {
		for (int j = 1; j < edgeCount - 1; j += 10) {

		Vec3 pos((i - edgeCount / 2.0f)*gridSize, (j - edgeCount / 2.0f)*gridSize, heightMap[i][j]);
		glVertex3fv(pos.data);
		glVertex3fv((pos + normalMap[i][j] * 50.0f).data);


		}
		}


		glEnd();*/
	}

};

#endif