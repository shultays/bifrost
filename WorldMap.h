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

#include "gTexture.h"

class WorldMap : public gRenderable {
	typedef struct {
		Vec3 pos;
		Vec3 normal;
		Vec4 color;
		Vec2 uv;
	}Vertex;

	PerlinMap perlinMap;
	PerlinMap earthMap;

	float mapSize;
	float gridSize;
	int edgeCount;

	Grid<float> heightMap;
	Grid<Vec3> normalMap;
	Grid<Vec3> colorMap;

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint waterBuffer;

	gTexture* texture;
public:
	WorldMap() : WorldMap(512 * 1024.0f, 128) {

	}

	WorldMap(float mapSize, int edgeCount) :heightMap(edgeCount), perlinMap(mapSize), normalMap(edgeCount), earthMap(mapSize), colorMap(edgeCount) {
		this->mapSize = mapSize;
		this->edgeCount = edgeCount;
		this->gridSize = mapSize / edgeCount;

		indexBuffer = vertexBuffer = waterBuffer = -1;

		texture = new gTexture("images/lena.png");
	}

	void build() {
		perlinMap.clear();
		earthMap.clear();


		float maxHeight = 10000.0f;

		perlinMap.addPerlinShell(10, 0.0f, 4 * maxHeight * 0.05f, 0.95f, 2.0f);
		perlinMap.addPerlinShell(11, 0.0f, 4 * maxHeight * 0.05f, 0.95f, 2.0f);
		perlinMap.addPerlinShell(13, 0.0f, 4 * maxHeight * 0.1f, 0.95f, 2.0f);
		perlinMap.addPerlinShell(14, 0.0f, 4 * maxHeight * 0.2f, 0.85f, 2.0f);
		perlinMap.addPerlinShell(15, 0.0f, 4 * maxHeight * 0.1f, 0.75f, 2.0f);
		perlinMap.addPerlinShell(15, 0.0f, 4 * maxHeight * 0.1f, 0.75f, 2.0f);
		//perlinMap.addPerlinShell(15, 0.0f, 4 * maxHeight * 0.1f, 0.75f, 2.0f);

		perlinMap.addPerlinShell(20, -1000.0, 0.0f, 0.5f);

		float remainingHeight = 0.4f * maxHeight;

		int start = 5;
		do {
			float h = remainingHeight*0.5f;
			perlinMap.addPerlinShell(start, 0.0f, h);
			remainingHeight -= h;
			start = (int)(start * 2.1f);
		} while (start < edgeCount);

		perlinMap.addPerlinShell(start, 0.0f, remainingHeight);


		earthMap.addPerlinShell(03, 0.0, 0.30f, 0.90f);
		earthMap.addPerlinShell(04, 0.0, 0.20f, 0.90f);
		earthMap.addPerlinShell(06, 0.0, 0.15f, 0.90f);
		earthMap.addPerlinShell(15, 0.0, 0.15f, 0.90f);
		earthMap.addPerlinShell(37, 0.0, 0.15f, 0.60f);
		earthMap.addPerlinShell(55, 0.0, 0.05f, 0.50f);
		earthMap.addPerlinShell(85, 0.0, 0.05f, 0.50f);

		if (indexBuffer != -1) {
			glDeleteBuffers(1, &indexBuffer);
			glDeleteBuffers(1, &vertexBuffer);
			glDeleteBuffers(1, &waterBuffer);
		}
		buildHeightMap();
		buildNormalMap();
		buildColorMap();
		buildBuffer();

		PngExporter::writeGridToPng("images/normalMap.png", normalMap, ExportTypeVec3AsNormal);
		PngExporter::writeGridToPng("images/heightMap.png", heightMap);
		PngExporter::writeGridToPng("images/earthMap.png", earthMap, edgeCount, Vec3(0.0f), Vec3(0.0f, 1.0f, 0.0));
		PngExporter::writeGridToPng("images/color.png", colorMap, ExportTypeVec3AsColor);




	}

	void buildHeightMap() {
		heightMap.setAll(0.0f);

		for (int i = 0; i < edgeCount; i++) {
			for (int j = 0; j < edgeCount; j++) {
				heightMap[i][j] = perlinMap.getHeightAt(Vec2(i*gridSize, j*gridSize));
			}
		}


		float min = FLT_MAX, max = FLT_MIN;


		for (int i = 0; i < edgeCount; i++) {
			for (int j = 0; j < edgeCount; j++) {
				min = gmin(min, heightMap[i][j]);
				max = gmax(max, heightMap[i][j]);
			}
		}

		// level land around water
		for (int i = 0; i < edgeCount; i++) {
			for (int j = 0; j < edgeCount; j++) {
				heightMap[i][j] -= 1500.0f;
				if (heightMap[i][j] < 20.0f) {
					heightMap[i][j] = (heightMap[i][j] - 20.0f)*0.5f;
				} else if (heightMap[i][j] < 80.0f) {
					heightMap[i][j] = (heightMap[i][j] - 20.0f)* 80.0f / 60.0f;
				}
			}
		}

		//fix too small areas
		for (int i = 5; i < edgeCount - 5; i++) {
			for (int j = 5; j < edgeCount - 5; j++) {
				if (heightMap[i][j] < 0) {
					int waterToAccept = 20;
					for (int a = i - 5; a < i + 5; a++) {
						for (int b = j - 5; b < j + 5; b++) {
							if (heightMap[a][b] < 0) {
								waterToAccept--;
								if (!waterToAccept) break;
							}
						}
					}
					if (waterToAccept > 0) {
						heightMap[i][j] = -heightMap[i][j] + 0.1f;;
					}
				} else {
					int landToAccept = 20;
					for (int a = i - 5; a < i + 5; a++) {
						for (int b = j - 5; b < j + 5; b++) {
							if (heightMap[a][b] > 0) {
								landToAccept--;
								if (!landToAccept) break;
							}
						}
					}
					if (landToAccept > 0) {
						heightMap[i][j] = -heightMap[i][j] - 0.1f;;
					}
				}
			}
		}


		min = FLT_MAX, max = FLT_MIN;


		for (int i = 0; i < edgeCount; i++) {
			for (int j = 0; j < edgeCount; j++) {
				min = gmin(min, heightMap[i][j]);
				max = gmax(max, heightMap[i][j]);
			}
		}
		printf("%f %f\n", min, max);

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
	void buildColorMap() {
		for (int i = 0; i < edgeCount; i++) {
			for (int j = 0; j < edgeCount; j++) {
				float height = heightMap[i][j];
				float beachW = (2.0f - height) / 200.0f;
				saturate(beachW);
				float snowW = (height - 5000.0f) / 1000.0f;
				saturate(snowW);


				float forestW = earthMap.getHeightAt(Vec2(i*gridSize, j*gridSize));
				saturate(forestW);

				Vec3 finalColor = Vec3::zero();



				finalColor += lerpVec(Vec3(0.9f), Vec3(1.0f), forestW) * snowW;

				finalColor += lerpVec(Vec3::fromColor(0xB7A888), Vec3::fromColor(0x80765F), forestW) * beachW;

				float remainingWeight = 1.0f - snowW - beachW;


				finalColor += lerpVec(Vec3::fromColor(0x477519), Vec3::fromColor(0x663300), forestW) * remainingWeight;
				colorMap[i][j] = finalColor;
				/*
				if (height < 500) {
				colorMap[i][j] = 0.0f;
				} else {
				int t = (int)((height - 500) / 500.0f);
				colorMap[i][j] = Vec3(t / 5.0f);
				}*/

			}
		}

	}
	void buildBuffer() {
		Vertex* vertices = new Vertex[edgeCount*edgeCount];

		for (int i = 0; i < edgeCount; i++) {
			for (int j = 0; j < edgeCount; j++) {
				vertices[i + j*edgeCount].pos = Vec3((i - edgeCount / 2.0f)*gridSize, (j - edgeCount / 2.0f)*gridSize, heightMap[i][j]);
				vertices[i + j*edgeCount].normal = normalMap[i][j];
				vertices[i + j*edgeCount].color = Vec4(colorMap[i][j], 1.0f);
				vertices[i + j*edgeCount].uv = Vec2(((float)i) / edgeCount, ((float)j) / edgeCount);
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


		Vertex water[6];
		water[0].pos.x = -mapSize / 2;
		water[0].pos.y = -mapSize / 2;
		water[0].pos.z = 0.0f;
		water[1].pos.x = +mapSize / 2;
		water[1].pos.y = -mapSize / 2;
		water[1].pos.z = 0.0f;
		water[2].pos.x = -mapSize / 2;
		water[2].pos.y = +mapSize / 2;
		water[2].pos.z = 0.0f;

		water[3].pos.x = +mapSize / 2;
		water[3].pos.y = -mapSize / 2;
		water[3].pos.z = 0.0f;
		water[4].pos.x = +mapSize / 2;
		water[4].pos.y = +mapSize / 2;
		water[4].pos.z = 0.0f;
		water[5].pos.x = -mapSize / 2;
		water[5].pos.y = +mapSize / 2;
		water[5].pos.z = 0.0f;

		water[0].normal = water[1].normal = water[2].normal
			= water[3].normal = water[4].normal = water[5].normal = Vec3(0.0f, 0.0f, 1.0f);

		water[0].color = water[1].color = water[2].color
			= water[3].color = water[4].color = water[5].color = Vec4(0.0f, 0.0f, 1.0f, 0.5f);

		glGenBuffers(1, &waterBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, waterBuffer);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), water, GL_STATIC_DRAW);


	}


	virtual void render() override {
		gears.game->shader.setWorldMatrix(Mat4::scale(Vec3(0.003f, 0.003f, 0.003f)));;

		gears.game->shader.setColor(Vec4(1.0, 1.0f, 1.0f, 1.0f));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);


		gears.game->shader.bindPosition(sizeof(Vertex), 0);
		gears.game->shader.bindNormal(sizeof(Vertex), sizeof(Vec3));
		gears.game->shader.bindColor(sizeof(Vertex), sizeof(Vec3)+sizeof(Vec3));
		gears.game->shader.bindUV(sizeof(Vertex), sizeof(Vec3)+sizeof(Vec3)+sizeof(Vec4));

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, texture->textureID);

		gears.game->shader.setUniform("uTexture0", 0);
		gears.game->shader.setUniform("uTextureCount", 1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		if (input.isKeyDown(GLFW_KEY_A)) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT, GL_FILL);
		}
		glDrawElements(
			GL_TRIANGLES,
			(edgeCount - 1)*(edgeCount - 1) * 6,
			GL_UNSIGNED_INT,
			(void*)0
			);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, waterBuffer);
		gears.game->shader.bindPosition(sizeof(Vertex), 0);
		gears.game->shader.bindNormal(sizeof(Vertex), sizeof(Vec3));
		//gears.game->shader.bindColor(sizeof(Vertex), sizeof(Vec3)+sizeof(Vec3));
		gears.game->shader.setAttributeConstant("aVertexColor", Vec4(1.0f, 0.0f, 1.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);

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


		gears.game->shader.setWorldMatrix(Mat4::identity());
	}

};

#endif