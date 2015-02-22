#pragma once

#ifndef G_SHADER_H__
#define G_SHADER_H__

#include "gTools.h"
#include <string>
#include <unordered_map>
#include <iostream>


enum {
	UniformTypeFloat,
	UniformTypeVec2,
	UniformTypeVec3,
	UniformTypeVec4,
	UniformTypeMat2,
	UniformTypeMat3,
	UniformTypeMat4,
	UniformTypeInt,
	UniformTypeIntVec2,
	UniformTypeIntVec3,
	UniformTypeIntVec4,
};

class gShader {
	class Uniform {
	public:
		int uniformType;

		GLint location;
		Uniform(int uniformType) {
			this->uniformType = uniformType;
		}
		Uniform() {}

		void setData(void * data) {
			switch (uniformType) {
				case UniformTypeFloat:
					glUniform1fv(location, 1, (GLfloat*)data);
					break;
				case UniformTypeVec2:
					glUniform2fv(location, 1, (GLfloat*)data);
					break;
				case UniformTypeVec3:
					glUniform3fv(location, 1, (GLfloat*)data);
					break;
				case UniformTypeVec4:
					glUniform4fv(location, 1, (GLfloat*)data);
					break;
				case UniformTypeMat2:
					glUniformMatrix2fv(location, 1, false, (GLfloat*)data);
					break;
				case UniformTypeMat3:
					glUniformMatrix3fv(location, 1, false, (GLfloat*)data);
					break;
				case UniformTypeMat4:
					glUniformMatrix4fv(location, 1, false, (GLfloat*)data);
					break;
				case UniformTypeInt:
					glUniform1iv(location, 1, (GLint*)data);
					break;
				case UniformTypeIntVec2:
					glUniform2iv(location, 1, (GLint*)data);
					break;
				case UniformTypeIntVec3:
					glUniform3iv(location, 1, (GLint*)data);
					break;
				case UniformTypeIntVec4:
					glUniform4iv(location, 1, (GLint*)data);
					break;
				default:
					break;
			}
		}
	};

	std::unordered_map<std::string, Uniform> uniforms;

	GLuint vertexShader;
	GLuint pixelShader;
	GLuint shaderProgram;

	GLint positionAttribute;
	GLint normalAttribute;

	int printShaderInfoLog(GLuint obj) {
		int infologLength = 0;
		int charsWritten = 0;
		char *infoLog;

		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

		if (infologLength > 0) {
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
			printf("%s\n", infoLog);
			free(infoLog);
		}
		return infologLength;
	}

	int printProgramInfoLog(GLuint obj) {
		int infologLength = 0;
		int charsWritten = 0;
		char *infoLog;

		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

		if (infologLength > 0) {
			infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
			printf("%s\n", infoLog);
			free(infoLog);
		}
		return infologLength;
	}


	GLuint buildShader(const std::string& source, int shaderType) {
		assert(shaderType == GL_VERTEX_SHADER || shaderType == GL_FRAGMENT_SHADER);


		GLuint shader = glCreateShader(shaderType);
		const char* cSource = source.c_str();
		glShaderSource(shader, 1, &cSource, NULL);

		glCompileShader(shader);
		int infoSize = printShaderInfoLog(shader);
		if (infoSize) {
			return 0;
		}
		return shader;
	}

	GLuint loadShader(char *fileName, int shaderType) {
		std::string shaderSource;
		if (!textFileRead(fileName, shaderSource)) {
			std::cout << "Cannot load shader file : " << fileName;
			return 0;
		}
		return buildShader(shaderSource, shaderType);
	}
public:

	gShader() {

	}

	bool loadFromFile(char* vertexShaderFile, char* pixelShaderFile) {
		vertexShader = loadShader(vertexShaderFile, GL_VERTEX_SHADER);
		if (!vertexShader) {
			return false;
		}
		pixelShader = loadShader(pixelShaderFile, GL_FRAGMENT_SHADER);
		if (!pixelShader) {
			return false;
		}

		shaderProgram = glCreateProgram();
		if (!shaderProgram) {
			return false;
		}
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, pixelShader);

		glLinkProgram(shaderProgram);
		int infoSize = printProgramInfoLog(shaderProgram);
		if (infoSize) {
			return false;
		}
		glUseProgram(shaderProgram);
		addDefaultUniforms();
		addDefaultAttribs();

		return true;
	}

	void begin() {
		glUseProgram(shaderProgram);
	}
	void addDefaultAttribs() {
		positionAttribute = glGetAttribLocation(shaderProgram, "aVertexPosition");
		glEnableVertexAttribArray(positionAttribute);
		normalAttribute = glGetAttribLocation(shaderProgram, "aVertexNormal");
		glEnableVertexAttribArray(normalAttribute);
	}

	void addDefaultUniforms() {
		addUniform("uWorldViewMatrix", UniformTypeMat4);
		addUniform("uProjectionMatrix", UniformTypeMat4);
		addUniform("uColor", UniformTypeVec4);
	}
	void addUniform(char* name, int uniformType) {
		Uniform& uniform = uniforms[name] = Uniform(uniformType);
		uniform.location = glGetUniformLocation(shaderProgram, name);
	}

	GLuint getNormalAttrib() {
		return normalAttribute;
	}
	GLuint getPositionAttrib() {
		return positionAttribute;
	}

	void setWorldViewMatrix(const Mat4& mat) {
		setUniform("uWorldViewMatrix", mat);
	}

	void setProjectionMatrix(const Mat4& mat) {
		setUniform("uProjectionMatrix", mat);
	}
	void setColor(const Vec4& color) {
		setUniform("uColor", color);
	}


	void setUniform(char* name, void* data) {
		uniforms[name].setData(data);
	}

	void setUniform(char* name, float data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, const Vec2& data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, const Vec3& data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, const Vec4& data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, const Mat3& data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, const Mat2& data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, const Mat4& data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, int data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, const IntVec2& data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, const IntVec3& data) {
		uniforms[name].setData((void*)&data);
	}
	void setUniform(char* name, const IntVec4& data) {
		uniforms[name].setData((void*)&data);
	}
};


#endif